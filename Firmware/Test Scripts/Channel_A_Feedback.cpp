// MCU Testing Script - Channel A Feedback
// Channel_A_Feedback.cpp
// Switch Mode Power Supply - Senior Projects
// Azra Rangwala, Ilona Lameka, Ridwan Hussain

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

const int A_BUTT = 23; // MCU_A_GPIO23
const int HV_LV_OUT = 18; // MCU_A_GPIO18
const int OUT_ENABLE = 26; // MCU_A_GPIO26
const int VOL_SENSE = 35; // MCU_A_ADC1_C7

void dominos (void*); // Core 0
void subway (void*);  // Core 1
void readSense();

int lcdColumns = 16;
int lcdRows = 2;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows); 

float vSenseCalc;//set to 0?
int stateA;
int prevState = 0;

TaskHandle_t domTaskHandle = NULL;
TaskHandle_t subTaskHandle = NULL;

void setup() 
{
  lcd.init();                  
  lcd.backlight();

  pinMode(A_BUTT, INPUT);
  pinMode(HV_LV_OUT, OUTPUT);
  pinMode(OUT_ENABLE, OUTPUT);
  digitalWrite(HV_LV_OUT, 1);

  xTaskCreatePinnedToCore(dominos, "Dominatrix", 2048, NULL, 2, &domTaskHandle, 0);
  xTaskCreatePinnedToCore(subway, "Sub", 2048, NULL, 2, &subTaskHandle, 1);
}

void dominos (void* pvParameters) {
  for (;;) {
    readSense();
    vTaskDelay(pdMS_TO_TICKS(50)); // Feeding the dog
  }
}

void subway(void *pvParameters)
{
  for (;;)
  {
    if (digitalRead(A_BUTT) && !prevState)
      {
        digitalWrite(OUT_ENABLE, 1);
        prevState = 1;
      }
      else if (!digitalRead(A_BUTT))
      {
        digitalWrite(OUT_ENABLE, 0);
        prevState = 0;
      }
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

void readSense()
{
  int vTotal = 0;
  for (int i = 0; i < 300; i++) // Getting muliple samples due to ADC fluctuations
  {
    vTotal += analogRead(VOL_SENSE);
    vTaskDelay(pdMS_TO_TICKS(1));
  }
  int vSense = vTotal / 300; 
  lcd.setCursor(0, 0);
  lcd.print(String("Voltage:") + String(vSense) + String("          "));
}

void loop() {
  // Empty loop necessary for FreeRTOS
}
