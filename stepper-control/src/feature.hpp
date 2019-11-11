#ifndef _FEATURE_HPP_INCLUDED_
#define _FEATURE_HPP_INCLUDED_
#include <Arduino.h>
#include <ArduinoSTL.h>
#include "motor.hpp"
#include "Keypad.h"
#include "LiquidCrystal_I2C.h" //Library for LCD
#include "Table.hpp"

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
void barrido_archim(functions_t func);
void barrido_linear(functions_t func);
float calibrate();

void wait_to_start(); // TODO: mover

int mm_to_steps(float,motor);
float steps_to_mm(int,motor);
int get_distance(motor);

void manual_control(motor&);
float receive_number(Keypad keypad, LiquidCrystal_I2C lcd); // DEBUG: move to somewhere else

#endif