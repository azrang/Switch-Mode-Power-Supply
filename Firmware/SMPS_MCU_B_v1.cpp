// MCU B
// Switch Mode Power Supply - Senior Projects
// Azra Rangwala, Ilona Lameka, Ridwan Hussain

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

// ADC Pins
const int CUR_SENSE = 36; // MCU_B_ADC1_C0
const int B_VOL_SENSE  = 39; // MCU_B_ADC1_C3
const int V_KNOB = 34; // MCU_B_ADC1_C6
const int C_KNOB = 35; // MCU_B_ADC1_C7
const int USB_VOL_SENSE = 32; // MCU_B_ADC1_C4

// GPIO Input Pins
const int TEMP = 33; // MCU_B_GPIO33
const int B_FUSE = 12; // MCU_B_GPIO12
const int B_BUTT = 23; // MCU_B_GPIO23
const int USB_BUTT = 17; // MCU_B_GPIO17
const int FLT_ALERT = 16; // MCU_B_GPIO16


// GPIO Output Pins
const int BUCK = 25; // MCU_B_GPIO25
const int OUT_ENABLE = 26; // MCU_B_GPIO26
const int USB_ENABLE = 13; // MCU_B_GPIO13
const int B_DRIVE = 19; // MCU_B_GPIO19
const int HV_LV_OUT = 18; // MCU_B_GPIO18
const int USB_DRIVE = 4; // MCU_B_GPIO4

void dominos (void*);
void subway (void*);
void death (void*);
void lcdScreen(float, float);
void readKnobs();
void readSense();
void IRAM_ATTR isrTemp();
void IRAM_ATTR onSetupTimer();
void IRAM_ATTR onOutputTimer();
void flyDrive(float);
void buckDrive();

int lcdColumns = 16;
int lcdRows = 2;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows); 

float vPotCalc, cPotCalc, vSenseCalc, cSenseCalc; //set to 0?
int stateB, B_drivePWM, buckPWM;
int prevStateB = 0;
int f1 = 0;
hw_timer_t* setupTimer = NULL;
hw_timer_t* outputTimer = NULL;

volatile bool interrupt = false;
volatile int faultState;
enum fault{OVERTEMP, SETUPOVERTIME, OUTPUTOVERTIME,  OVERCURRENT, BLOWNFUSE};

TaskHandle_t domTaskHandle = NULL;
TaskHandle_t subTaskHandle = NULL;
TaskHandle_t deathTaskHandle = NULL;

void setup() 
{
  Serial.begin (115200);
  Serial.println("MCU Restarting...");
  delay(100);
  lcd.init();                  
  lcd.backlight();

  pinMode(B_BUTT, INPUT);
  pinMode(USB_BUTT, INPUT);
  pinMode(B_FUSE, INPUT);
  
  pinMode(TEMP, INPUT);
  attachInterrupt(TEMP, isrTemp, ONLOW);

  ledcSetup(0, 50000, 10);
  ledcAttachPin(B_DRIVE, 0);

  ledcSetup(1, 50000, 10);
  ledcAttachPin(BUCK, 1);

  pinMode(OUT_ENABLE, OUTPUT);
  pinMode(HV_LV_OUT, OUTPUT);

  setupTimer = timerBegin(0,80, true);
  timerAttachInterrupt(setupTimer, &onSetupTimer, true);
  timerAlarmWrite(setupTimer, 30000000, false); //30s

  outputTimer = timerBegin(1,80, true);
  timerAttachInterrupt(outputTimer, &onOutputTimer, true);
  timerAlarmWrite(outputTimer, 10000000, false); //10s

  // False Start Prevention
  if (digitalRead(B_BUTT) || digitalRead(USB_BUTT))
  {
    while(1)
    {
      lcd.setCursor(0, 0);
      lcd.print("Turn Off Buttons");
    }
  }
  xTaskCreatePinnedToCore(dominos, "Dominatrix", 2048, NULL, 2, &domTaskHandle, 0);
  xTaskCreatePinnedToCore(subway, "Sub", 2048, NULL, 2, &subTaskHandle, 1);
}

void dominos (void* pvParameters) {
  for (;;) {
    readSense();
    if (interrupt && !f1)
    {
      vTaskDelete(subTaskHandle);
      vTaskDelay(pdMS_TO_TICKS(5));
      xTaskCreatePinnedToCore(death, "death", 2048, NULL, 8, &deathTaskHandle, 1);
      f1 = 1;
    }
    vTaskDelay(pdMS_TO_TICKS(5)); // Feeding the dog
  }
}


void subway(void *pvParameters)
{
  for (;;)
  {
    Serial.println("subway");
    stateB = digitalRead(B_BUTT);
    if(stateB == 0)
    {
      ledcWrite(0, 0); //Drive
      ledcWrite(1, 0); //Buck
      digitalWrite(HV_LV_OUT, 0);
      digitalWrite(OUT_ENABLE, 0);
      readKnobs();
      lcdScreen(vPotCalc, cPotCalc);
      prevStateB = 0;
      vTaskDelay(pdMS_TO_TICKS(10));
    }
    else if((stateB == 1) && (prevStateB == 0))
    {
      timerRestart(setupTimer);
      timerAlarmEnable(setupTimer);
      if (vPotCalc > 10)
      {
        flyDrive(vPotCalc);
      }
      else
      {
        flyDrive(12);
        if (digitalRead(B_BUTT) && !interrupt)
        {
          digitalWrite(HV_LV_OUT, 1);
        }
        buckDrive();
      }
      timerAlarmDisable(setupTimer);
      prevStateB = 1;
    }
    else
    {
      Serial.println("s3");
      digitalWrite(OUT_ENABLE, 1);
      lcdScreen(vSenseCalc, cSenseCalc);
      if ((cSenseCalc - cPotCalc) > 0.01) 
      {
        vTaskDelay(pdMS_TO_TICKS(15)); //delay to make sure its not a current spike
        if ((cSenseCalc - cPotCalc) > 0.01)
        {
          faultState = OVERCURRENT;
          interrupt = true;
        }
      }
      if (abs(vPotCalc-vSenseCalc) > 0.05)
      {
        timerRestart(outputTimer);
        timerAlarmEnable(outputTimer);
        if (vPotCalc > 10)
        {
          Serial.println("trying");
          flyDrive(vPotCalc);
        }
        else
        {
          buckDrive();
        }
        timerAlarmDisable(outputTimer);
      }
      if (digitalRead(B_FUSE) && (vSenseCalc < 0.1)) //change v value
      {
        faultState = BLOWNFUSE;
        interrupt = true;
      }
      vTaskDelay(pdMS_TO_TICKS(15));
    }
  }
}

void readKnobs()
{
  // Getting the ADC Knob Values
  int vPotTotal = 0;
  int cPotTotal = 0;
  for (int i = 0; i < 50; i++) // Getting muliple samples due to ADC fluctuations
  {
    vPotTotal += analogRead(V_KNOB);
    cPotTotal += analogRead(C_KNOB);
    vTaskDelay(pdMS_TO_TICKS(1));
  }
  int vPot = vPotTotal / 50; 
  vPotCalc = ((vPot / 4095.0) * 19.0) + 1;

  int cPot = cPotTotal / 50;
  cPotCalc = (cPot / 4095.0) * 3.0;
}

void readSense()
{
  int vSense = analogRead(B_VOL_SENSE);
  vSenseCalc = ((vSense / 4095.0) * 19.0); // math might have to change for v and c

  int cSense = analogRead(CUR_SENSE);
  cSenseCalc = (cSense / 4095.0) * 3.0;
}

void lcdScreen(float v, float c) 
{   
    lcd.setCursor(0, 0);
    char vDisplay[6];  
    dtostrf(v, 4, 2, vDisplay); 
    lcd.print(String("Voltage: ") + String(vDisplay) + String(" V       "));

    lcd.setCursor(0,1);
    char cDisplay[6];  
    dtostrf(c, 4, 2, cDisplay); 
    lcd.print(String("Current: ") + String(cDisplay) + String(" A"));
}

void IRAM_ATTR isrTemp() 
{
  detachInterrupt(TEMP);
  faultState = OVERTEMP;
  interrupt = true;
}

void IRAM_ATTR onSetupTimer()
{
  timerAlarmDisable(setupTimer);
  timerDetachInterrupt(setupTimer);
  faultState = SETUPOVERTIME;
  interrupt = true;
}

void IRAM_ATTR onOutputTimer()
{
  timerAlarmDisable(outputTimer);
  timerDetachInterrupt(outputTimer);
  faultState = OUTPUTOVERTIME;
  interrupt = true;
}

void death (void* pvParameters)
{
  for (;;)
  {
    Serial.println("dying");
    detachInterrupt(TEMP);
    ledcWrite(0, 0);
    ledcWrite(1, 0);
    digitalWrite(OUT_ENABLE, 0); 
    digitalWrite(HV_LV_OUT, 0);
    lcd.setCursor(0, 0);
    Serial.println("this again");
    switch(faultState)
    {
      case OVERTEMP:
      {
        lcd.print("Over Temperature");
        break;
      }
      case SETUPOVERTIME:
      {
        lcd.print("Setup Overtime    ");
        break;
      }
      case OUTPUTOVERTIME:
      {
        lcd.print("Output Overtime    ");
        break;
      }
      case OVERCURRENT:
      {
        lcd.print("Over Current    ");
        break;
      }
      case BLOWNFUSE:
      {
        lcd.print("Blown Fuse       ");
        break;
      }
    
    }
    lcd.setCursor(0, 1);
    lcd.print("                    ");
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

// Controls the Flyback PWM signals
void flyDrive(float vComp) 
{
  float err = 2; 
  if (prevStateB == 0) // Only allows change to the PWM rate if a new Pot Value was given
  {
    B_drivePWM = (((vComp - 10) / 10) * 154) + 256; //fix math
    ledcWrite(0, B_drivePWM);
  }

  while(!interrupt && (abs(err) > 0.05) && digitalRead(B_BUTT)) 
  {
    B_drivePWM =  ((err / 10) * 500); //fix math
    ledcWrite(0, B_drivePWM);
    Serial.println(B_drivePWM);
    vTaskDelay(pdMS_TO_TICKS(2));
    err = vComp - vSenseCalc;
  }
  vTaskDelay(pdMS_TO_TICKS(5));
}

// Controls the Buck Converter PWM signals
void buckDrive() 
{
  float err = 2; 
  if (prevStateB == 0)
  {
    buckPWM = (((vPotCalc - 10) / 10) * 154) + 256;
    ledcWrite(0, buckPWM);
  }
  while(!interrupt && (abs(err) > 0.05) && digitalRead(B_BUTT))
  {
    buckPWM = ceil((((err / 10) * 154) + 256));
    ledcWrite(1, buckPWM);
    Serial.println("buck");
    Serial.println(buckPWM);
    vTaskDelay(pdMS_TO_TICKS(2));
    err = vPotCalc - vSenseCalc;
  }
}

void loop() {
  // Empty loop necessary for FreeRTOS
}
