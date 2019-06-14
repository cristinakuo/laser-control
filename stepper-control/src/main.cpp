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
void wait_to_start();

const int mm_per_rev = 1;

// Global variables 
volatile bool must_stop = false;
long unsigned timeTable[ARRAY_MAX_LEN];
size_t table_size; // TODO: ver que onda cuando haya otra tabla 
long unsigned initial_time; // [us]

void setup() {
	// Init serial communication
    Serial.begin(BAUDRATE);

    // Stop program if button is hit 
    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), button_ISR, RISING);
    
    // Set input and output ports
    pinMode(BUTTON_PIN, INPUT);
    // Configurate stepper
    step_mode_t chosenMode = EIGHTH;
    byte chosenDir = LOW;
    
    motor carrito(STEP_PIN, DIRECTION_PIN, 
			LENSE_MS1_PIN, LENSE_MS2_PIN, LENSE_MS3_PIN,
            chosenMode,
            STEPS_PER_REV, FULL_STEPS_PER_INTERVAL);
    carrito.setDirection(chosenDir);

    table_size = create_table(ARCHIMEDEAN, chosenMode);

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
        long X_min = 2;
        float dx = (float)(FULL_STEPS_PER_INTERVAL * mm_per_rev) / STEPS_PER_REV; // [mm] Distance
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
        Serial.print("i_limit is: ");
        Serial.println(i_limit);
        // HARDCODEO

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
            min_delay = 700;
            microsteps = 16;
        }
            
        // Write in table
        for (i = i_limit; i < length; i++) {
            timeTable[i] = min_delay*microsteps* FULL_STEPS_PER_INTERVAL;
        }
        //DEBUG
        for (i = 0; i < length; i++) {
            Serial.println(timeTable[i]);
        }
        Serial.println("end time table");
        return length;
    }
    else if (f == LINEAR) {
        // TODO: complete
        return 1;
    }
    else
        return 1;
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
