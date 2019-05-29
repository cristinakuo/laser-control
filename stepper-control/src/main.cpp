#include <Arduino.h>
#include <math.h>

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
#define FULL_STEPS_PER_INTERVAL 10

typedef enum {ARCHIMEDEAN, LINEAR} functions_t;
typedef enum {FULL, HALF, QUARTER, EIGHTH, SIXTEENTH} step_mode_t;

// Interrupt Services
void button_ISR();

// Functions
size_t create_table(functions_t);
void init_ports();
void step();
void wait_to_start();
void config_step_mode(step_mode_t);

// TODO: get rid of global variables
const int steps_per_rev = 200;
const int mm_per_rev = 1;

int microsteps = 1; 


// Global variables accesible from ISR
volatile bool must_stop = false;

long unsigned timeTable[ARRAY_MAX_LEN]; 
long unsigned initial_time; // [us]

void setup() {
    // Init serial communication
    Serial.begin(BAUDRATE);
    // Set input and output ports
    init_ports();
    // Configurate stepper
    config_step_mode(QUARTER); // Updates microsteps
    byte stepperDirection = LOW; 
    digitalWrite(DIRECTION_PIN, stepperDirection);

    size_t steps_per_interval = FULL_STEPS_PER_INTERVAL * microsteps;
    size_t table_size = create_table(ARCHIMEDEAN);
    
    // Auxiliar variables
    
    long unsigned step_delay; // Delay between steps[us]
    long unsigned time_to_move; // [us]
    size_t taken_steps; // Counts steps taken within an interval
    int i = 0; // Index for time array
    int i_sign = 1; // It's either 1 or -1
    long unsigned t_last_interval = 0;
    // Wait for button to be hit
    //   wait_to_start();

    // Increase counter when speed sensor pin goes High
  	//attachInterrupt(digitalPinToInterrupt(SENSOR_PIN), printTime_ISR,CHANGE);
    // Stop program if button is hit 
    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), button_ISR, RISING); 

    initial_time = micros();
    while (must_stop == false) {

        //// DEBUG LINEAR
        //digitalWrite(DIRECTION_PIN, HIGH);
        //for (i=0;i<table_size*steps_per_interval;i++) {
        //    step();
        //    delayMicroseconds(30);    
        //}
        //must_stop = true;
        //continue; // DEBUG

        step_delay = floor((float)timeTable[i] / (float) steps_per_interval);
        Serial.println(micros()-initial_time); // DEBUG  
        time_to_move = step_delay;
        taken_steps = 0;
        
        while (taken_steps < steps_per_interval) {                
            
            if ( micros()-initial_time-t_last_interval >= time_to_move ) {                
                step();
                taken_steps++;
                time_to_move += step_delay;
                //Serial.println("paso");
            }                
        }
        t_last_interval += timeTable[i]; // TODO: check overflow
  
        i = i + i_sign;

        // If index is zero, then direction of motor has to be changed
        // and also direction of index.

        if (i < 0) {
            stepperDirection = (stepperDirection == HIGH) ? LOW : HIGH;
            digitalWrite(DIRECTION_PIN, stepperDirection);
            i_sign = -i_sign;
            i = i + i_sign;
        } 
        // If index exceeds max index value, then index direction has to be changed
        else if (i >= (int)table_size) {
            i_sign = -i_sign;
            i = i + i_sign;
            must_stop = true; // DEBUG
        } 
    }

    Serial.println("Stopped.");
}   

void loop() {
}

// Calculates time table in micro seconds
size_t create_table(functions_t f) {
    if (f == ARCHIMEDEAN) {
        long X0_measured = 10; // [mm]
        float dx = (float)(FULL_STEPS_PER_INTERVAL * mm_per_rev) / steps_per_rev; // [mm] Distance
        float X0 =  floor(X0_measured/dx) * dx;
        size_t length = round(X0/dx); 
    
        // Parameters of the function
        float a = 0.9489;
        float b = 0.6709;
        float F = 10;
        size_t i;   

        for(i = 1; i <= length; i++) {
            timeTable[i-1] = (PI/a/b/F * (pow(X0,2)-pow(X0-dx*i,2)) - PI/a/b/F * (pow(X0,2)-pow(X0-dx*(i-1),2)))*1000000; // [s]
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