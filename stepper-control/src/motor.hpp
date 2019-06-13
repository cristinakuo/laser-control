#ifndef _STEPPER_HPP_INCLUDED_
#define _STEPPER_HPP_INCLUDED_

#include <Arduino.h>
extern long unsigned initial_time;
extern long unsigned timeTable[200];
extern size_t table_size;
extern volatile bool must_stop;

typedef enum {FULL, HALF, QUARTER, EIGHTH, SIXTEENTH} step_mode_t;

class motor {
public:
	motor(const byte step_pin, const byte direction_pin, 
			const byte MS1_pin, const byte MS2_pin, const byte MS3_pin, 
            const step_mode_t mode,
            const int stepsPerRev, const int fullStepsPerInterval); // Constructor
	~motor(); // Destructor
	void step() const;
    void setStepMode(const step_mode_t mode);
    void setDirection(const byte direction);
    void move();
    void init();
	
private:
    size_t _microsteps;
    int _fullStepsPerInterval;
	byte _directionPin;
	byte _stepPin;
	byte _MS1Pin;
	byte _MS2Pin;
	byte _MS3Pin; 
	size_t _stepsPerRevolution;
    int i; // Table
    int i_sign;

    long unsigned _timeToMove;
    long unsigned _stepDelay;
    long unsigned _timeLastInterval;
    size_t _takenStepsInInterval;
    size_t _stepsPerInterval;
    size_t _nRounds; // DEBUG

};

#endif
