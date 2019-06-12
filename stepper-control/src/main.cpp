#include <Arduino.h>
#include <math.h>
#include "motor.hpp"

#define BAUDRATE 9600 // Serial communication

// Stepper pins
#define DIRECTION_PIN 8
#define STEP_PIN 9
#define LENSE_MS1_PIN 7
#define LENSE_MS2_PIN 6
#define LENSE_MS3_PIN 5

// Interrupt pins
#define BUTTON_PIN 3 // Start/Stop button

#define ARRAY_MAX_LEN 200 // Maximum length of Time interval array
#define STEPS_PER_REV 200
#define FULL_STEPS_PER_INTERVAL 10

typedef enum {ARCHIMEDEAN, LINEAR} functions_t;

// Interrupt Services
void button_ISR();

// Functions
size_t create_table(functions_t, step_mode_t);
void init_ports();
void step();
void wait_to_start();
void config_step_mode(step_mode_t);
void check_motor(); // TODO: Probably this will have to become part of a class
void setup2(); // TEMP
// TODO: get rid of global variables

const int mm_per_rev = 1;

int microsteps = 1; 


// Global variables accesible from ISR
volatile bool must_stop = false;
long unsigned timeTable[ARRAY_MAX_LEN];
size_t table_size; // TODO: ver que onda cuando haya otra tabla 
long unsigned initial_time; // [us]

void setup() {
	// Init serial communication
    Serial.begin(BAUDRATE);
    // Set input and output ports
    init_ports();
    // Configurate stepper
    //config_step_mode(EIGHTH); // Updates microsteps. TODO: This might be part of a class
    motor carrito(STEP_PIN, DIRECTION_PIN, 
			LENSE_MS1_PIN, LENSE_MS2_PIN, LENSE_MS3_PIN,
            EIGHTH,
            STEPS_PER_REV, FULL_STEPS_PER_INTERVAL);
    carrito.setDirection(LOW);

    table_size = create_table(ARCHIMEDEAN, FULL);

    // Stop program if button is hit 
    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), button_ISR, RISING);
    
    initial_time = micros();
    carrito.init();
    // Loop
    while(must_stop == false) {
        carrito.move();
    }

    Serial.println("Stopped.");
}

void loop() {
}

// Calculates time table in micro seconds
size_t create_table(functions_t f, step_mode_t mode) {
    if (f == ARCHIMEDEAN) {
        long X0_measured = 10; // [mm]
        long X_min = 9.85; // Ingresado a mano
        float dx = (float)(FULL_STEPS_PER_INTERVAL * mm_per_rev) / STEPS_PER_REV; // [mm] Distance
        float X0 =  floor(X0_measured/dx) * dx;
        size_t length = round(X0/dx); 
        size_t i_limit;
        // Parameters of the function
        float a = 0.9489;
        float b = 0.6709;
        float F = 10;
        size_t i;
        int microsteps;

        long min_delay;   

        for(i = 1; i <= length; i++) {
            timeTable[i-1] = (PI/a/b/F * (pow(X0,2)-pow(X0-dx*i,2)) - PI/a/b/F * (pow(X0,2)-pow(X0-dx*(i-1),2)))*1000000; // [s]
        }

        // Max velocity control
        i_limit = floor((X_min * STEPS_PER_REV) / FULL_STEPS_PER_INTERVAL);

        // TODO: error control
        if (i_limit >= length) {
            Serial.println("Error: array limit out of reach.");
        }
        Serial.print("i_limit is: ");
        Serial.println(i_limit);
        // HARDCODEO

        if (mode == FULL) {
            min_delay = 700; // Micros
            microsteps = 1;
        }
        else if (mode == HALF) {
            min_delay = 230;
            microsteps = 2;
        }
        else if (mode == QUARTER) {
            min_delay = 110;
            microsteps = 4;
        }
        else if (mode == EIGHTH) {
            min_delay = 60;
            microsteps = 8;
        }
        else if (mode == SIXTEENTH) {
            min_delay = 20;
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

void init_ports() {
    // Stepper pins
    pinMode(DIRECTION_PIN, OUTPUT);
    pinMode(STEP_PIN, OUTPUT);
    pinMode(LENSE_MS1_PIN, OUTPUT);
    pinMode(LENSE_MS2_PIN, OUTPUT);
    pinMode(LENSE_MS3_PIN, OUTPUT);

    // Start/Stop button
    pinMode(BUTTON_PIN, INPUT);

    // Speed sensor
    //pinMode(SENSOR_PIN, INPUT);
}

void step() {
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(5); // Probably unnecesary because delay in digital Write is pretty long
    digitalWrite(STEP_PIN, LOW);
}

// TODO: Fix
// Loop until button is hit
void wait_to_start() {
    while (digitalRead(BUTTON_PIN) == LOW) {};
    delay(1000); // Wait for stability in electrical signal
}

void button_ISR() {
    must_stop = true;
}

void config_step_mode(step_mode_t mode) {
    byte pin1_state;
    byte pin2_state;
    byte pin3_state;
    switch (mode) {
        case FULL:
            pin1_state = LOW;
            pin2_state = LOW;
            pin3_state = LOW;
            microsteps = 1; // TODO: todavia no entra en efecto
            break;
        case HALF:
            pin1_state = HIGH;
            pin2_state = LOW;
            pin3_state = LOW;
            microsteps = 2;
            break;
        case QUARTER:
            pin1_state = LOW;
            pin2_state = HIGH;
            pin3_state = LOW;
            microsteps = 4;
            break;
        case EIGHTH:
            pin1_state = HIGH;
            pin2_state = HIGH;
            pin3_state = LOW;
            microsteps = 8;
            break;
        case SIXTEENTH:
            pin1_state = HIGH;
            pin2_state = HIGH;
            pin3_state = HIGH;
            microsteps = 16;
            break;
    }
    digitalWrite(LENSE_MS1_PIN, pin1_state);
    digitalWrite(LENSE_MS2_PIN, pin2_state);
    digitalWrite(LENSE_MS3_PIN, pin3_state);      
}
