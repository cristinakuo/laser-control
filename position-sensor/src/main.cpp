#include <Arduino.h>
#define BAUDRATE 9600
#define SENSOR_PIN 7

// Resets after 70 minutes
void ISR_printTime();
void wait_to_start();

long unsigned initial_time;
byte current_state;
byte prev_state;

void setup() {
    Serial.begin(BAUDRATE);
    wait_to_start();
    initial_time = micros();
    Serial.println(0);
    prev_state = digitalRead(SENSOR_PIN);
    //attachInterrupt(digitalPinToInterrupt(SENSOR_PIN),ISR_printTime,CHANGE);
}


void loop() {
    // On Change
    current_state = digitalRead(SENSOR_PIN);
    if(current_state != prev_state) {
        Serial.println(micros()-initial_time);
        prev_state = current_state;
    }
}

void ISR_printTime() {
    Serial.println(micros()-initial_time);
}

// Loop until first slot detected
void wait_to_start() {
    byte before_state = digitalRead(SENSOR_PIN);
    while(digitalRead(SENSOR_PIN) == before_state) { };
}