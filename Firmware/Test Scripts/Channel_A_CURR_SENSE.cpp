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
void readSense();

int lcdColumns = 16;
int lcdRows = 2;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows); 

float vSenseCalc, cSenseCalc; //set to 0?
int stateA, drivePWM, buckPWM;
int prevState = 0;

TaskHandle_t domTaskHandle = NULL;
TaskHandle_t subTaskHandle = NULL;

void setup() 
{
  //Serial.begin (115200);
  //Serial.println("MCU Restarting...");
  lcd.init();                  
  lcd.backlight();

  pinMode(A_BUTT, INPUT);

  ledcSetup(0, 35000, 10);
  ledcAttachPin(BUCK, 1);

  ledcSetup(1, 42000, 10);
  ledcAttachPin(DRIVE, 0);

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
    /* FOR TRANSFORMER AND BUCK
    if (digitalRead(A_BUTT) && !prevState)
    {
      int drive_PWM = 97;
      int buck_PWM = 910;
      for(int i = 0; (i <= 19 && digitalRead(A_BUTT)); i++)
      {
        
        lcd.setCursor(0, 1);
        lcd.print(String("PWM: ") + String(drive_PWM) + String("              "));
        ledcWrite(1, drive_PWM);
        Serial.println(drive_PWM);
        drive_PWM += 10; 
        delay(100); 
      }

      ledcWrite(0, buck_PWM);
      lcd.setCursor(0, 1);
      lcd.print(String("Buck:") + String(buck_PWM) + String("            "));

      delay(1000);
      digitalWrite(HV_LV_OUT, 0);
      
      
      prevState = 1;

    }
    else if (!digitalRead(A_BUTT))
    {
      ledcWrite(1, 0);
    digitalWrite(HV_LV_OUT, 1);
    ledcWrite(0, 0);
    lcd.setCursor(0, 1);
    lcd.print("No PWM                    ");
    prevState = 0;
    }
    vTaskDelay(pdMS_TO_TICKS(100)); */
  

  // FOR JUST BUCK
  if (digitalRead(A_BUTT) && !prevState)
    {
      int buck_PWM = 910;
      ledcWrite(0, buck_PWM);
      lcd.setCursor(0, 1);
      lcd.print(String("Buck:") + String(buck_PWM) + String("            "));

      delay(1000);
      digitalWrite(HV_LV_OUT, 0);
      prevState = 1;
    }
    else if (!digitalRead(A_BUTT))
    {
      digitalWrite(HV_LV_OUT, 1);
      ledcWrite(0, 0);
      lcd.setCursor(0, 1);
      lcd.print("No PWM                    ");
      prevState = 0;
    }
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

void readSense()
{
  int cSense = analogRead(CUR_SENSE);
  lcd.setCursor(0, 0);
  lcd.print(String("Current:") + String(cSense) + String("          "));
}

void loop() {
  // Empty loop necessary for FreeRTOS
}
