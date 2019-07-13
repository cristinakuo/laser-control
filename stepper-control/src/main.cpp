#include <Arduino.h>
#include <math.h>
#include "Keypad.h"

#include "menu.hpp"
#include "ports.hpp"
#include "motor.hpp"

#define BAUDRATE 9600 // Serial communication

// Global variables 
volatile bool must_stop = false;
long unsigned timeTable[ARRAY_MAX_LEN];
size_t table_size; // TODO: ver que onda cuando haya otra tabla 
long unsigned initial_time; // [us]

// Keypad
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, KEYPAD_ROWS, KEYPAD_COLS); 

// Stepper
step_mode_t chosenMode = SIXTEENTH; // DEBUG: despues deberia ser const!
byte chosenDir = LOW; // DEBUG: solo para cambiar facilmente en las pruebas

motor carrito(STEP_PIN, DIRECTION_PIN, LENSE_MS1_PIN, LENSE_MS2_PIN, LENSE_MS3_PIN,
            chosenMode, STEPS_PER_REV, FULL_STEPS_PER_INTERVAL);

motor target(TARGET_STEP_PIN, TARGET_DIRECTION_PIN, TARGET_MS1_PIN, TARGET_MS2_PIN, TARGET_MS3_PIN,
            chosenMode, STEPS_PER_REV, FULL_STEPS_PER_INTERVAL);

// Menu
menu_state_t my_menu;
menu_list_t my_menu_list[NUM_MENU_ITEMS]; 
bool thereIsInput;

void setup() {
	// Init serial communication
    Serial.begin(BAUDRATE);
    // Set input and output ports
    //pinMode(BUTTON_PIN, INPUT);
}

void loop() {
	thereIsInput = false;
	int myOpt; // TODO: declararlo afuera sin que se rompa nada 

	mainMenu(my_menu, my_menu_list);
	
    while (thereIsInput != true) {
		if (my_menu.has_opt == true) {
            myOpt = my_menu.input;
            my_menu.has_opt = false;
            break;
        }
        if (Serial.available() > 0) {
			myOpt = Serial.read();
			myOpt = myOpt - 48;
			thereIsInput = true;
		}
		else 
			myOpt = 0;
	}
	menu_set_input(my_menu, myOpt);
}