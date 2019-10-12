#ifndef _FEATURE_HPP_INCLUDED_
#define _FEATURE_HPP_INCLUDED_
#include <Arduino.h>
#include <ArduinoSTL.h>
#include "motor.hpp"
#include "Keypad.h"
#include "LiquidCrystal_I2C.h" //Library for LCD
#include "manual_control.hpp"
#include "archimedean.hpp"
#include "Function.hpp"

#define ARRAY_MAX_LEN 200 // Maximum length of Time interval array
#define STEPS_PER_REV 200
#define FULL_STEPS_PER_INTERVAL 10
#define MM_PER_REV 1 // TODO: mover a motor

typedef enum {ARCHIMEDEAN, LINEAR} functions_t;

extern Keypad customKeypad;
extern motor target;
extern motor carrito;
extern step_mode_t chosenMode;
extern byte chosenDir;
extern long unsigned initial_time;
extern LiquidCrystal_I2C lcd;
extern volatile bool must_stop;

// Main features
void barrido(functions_t func);

void wait_to_start(); // TODO: mover

// Functions
size_t create_table(params_arch_t, step_mode_t);
int mm_to_steps(float,motor);
int get_distance(motor);

void manual_control(motor&);
float receive_number(Keypad keypad, LiquidCrystal_I2C lcd); // DEBUG: move to somewhere else

#endif