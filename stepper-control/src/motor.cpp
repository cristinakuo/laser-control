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
    _i = 0;
    i_sign = 1;
    _stepsPerInterval = _fullStepsPerInterval * _microsteps; // OJO: declarar despues de settear el step mode
    _takenStepsInInterval = 0;
    _timeLastInterval = 0;
}


void motor::setStepMode(const step_mode_t mode) {
    _stepMode = mode;
    if (mode == FULL) {
        digitalWrite(_MS1Pin, LOW);
        digitalWrite(_MS2Pin, LOW);
        digitalWrite(_MS3Pin, LOW);
        _stepsPerRevolution = 200;
        _microsteps = 1;
        _minDelay = 700;
    }
    else if (mode == HALF) {
        digitalWrite(_MS1Pin, HIGH);
        digitalWrite(_MS2Pin, LOW);
        digitalWrite(_MS3Pin, LOW);
        _stepsPerRevolution = 400;
        _microsteps = 2;
        _minDelay = 700;
    }
    else if (mode == QUARTER) {
        digitalWrite(_MS1Pin, LOW);
        digitalWrite(_MS2Pin, HIGH);
        digitalWrite(_MS3Pin, LOW);
        _stepsPerRevolution = 800;
        _microsteps = 4;
        _minDelay = 700;
    }
    else if (mode == EIGHTH) {
        digitalWrite(_MS1Pin, HIGH);
        digitalWrite(_MS2Pin, HIGH);
        digitalWrite(_MS3Pin, LOW);
        _stepsPerRevolution = 1600;
        _microsteps = 8;  
        _minDelay = 150; 
    }
    else if (mode == SIXTEENTH) {
        digitalWrite(_MS1Pin, HIGH);
        digitalWrite(_MS2Pin, HIGH);
        digitalWrite(_MS3Pin, HIGH);
        _stepsPerRevolution = 3200;
        _microsteps = 16; 
        _minDelay = 100; // TODO: un getter de este min delay
    }
}

motor::~motor() {
}

void motor::step() const {
    digitalWrite(_stepPin, HIGH);
    delayMicroseconds(5); // Probably unnecesary because delay in digital Write is pretty long
    digitalWrite(_stepPin, LOW);
}

void motor::step(int N) const {
    int i;
    for (i = 0; i < N; i++) {
        step();
        delayMicroseconds(_minDelay);
    }
}

void motor::setDirection(const byte direction) {
    digitalWrite(_directionPin, direction);
}

void motor::init() {
    _i = 0;
    i_sign = 1;
    _stepDelay = floor((float)timeTable[0] / (float) _stepsPerInterval);
    if (log == true)
        Serial.println(micros()-initial_time); // DEBUG  
    _timeToMove = _stepDelay;
    _takenStepsInInterval = 0;
    _timeLastInterval = 0;
    _nRounds = 0; // DEBUG
}

void motor::move() {
    
    // Checks if it's time to move.
    if (micros()-initial_time-_timeLastInterval >= _timeToMove) {
        step();
        _takenStepsInInterval++;
        _timeToMove += _stepDelay; 
    }
    // TODO: agregar un else return
    // Change interval
    if (_takenStepsInInterval >= _stepsPerInterval) {

        _timeLastInterval += timeTable[_i];
        _i = _i + i_sign;

        // Check end of array
        if (_i < 0) {
            if (digitalRead(_directionPin) == HIGH)
                digitalWrite(_directionPin,LOW);
            else
                digitalWrite(_directionPin,HIGH);
            i_sign = -i_sign;
            _i = _i + i_sign;
            _nRounds++;  
        } 
        // If index exceeds max index value, then index direction has to be changed
        else if (_i >= (int)table_size) {
            i_sign = -i_sign;
            _i = _i + i_sign;
            _nRounds++;
        }
        //if (_nRounds == 2) // DEBUG
        //    must_stop = true;
            
        // Reasignar parametros
        _stepDelay = floor((float)timeTable[_i] / (float) _stepsPerInterval);
        _takenStepsInInterval = 0;
        _timeToMove = _stepDelay;
        if (log == true)
            Serial.println(micros()-initial_time); // DEBUG  
    } 
}

int motor::getStepsPerRevolution() const {
    return _stepsPerRevolution;
}  

step_mode_t motor::getStepMode() const {
    return _stepMode;
}