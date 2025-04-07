// MCU Testing Script - Channel A Temperature Sensor
// Channel_A_Temp_Sense.cpp
// Switch Mode Power Supply - Senior Projects
// Azra Rangwala, Ilona Lameka, Ridwan Hussain

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

const int TEMP = 32; // MCU_A_GPIO32
int lcdColumns = 16;
int lcdRows = 2;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows); 

void setup() 
{
  lcd.init();                  
  lcd.backlight();
  pinMode(TEMP, INPUT);
}

void loop() 
{
  if (digitalRead(TEMP))
  {
    lcd.setCursor(0, 0);
    lcd.print("Low Temp     ");
    lcd.setCursor(0, 1);
    lcd.print("          ");
  }
  else
  {
    lcd.setCursor(0, 0);
    lcd.print("Over Temp     ");
    lcd.setCursor(0, 1);
    lcd.print("          ");
  }
  delay(100);
}
