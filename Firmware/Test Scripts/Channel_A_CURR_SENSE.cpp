// MCU Testing Script - Channel A Current Sense & OVP
// Channel_A_CURR_SENSE.cpp
// Switch Mode Power Supply - Senior Projects
// Azra Rangwala, Ilona Lameka, Ridwan Hussain

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

const int CUR_SENSE = 36; // MCU_A_ADC1_C0
const int A_BUTT = 23; // MCU_A_GPIO23
const int BUCK = 25; // MCU_A_GPIO25
const int HV_LV_OUT = 18; // MCU_A_GPIO18
const int DRIVE = 4; // MCU_A_GPIO4

void dominos (void*); // Core 0
void subway (void*);  // Core 1
void lcdScreen(float, float);

void readSense();
void flyDrive(float);
void buckDrive();

int lcdColumns = 16;
int lcdRows = 2;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows); 

float vSenseCalc, cSenseCalc; //set to 0?
int stateA, drivePWM, buckPWM;
int prevStateA = 0;
int f1 = 0;

TaskHandle_t domTaskHandle = NULL;
TaskHandle_t subTaskHandle = NULL;

void setup() 
{
  //Serial.begin (115200);
  //Serial.println("MCU Restarting...");
  lcd.init();                  
  lcd.backlight();

  pinMode(A_BUTT, INPUT);

  ledcSetup(0, 42000, 10);
  ledcAttachPin(DRIVE, 0);

  ledcSetup(1, 10000, 10);
  ledcAttachPin(BUCK, 1);

  pinMode(HV_LV_OUT, OUTPUT);

  // False Start Prevention
  if (digitalRead(A_BUTT))
  {
    while(1)
    {
      lcd.setCursor(0, 0);
      lcd.print("Turn Off Buttons");
      lcd.setCursor(0, 1);
      lcd.print("Restart SMPS               ");
    }
  }
  xTaskCreatePinnedToCore(dominos, "Dominatrix", 2048, NULL, 2, &domTaskHandle, 0);
  xTaskCreatePinnedToCore(subway, "Sub", 2048, NULL, 2, &subTaskHandle, 1);
}

// Always running on Core 0
// Output + Interrupt Sensing
void dominos (void* pvParameters) {
  for (;;) {
    readSense();
    vTaskDelay(pdMS_TO_TICKS(5)); // Feeding the dog
  }
}


void subway(void *pvParameters)
{
  for (;;)
  {
    stateA = digitalRead(A_BUTT);
    if(stateA == 0)
    {
      ledcWrite(0, 0); //Drive
      ledcWrite(1, 0); //Buck
      digitalWrite(HV_LV_OUT, 0);
      lcdScreen(vPotCalc, cPotCalc);
      prevStateA = 0;
      vTaskDelay(pdMS_TO_TICKS(10));
    }
    else if((stateA == 1) && (prevStateA == 0))  // possibly two timers HV vs LV timer
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
        if (digitalRead(A_BUTT) && !interrupt)
        {
          digitalWrite(HV_LV_OUT, 1);
        }
        buckDrive();
      }
      timerAlarmDisable(setupTimer);
      prevStateA = 1;
    }
    else
    {
      Serial.println("s3");
      digitalWrite(OUT_ENABLE, 1);
      lcdScreen(vSenseCalc, cSenseCalc);
      // Fuse Check
      if (digitalRead(A_FUSE) && !(vSenseCalc > 0)) //check fi this actually works
      {
        faultState = BLOWNFUSE;
        interrupt = true;
      }
      // Current Check
      if ((cSenseCalc - cPotCalc) > 0.01) // if big difference in curret, kill immediately, if its below 20 mA, wait, then decide to kill or not
      {
        vTaskDelay(pdMS_TO_TICKS(15)); //delay to make sure its not a current spike //change delay later
        if ((cSenseCalc - cPotCalc) > 0.01)
        {
          faultState = OVERCURRENT;
          interrupt = true;
        }
      }
      // Voltage Check
      if (abs(vPotCalc-vSenseCalc) > 0.05) //check current in fly and drive while loops, die if over current
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
      
      vTaskDelay(pdMS_TO_TICKS(15));
    }
  }
}



void readSense()
{
  int vSense = analogRead(VOL_SENSE);
  vSenseCalc = ((vSense / 4095.0) * 20.0); // math might have to change for v and c

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

// Controls the Flyback PWM signals
void flyDrive(float vComp) 
{
  if (prevStateA == 0) // Only allows change to the PWM rate if a new Pot Value was given
  {
    drivePWM = (((vComp - 10) / 10) * 154) + 256; //fix math
    ledcWrite(0, drivePWM);
  }
  float err = vComp - vSenseCalc;
  do
  {
    drivePWM =  ((err / 10) * 500); //fix math
    ledcWrite(0, drivePWM);
    Serial.println(drivePWM);
    vTaskDelay(pdMS_TO_TICKS(2));
    err = vComp - vSenseCalc;
  }
  while(!interrupt && (abs(err) > 0.05) && digitalRead(A_BUTT));
  vTaskDelay(pdMS_TO_TICKS(5));
}

// Controls the Buck Converter PWM signals
void buckDrive() 
{
  if (prevStateA == 0)
  {
    buckPWM = (((vPotCalc - 10) / 10) * 154) + 256;
    ledcWrite(1, buckPWM);
  }
  float err = vPotCalc - vSenseCalc;
  do
  {
    buckPWM = ceil((((err / 10) * 154) + 256));
    ledcWrite(1, buckPWM);
    Serial.println("buck");
    Serial.println(buckPWM);
    vTaskDelay(pdMS_TO_TICKS(2));
    err = vPotCalc - vSenseCalc;
  }
  while(!interrupt && (abs(err) > 0.05) && digitalRead(A_BUTT));
}

void loop() {
  // Empty loop necessary for FreeRTOS
}
