// MCU Testing Script - Channel A Enable
// Channel_A_Enable1.cpp
// Switch Mode Power Supply - Senior Projects
// Azra Rangwala, Ilona Lameka, Ridwan Hussain

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

const int A_BUTT = 23; // MCU_A_GPIO23
const int HV_LV_OUT = 18; // MCU_A_GPIO18
const int OUT_ENABLE = 26; // MCU_A_GPIO26
int lcdColumns = 16;
int lcdRows = 2;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows); 
int prevState = 0;

void setup() 
{
  //Serial.begin (115200);
  //Serial.println("MCU Restarting...");
  lcd.init();                  
  lcd.backlight();
  pinMode(A_BUTT, INPUT);
  pinMode(HV_LV_OUT, OUTPUT);
  pinMode(OUT_ENABLE, OUTPUT);
  digitalWrite(HV_LV_OUT, 1);
}

void loop() 
{
  if (digitalRead(A_BUTT) && !prevState)
  {
    digitalWrite(OUT_ENABLE, 1);
    lcd.setCursor(0, 0);
    lcd.print("Output Enabled                   ");
    prevState = 1;
  }
  else if (!digitalRead(A_BUTT))
  {
    digitalWrite(OUT_ENABLE, 0);
    lcd.setCursor(0, 0);
    lcd.print("No Output                    ");
    prevState = 0;
  }
  delay(100);
}
