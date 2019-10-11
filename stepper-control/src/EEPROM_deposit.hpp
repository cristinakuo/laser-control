#ifndef _EEPROM_DEPOSIT_HPP_INCLUDED_
#define _EEPROM_DEPOSIT_HPP_INCLUDED_

#include <Arduino.h>

typedef struct params {
    float fine;
    float coarse;
} step_param_t;

typedef struct params_arch {
    float a;
    float b;
    float X_min;
} params_arch_t;

// TODO: definir direccion para guardar parametros de funcion archimedean
const int test_global_variable = 1;
const int STEP_PARAM_ADDR = 0;
const int ARCHIM_PARAM_ADDR = STEP_PARAM_ADDR + sizeof(step_param_t);

#endif