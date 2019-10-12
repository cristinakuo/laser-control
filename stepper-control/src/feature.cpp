#include "feature.hpp"

// Archimedean 
void barrido(functions_t func) {
    ask_for_new_archimedean_params(); // TODO: make object oriented
    archimedean_param_t parameters;
    // TODO: add X_min
    EEPROM.get(ARCHIM_PARAM_ADDR, parameters);

    lcd.clear();
    lcd.setCursor(0,0); 
    lcd.print("UsingParam:"); 
    lcd.setCursor(0,1); 
    lcd.print(parameters.a);
    lcd.setCursor(7,1);     
    lcd.print(parameters.b);
    delay(2000);
    
    carrito.log = true; // DEBUG: esto es harcodeado para imprimir
    target.log = false; // DEBUG: esto es harcodeado
    
    // Calibracion (otro feature)
    // ask for new params: a, b, and X0
    target.setDirection(chosenDir);
    carrito.setDirection(chosenDir);
    
    // Creacion de la tabla
    table_size = create_table(parameters, chosenMode);

    // Teclado para empezar
    wait_to_start();

    //TODO: que diga si es arq o lineal
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("   AutoMode");
    lcd.setCursor(0,1); 
    lcd.print("Press * to stop"); 

    initial_time = micros();
    carrito.init();
    target.init();
    
    // Loop
    char stopkey = '0'; // TODO: Pongo en cualquiera
    while (stopkey != '*' && must_stop == false) {
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

    lcd.clear();
    lcd.setCursor(0,0); 
    lcd.print("Press 0 to start"); 

    while(button!='0') {
        button = customKeypad.getKey();
    }
    Serial.println("Starting..."); 
}

// Returns time in micro seconds, timing for full steps
size_t create_table(archimedean_param_t parameters, step_mode_t mode) {
 
    long X0_measured = 10; // [mm] TODO: this should be a parameter (obtained through calibration)
    // Saving just for reference
    //long X_min = 1;
    //float a = 0.9489; 
    //float b = 0.6709;  

    float dx = (float)(FULL_STEPS_PER_INTERVAL * MM_PER_REV) / STEPS_PER_REV; // [mm] Distance
    Archimedean function = Archimedean(X0_measured, parameters.a, parameters.b, parameters.X_min, dx);
    size_t length = round( function.getX0() / dx);
    size_t i;

    for(i = 1; i <= length; i++) {
        timeTable[i-1] = function.eval(i); // [micro seconds]
    }

    size_t i_limit;   
    int microsteps = 1;
    long min_delay = 700; 
    // Max velocity control
    i_limit = floor(((X0_measured - parameters.X_min) * STEPS_PER_REV) / FULL_STEPS_PER_INTERVAL);

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

void manual_control(motor &chosenMotor) {
    int STEP_PARAM_ADDR = 0; // TODO: fix this
    //step_mode_t mode = chosenMotor.getStepMode();
    char key_pressed;
    bool want_to_exit = false;
    
    // Initial values
    //float distance_mm = 1; // [mm] TODO: llevar 
    ask_for_new_step_param();
    step_param_t parameters;
    EEPROM.get(STEP_PARAM_ADDR, parameters);

    // TEST
    Serial.println("*******TEST********");
    if (parameters.fine == 2.60 && parameters.coarse == 5.10) {
        Serial.println("TEST SUCCESS");
    } else {
        Serial.println("TEST FAILURE");
    }
    
    lcd.clear();
    lcd.setCursor(0,0); 
    lcd.print("UsingParam:"); 
    lcd.setCursor(0,1); 
    lcd.print(parameters.fine);
    lcd.setCursor(7,1);     
    lcd.print(parameters.coarse);
    delay(2000);
    int N_steps_fine = mm_to_steps(parameters.fine, chosenMotor);
    int N_steps_coarse = mm_to_steps(parameters.coarse, chosenMotor);

    lcd.clear();
    lcd.setCursor(0,0); 
    lcd.print("1) o 4) IZQ"); 
    lcd.setCursor(0,1); 
    lcd.print("3) o 6) DER");

    // Show in pantalla
    while(want_to_exit != true) {
        key_pressed = customKeypad.waitForKey(); 
        if (key_pressed == '1') {
            // go left
            chosenMotor.setDirection(LOW);
            chosenMotor.step(N_steps_coarse);
        } else if (key_pressed == '3') {
            // go right
            chosenMotor.setDirection(HIGH);
            chosenMotor.step(N_steps_coarse);
        } else if (key_pressed == '4') {
            // go right
            chosenMotor.setDirection(LOW);
            chosenMotor.step(N_steps_fine);
        } else if (key_pressed == '6') {
            // go right
            chosenMotor.setDirection(HIGH);
            chosenMotor.step(N_steps_fine);
        } else if (key_pressed == '#')
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

