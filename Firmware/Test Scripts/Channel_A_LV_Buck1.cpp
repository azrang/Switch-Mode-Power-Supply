// MCU Testing Script - Channel A HV/LV
// Channel_A_LV_Buck1.cpp
// Switch Mode Power Supply - Senior Projects
// Azra Rangwala, Ilona Lameka, Ridwan Hussain

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

const int A_BUTT = 23; // MCU_A_GPIO23
const int HV_LV_OUT = 18; // MCU_A_GPIO18
const int BUCK = 25; // MCU_A_GPIO25
const int DRIVE = 4; // MCU_A_GPIO4
int lcdColumns = 16;
int lcdRows = 2;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows); 
int prevState = 0;



void setup() 
{
  Serial.begin (115200);
  Serial.println("MCU Restarting...");
  lcd.init();                  
  lcd.backlight();
  pinMode(A_BUTT, INPUT);
  pinMode(HV_LV_OUT, OUTPUT);
  ledcSetup(0, 35000, 10); 
  ledcAttachPin(BUCK, 0);

  ledcSetup(1, 42000, 10); 
  ledcAttachPin(DRIVE, 1);

  digitalWrite(HV_LV_OUT, 1);
}

void loop() 
{
  if (digitalRead(A_BUTT) && !prevState)
  {
    int PWM = 102;
    for(int i = 0; (i <= 19 && digitalRead(A_BUTT)); i++)
    {
      
      lcd.setCursor(0, 0);
      lcd.print(String("PWM: ") + String(PWM) + String("              "));
      lcd.setCursor(0, 1);
      lcd.print(String("I: ") + String(i) + String("                   "));
      ledcWrite(1, PWM);
      Serial.println(PWM);
      PWM += 10; //Math to increase by voltage
      delay(100); 
    }
    ledcWrite(1, PWM-5); //297 should be around 12V
    delay(10);
    lcd.setCursor(0, 0);
    lcd.print(String(PWM-5) + String("                   "));
    delay(1000);
    
    //Serial.println(PWM);
    digitalWrite(HV_LV_OUT, 0);
    ledcWrite(0, 512);
    lcd.setCursor(0, 0);
    lcd.print("Buck                          ");
    lcd.setCursor(0, 1);
    lcd.print("                             ");
    
    prevState = 1;
  }
  else if (!digitalRead(A_BUTT))
  {
    ledcWrite(1, 0);
    digitalWrite(HV_LV_OUT, 1);
    ledcWrite(0, 0);
    lcd.setCursor(0, 0);
    lcd.print("No PWM                    ");
    prevState = 0;
  }
  delay(100);
}
