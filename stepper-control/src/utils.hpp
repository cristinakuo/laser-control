#include <Arduino.h>
#include <ArduinoSTL.h>
#include "Keypad.h"
#include "LiquidCrystal_I2C.h" //Library for LCD

extern Keypad customKeypad;
extern LiquidCrystal_I2C lcd;

float receive_number();