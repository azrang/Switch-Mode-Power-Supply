// MCU Testing Script - Channel A External Connections Part 1
// Channel_A_Ext_Conn_1.cpp
// Switch Mode Power Supply - Senior Projects
// Azra Rangwala, Ilona Lameka, Ridwan Hussain

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

const int A_BUTT = 23; // MCU_A_GPIO23
int lcdColumns = 16;
int lcdRows = 2;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows); 

void setup() 
{
  lcd.init();                  
  lcd.backlight();
  pinMode(A_BUTT, INPUT);
}

void loop() 
{
  if (!digitalRead(A_BUTT))
  {
    lcd.setCursor(0, 0);
    lcd.print("Button LOW");
    lcd.setCursor(0, 1);
    lcd.print("          ");
  }
  else
  {
    lcd.setCursor(0, 0);
    lcd.print("Button HIGH");
    lcd.setCursor(0, 1);
    lcd.print("          ");
  }
  delay(100);
}
