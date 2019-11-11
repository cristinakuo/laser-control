#ifndef __TABLE_HPP_INCLUDED__
#define __TABLE_HPP_INCLUDED__

#include <Arduino.h>
#include "Function.hpp"
#include "archimedean.hpp"
#include "Linear.hpp"
#include "manual_control.hpp"
#include "motor.hpp"

#define ARRAY_MAX_LEN 200 // Maximum length of Time interval array
#define STEPS_PER_REV 200
#define FULL_STEPS_PER_INTERVAL 10
#define MM_PER_REV 1 // TODO: mover a motor

class Table {
public:
    Table();
    ~Table();
    size_t create(archimedean_param_t,step_mode_t);
    size_t create(linear_param_t, step_mode_t);

};

#endif