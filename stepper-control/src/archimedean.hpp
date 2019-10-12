#ifndef _ARCHIMEDEAN_HPP_INCLUDED_
#define _ARCHIMEDEAN_HPP_INCLUDED_

#include <Arduino.h>
#include <EEPROM.h>
#include "Keypad.h"
#include "LiquidCrystal_I2C.h" //Library for LCD
#include "EEPROM_deposit.hpp"

#include "utils.hpp"

// Global variables
extern Keypad customKeypad;
extern LiquidCrystal_I2C lcd;

archimedean_param_t get_archimedean_params();
void show_current_archimedean_params();
void ask_for_new_archimedean_params();

#endif