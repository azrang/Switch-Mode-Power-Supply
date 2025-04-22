// MCU Testing Script - Channel A HV/LV
// Channel_A_LV_Buck1.cpp
// Switch Mode Power Supply - Senior Projects
// Azra Rangwala, Ilona Lameka, Ridwan Hussain

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

const int A_BUTT = 23; // MCU_A_GPIO23
const int HV_LV_OUT = 18; // MCU_A_GPIO18
const int BUCK = 25; // MCU_A_GPIO25
int lcdColumns = 16;
int lcdRows = 2;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows); 
int prevState = 0;

void setup() 
{
  lcd.init();                  
  lcd.backlight();
  pinMode(A_BUTT, INPUT);
  pinMode(HV_LV_OUT, OUTPUT);
  ledcSetup(0, 35000, 10); // Play around with frequency and resolution
  ledcAttachPin(BUCK, 0);
  
}

void loop() 
{
  if (digitalRead(A_BUTT) && !prevState)
  {
    digitalWrite(HV_LV_OUT, 0);
    delay(10);
    ledcWrite(0, 512);
    lcd.setCursor(0, 0);
    lcd.print("LV BUCK               ");
    prevState = 1;
  }
  else if (!digitalRead(A_BUTT))
  {
    digitalWrite(HV_LV_OUT, 1);
    ledcWrite(0, 0);
    lcd.setCursor(0, 0);
    lcd.print("HV               ");
    prevState = 0;
  }
  delay(100);
}
