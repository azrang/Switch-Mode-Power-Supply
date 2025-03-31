// MCU B
// Switch Mode Power Supply - Senior Projects
// Azra Rangwala, Ilona Lameka, Ridwan Hussain

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

// ADC Pins
const int CUR_SENSE = 36; // MCU_B_ADC1_C0
const int B_VOL_SENSE  = 39; // MCU_B_ADC1_C3
const int V_KNOB = 34; // MCU_B_ADC1_C6
const int C_KNOB = 35; // MCU_B_ADC1_C7
const int USB_VOL_SENSE = 32; // MCU_B_ADC1_C4

// GPIO Input Pins
const int TEMP = 33; // MCU_B_GPIO33
const int B_FUSE = 12; // MCU_B_GPIO12
const int B_BUTT = 23; // MCU_B_GPIO23
const int C_BUTT = 17; // MCU_B_GPIO17
const int FLT_ALERT = 16; // MCU_B_GPIO16


// GPIO Output Pins
const int BUCK = 25; // MCU_B_GPIO25
const int OUT_ENABLE = 26; // MCU_B_GPIO26
const int USB_ENABLE = 13; // MCU_B_GPIO13
const int B_DRIVE = 19; // MCU_B_GPIO19
const int HV_LV_OUT = 18; // MCU_B_GPIO18
const int USB_DRIVE = 4; // MCU_B_GPIO4
