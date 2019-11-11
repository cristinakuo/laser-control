#ifndef _EEPROM_DEPOSIT_HPP_INCLUDED_
#define _EEPROM_DEPOSIT_HPP_INCLUDED_

#include <Arduino.h>

typedef struct params {
    float fine;
    float coarse;
} step_param_t;

// TODO: Add X0 for calibration
typedef struct params_arch {
    float a;
    float b;
    float X_min;
    float X0_measured;
} archimedean_param_t;

typedef struct linear_params {
    float linear_speed; // [mm/s]
    float angular_speed; // [rad/s]
    float X0_measured;
} linear_param_t;

// TODO: definir direccion para guardar parametros de funcion archimedean
const int test_global_variable = 1;
const int STEP_PARAM_ADDR = 0;
const int ARCHIM_PARAM_ADDR = STEP_PARAM_ADDR + sizeof(step_param_t);
const int LINEAR_PARAM_ADDR = ARCHIM_PARAM_ADDR + sizeof(archimedean_param_t);

#endif