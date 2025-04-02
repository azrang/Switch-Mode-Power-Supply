// MCU Testing Script - Channel A Transformers
// Channel_A_Transformer1.cpp
// Switch Mode Power Supply - Senior Projects
// Azra Rangwala, Ilona Lameka, Ridwan Hussain

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

const int A_BUTT = 23; // MCU_A_GPIO23
const int DRIVE = 4; // MCU_A_GPIO4
int lcdColumns = 16;
int lcdRows = 2;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows); 

int PWM = 307; // 30% Duty Cycle
int prevStateA = 0;

void setup() 
{
  lcd.init();                  
  lcd.backlight();
  pinMode(A_BUTT, INPUT);
  ledcSetup(0, 10000, 10); // Play around with frequency and resolution
  ledcAttachPin(DRIVE, 0);
}

void loop() 
{
  if (digitalRead(A_BUTT) && !prevStateA)
  {
    ledcWrite(0, PWM);
    lcd.setCursor(0, 0);
    lcd.print(String("PWM: ") + String(PWM) + String("  "));
    prevStateA = 1;
  }
  else if(!digitalRead(A_BUTT))
  {
    ledcWrite(0, 0);
    lcd.setCursor(0, 0);
    lcd.print("PWM 0      ");
    prevStateA = 0;
  }
}
