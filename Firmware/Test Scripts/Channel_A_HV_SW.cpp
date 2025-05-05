// MCU Testing Script - Channel A HV/LV
// Channel_A_HV_SW.cpp
// Switch Mode Power Supply - Senior Projects
// Azra Rangwala, Ilona Lameka, Ridwan Hussain

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

const int A_BUTT = 23; // MCU_A_GPIO23
const int HV_LV_OUT = 18; // MCU_A_GPIO18
int lcdColumns = 16;
int lcdRows = 2;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows); 

void setup() 
{
  lcd.init();                  
  lcd.backlight();
  pinMode(A_BUTT, INPUT);
  pinMode(HV_LV_OUT, OUTPUT);
  digitalWrite(HV_LV_OUT, 1);
}

void loop() 
{
  if (digitalRead(A_BUTT))
  {
    digitalWrite(HV_LV_OUT, 0);
    lcd.print("LOW HV_LV");
    delay(100);
  }
}
