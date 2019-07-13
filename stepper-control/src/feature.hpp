#ifndef _FEATURE_HPP_INCLUDED_
#define _FEATURE_HPP_INCLUDED_

#include "Arduino.h"
#include "motor.hpp"
#include "Keypad.h"

#define ARRAY_MAX_LEN 200 // Maximum length of Time interval array
#define STEPS_PER_REV 200
#define FULL_STEPS_PER_INTERVAL 10

typedef enum {ARCHIMEDEAN, LINEAR} functions_t;

extern Keypad customKeypad;
extern motor target;
extern motor carrito;
extern step_mode_t chosenMode;
extern byte chosenDir;

void barrido(functions_t func);

void wait_to_start(); // TODO: mover

// Functions
size_t create_table(functions_t, step_mode_t);

#endif