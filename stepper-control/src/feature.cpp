#include "feature.hpp"

void barrido(functions_t func) {
    carrito.isCarrito = true; // DEBUG: esto es harcodeado para imprimir
    target.isCarrito = false; // DEBUG: esto es harcodeado
    
    target.setDirection(chosenDir);
    carrito.setDirection(chosenDir);
    
    // Creacion de la tabla
    table_size = create_table(func, chosenMode);

    // Teclado para empezar
    wait_to_start();

    initial_time = micros();
    carrito.init();
    target.init();
    // Loop
    char stopkey = '0';
    while(stopkey != '*') {
        carrito.move();
        target.move();
        stopkey = customKeypad.getKey();
    }
    Serial.println("Stopped.");
}

// Loop until button is hit
void wait_to_start() {
    char button = '1';
    Serial.println("Press 0 to start:");
    while(button!='0') {
        button = customKeypad.getKey();
    }
    Serial.println("Starting..."); 
}

// Calculates time table in micro seconds
size_t create_table(functions_t f, step_mode_t mode) {
    if (f == ARCHIMEDEAN) {
        long X0_measured = 10; // [mm]
        long X_min = 1;
        float dx = (float)(FULL_STEPS_PER_INTERVAL * MM_PER_REV) / STEPS_PER_REV; // [mm] Distance
        float X0 =  floor(X0_measured/dx) * dx;
        size_t length = round(X0/dx); 
        size_t i_limit;
        // Parameters of the function
        float a = 0.9489;
        float b = 0.6709;
        float F = 10;
        size_t i;
        int microsteps = 1;
        long min_delay = 700;   

        for(i = 1; i <= length; i++) {
            timeTable[i-1] = (PI/a/b/F * (pow(X0,2)-pow(X0-dx*i,2)) - PI/a/b/F * (pow(X0,2)-pow(X0-dx*(i-1),2)))*1000000; // [s]
        }

        // Max velocity control
        i_limit = floor(((X0_measured - X_min) * STEPS_PER_REV) / FULL_STEPS_PER_INTERVAL);

        // TODO: error control
        if (i_limit >= length) {
            Serial.println("Error: array limit out of reach.");
            
        }

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
            min_delay = 100;
            microsteps = 16;
        }
            
        // Write in table
        for (i = i_limit; i < length; i++) {
            timeTable[i] = min_delay*microsteps* FULL_STEPS_PER_INTERVAL;
        }

        return length;
    }
    else if (f == LINEAR) {
        // TODO: complete
        return 1;
    }
    else
        return 1;
}

void manual_control(motor &chosenMotor) {
    //step_mode_t mode = chosenMotor.getStepMode();
    char key_pressed;
    bool want_to_exit = false;
    
    // Initial values
    float distance_mm = 1; // [mm] 
    int N_steps = mm_to_steps(distance_mm, chosenMotor);
    // Show in pantalla
    while(want_to_exit != true) {
        key_pressed = customKeypad.getKey(); // OJO: no es bloqueante y si no recibe nada no se que hace
        if (key_pressed == 'A') {
            // edit distance
            N_steps = get_distance(chosenMotor);
            Serial.print("Distance is: ");
            Serial.println(N_steps);
            // TODO: refresh LCD
        } else if (key_pressed == '1') {
            // go left
            chosenMotor.setDirection(LOW);
            chosenMotor.step(N_steps);
        } else if (key_pressed == '3') {
            // go right
            chosenMotor.setDirection(HIGH);
            chosenMotor.step(N_steps);
        } else if (key_pressed == '*')
            want_to_exit = true;
    }
}


int get_distance(motor the_motor) {
    char char_dist;
    char_dist = customKeypad.waitForKey();

    // TODO: VALIDAAAR
    // TODO: hacer que pueda recibir dos digitos
    return mm_to_steps(char_dist - '0',the_motor);  
}

int mm_to_steps(float x_mm, motor the_motor) {
       return x_mm*the_motor.getStepsPerRevolution()/MM_PER_REV;
}