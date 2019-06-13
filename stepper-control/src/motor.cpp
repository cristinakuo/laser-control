#include "motor.hpp"

// TODO: hacer que solo MachineElement pueda acceder a este constructor.


motor::motor(const byte step_pin, const byte direction_pin, 
    const byte MS1_pin, const byte MS2_pin, const byte MS3_pin, step_mode_t mode,
    const int stepsPerRev, const int fullStepsPerInterval) {

    _stepPin = step_pin;
    _directionPin = direction_pin;
    _MS1Pin = MS1_pin;
	_MS2Pin = MS2_pin;
	_MS3Pin = MS3_pin; 
    _stepsPerRevolution = stepsPerRev; 
    pinMode(_directionPin, OUTPUT);
    digitalWrite(_directionPin, HIGH); // DEFAULT
    pinMode(_stepPin, OUTPUT);
    pinMode(_MS1Pin, OUTPUT);
    pinMode(_MS2Pin, OUTPUT);
    pinMode(_MS3Pin, OUTPUT);

    // Default value
    setStepMode(mode);
    _fullStepsPerInterval = fullStepsPerInterval;
    i = 0;
    i_sign = 1;
    _stepsPerInterval = _fullStepsPerInterval * _microsteps; // OJO: declarar despues de settear el step mode
    Serial.print("Steps per interval is: ");
    Serial.println(_stepsPerInterval);
    _takenStepsInInterval = 0;
    _timeLastInterval = 0;
}


void motor::setStepMode(const step_mode_t mode) {
    if (mode == FULL) {
        digitalWrite(_MS1Pin, LOW);
        digitalWrite(_MS2Pin, LOW);
        digitalWrite(_MS3Pin, LOW);
        _stepsPerRevolution = 200;
        _microsteps = 1;
    }
    else if (mode == HALF) {
        digitalWrite(_MS1Pin, HIGH);
        digitalWrite(_MS2Pin, LOW);
        digitalWrite(_MS3Pin, LOW);
        _stepsPerRevolution = 400;
        _microsteps = 2;
    }
    else if (mode == QUARTER) {
        digitalWrite(_MS1Pin, LOW);
        digitalWrite(_MS2Pin, HIGH);
        digitalWrite(_MS3Pin, LOW);
        _stepsPerRevolution = 800;
        _microsteps = 4;
    }
    else if (mode == EIGHTH) {
        digitalWrite(_MS1Pin, HIGH);
        digitalWrite(_MS2Pin, HIGH);
        digitalWrite(_MS3Pin, LOW);
        _stepsPerRevolution = 1600;
        _microsteps = 8;   
    }
    else if (mode == SIXTEENTH) {
        digitalWrite(_MS1Pin, HIGH);
        digitalWrite(_MS2Pin, HIGH);
        digitalWrite(_MS3Pin, HIGH);
        _stepsPerRevolution = 3200;
        _microsteps = 16;  
    }
}

motor::~motor() {
}

void motor::step() const {
    digitalWrite(_stepPin, HIGH);
    delayMicroseconds(5); // Probably unnecesary because delay in digital Write is pretty long
    digitalWrite(_stepPin, LOW);
}

void motor::setDirection(const byte direction) {
    digitalWrite(_directionPin, direction);
}

void motor::init() {
    _stepDelay = floor((float)timeTable[i] / (float) _stepsPerInterval);
    Serial.println(micros()-initial_time); // DEBUG  
    _timeToMove = _stepDelay;
    _takenStepsInInterval = 0;
    _nRounds = 0; // DEBUG
}

void motor::move() {
    
    if (micros()-initial_time-_timeLastInterval >= _timeToMove) {
        step();
        _takenStepsInInterval++;
        _timeToMove += _stepDelay; 
    }

    if (_takenStepsInInterval >= _stepsPerInterval) {
        _timeLastInterval += timeTable[i];
        i = i + i_sign;

        // Check i
        if (i < 0) {
            if (digitalRead(_directionPin) == HIGH)
                digitalWrite(_directionPin,LOW);
            else
                digitalWrite(_directionPin,HIGH);

            i_sign = -i_sign;
            i = i + i_sign;
            _nRounds++;

        
        } 
        // If index exceeds max index value, then index direction has to be changed
        else if (i >= (int)table_size) {
            i_sign = -i_sign;
            i = i + i_sign;
            _nRounds++;
        }

        if (_nRounds == 2)
            must_stop = true;
        // Reasignar parametros
        _stepDelay = floor((float)timeTable[i] / (float) _stepsPerInterval);
        _takenStepsInInterval = 0;
        _timeToMove = _stepDelay;
        Serial.println(micros()-initial_time); // DEBUG  
        //Serial.print(" ");
        //Serial.println(_stepDelay);
    } 
}