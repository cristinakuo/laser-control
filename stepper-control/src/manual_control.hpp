#include <Arduino.h>
#include <EEPROM.h>
#include "Keypad.h"
#include "LiquidCrystal_I2C.h" //Library for LCD
#include "utils.hpp"

#define STEP_PARAM_ADDR 0
// Global variables
extern Keypad customKeypad;
extern LiquidCrystal_I2C lcd;

typedef struct params {
    float fine;
    float coarse;
} step_param_t;

step_param_t get_step_param();
void show_current_step_param();
void ask_for_new_step_param();