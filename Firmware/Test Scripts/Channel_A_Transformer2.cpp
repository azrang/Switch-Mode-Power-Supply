// MCU Testing Script - Channel A Transformers
// Channel_A_Transformer2
// Switch Mode Power Supply - Senior Projects
// Azra Rangwala, Ilona Lameka, Ridwan Hussain

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

const int A_BUTT = 23; // MCU_A_GPIO23
const int DRIVE = 4; // MCU_A_GPIO4
int lcdColumns = 16;
int lcdRows = 2;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows); 

int prevStateA = 0;

void setup() 
{
  //Serial.begin (115200);
  //Serial.println("MCU Restarting...");
  lcd.init();                  
  lcd.backlight();
  pinMode(A_BUTT, INPUT);
  ledcSetup(0, 50000, 10); // Play around with frequency and resolution
  ledcAttachPin(DRIVE, 0);
}

void loop() 
{
  int PWM = 102; // 10% starting point
  if (digitalRead(A_BUTT) && !prevStateA)
  {
    for(int i = 0; (i < 31 && digitalRead(A_BUTT)); i++)
    {
      PWM += 10; //Math to increase by voltage
      lcd.setCursor(0, 0);
      lcd.print(String("PWM: ") + String(PWM) + String("              "));
      lcd.setCursor(0, 1);
      lcd.print(String("I: ") + String(i) + String("                   "));
      ledcWrite(0, PWM);
      //Serial.println(PWM/1024.00);
      delay(2000);
    }
    prevStateA = 1;
  }
  else if(!digitalRead(A_BUTT))
  {
    ledcWrite(0, 0);
    lcd.setCursor(0, 0);
    lcd.print("PWM 0      ");
    //Serial.println(String(0));
    prevStateA = 0;
  }
  delay(100);
}
