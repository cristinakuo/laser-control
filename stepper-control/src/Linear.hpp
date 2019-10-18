#ifndef _LINEAR_HPP_INCLUDED_
#define _LINEAR_HPP_INCLUDED_

#include <Arduino.h>
#include <EEPROM.h>
#include "Keypad.h"
#include "LiquidCrystal_I2C.h" //Library for LCD
#include "EEPROM_deposit.hpp"

#include "utils.hpp"

// Global variables
extern Keypad customKeypad;
extern LiquidCrystal_I2C lcd;

linear_param_t get_linear_params();
void show_current_linear_params();
void ask_for_new_linear_params();

#endif