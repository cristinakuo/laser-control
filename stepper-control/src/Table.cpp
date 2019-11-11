#include "Table.hpp"

Table::Table() {
}

Table::~Table() {
}

size_t Table::create(archimedean_param_t parameters, step_mode_t mode) {
    float dx = (float)(FULL_STEPS_PER_INTERVAL * MM_PER_REV) / STEPS_PER_REV; // [mm] Distance
    Archimedean function = Archimedean(parameters.X0_measured, parameters.a, parameters.b, parameters.X_min, dx);
    size_t length = round( function.getX0() / dx);
    size_t i;

    for(i = 1; i <= length; i++) {
        timeTable[i-1] = function.eval(i); // [micro seconds]
    }

    size_t i_limit;   
    int microsteps = 1;
    long min_delay = 700; 
    // Max velocity control
    i_limit = floor(((parameters.X0_measured - parameters.X_min) * STEPS_PER_REV) / FULL_STEPS_PER_INTERVAL);

    // TODO: error control
    if (i_limit >= length) {
        Serial.println("Error: array limit out of reach.");  
    }

    // TODO: modularize
    if (mode == FULL) {
        min_delay = 700; // Micros
        microsteps = 1;
    }
    else if (mode == HALF) {
        min_delay = 700;
        microsteps = 2;
    }
    else if (mode == QUARTER) {
        min_delay = 700;
        microsteps = 4;
    }
    else if (mode == EIGHTH) {
        min_delay = 150;
        microsteps = 8;
    }
    else if (mode == SIXTEENTH) {
        //min_delay = 100; // Esto esta repetido
        min_delay = 200;
        microsteps = 16;
    }
        
    // Write in table
    for (i = i_limit; i < length; i++) {
        timeTable[i] = min_delay*microsteps* FULL_STEPS_PER_INTERVAL;
    }
    return length;
}

size_t Table::create(linear_param_t parameters, step_mode_t mode) {
    long X0_measured = 10; // [mm] TODO: this should be a parameter (obtained through calibration)
    float dx = (float)(FULL_STEPS_PER_INTERVAL * MM_PER_REV) / STEPS_PER_REV; // [mm] Distance
    Linear function = Linear(X0_measured, parameters.linear_speed,parameters.angular_speed , dx);
    size_t length = ARRAY_MAX_LEN;
    size_t i;

    for(i = 1; i <= length; i++) {
        timeTable[i-1] = function.eval(i); // [micro seconds]
    }
    return length;
}

