#include <Arduino.h>
#include <math.h>
#include "Keypad.h"
#include "LiquidCrystal.h"
#include "menu.hpp"
#include "ports.hpp"
#include "motor.hpp"

#define BAUDRATE 9600 // Serial communication

int initial_menu();

// Global variables 
volatile bool must_stop = false;
long unsigned timeTable[ARRAY_MAX_LEN];
size_t table_size; // TODO: ver que onda cuando haya otra tabla 
long unsigned initial_time; // [us]

// Keypad variables
byte rowPins[KEYPAD_ROWS] = {KEYPAD_R1_PIN, KEYPAD_R2_PIN, KEYPAD_R3_PIN, KEYPAD_R4_PIN};
byte colPins[KEYPAD_COLS] = {KEYPAD_C1_PIN, KEYPAD_C2_PIN, KEYPAD_C3_PIN, KEYPAD_C4_PIN}; 

char hexaKeys[KEYPAD_ROWS][KEYPAD_COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, KEYPAD_ROWS, KEYPAD_COLS); 
void setup() {
	// Init serial communication
    Serial.begin(BAUDRATE);

    // Set input and output ports
    pinMode(BUTTON_PIN, INPUT);
    
    //barrido(ARCHIMEDEAN);


}
menu_state_t my_menu;
menu_list_t my_menu_list[NUM_MENU_ITEMS]; 

//void loop() {
//}

// Configurate stepper
step_mode_t chosenMode = SIXTEENTH; // DEBUG: despues deberia ser const!
byte chosenDir = LOW; // DEBUG: solo para cambiar facilmente en las pruebas

motor carrito(STEP_PIN, DIRECTION_PIN, LENSE_MS1_PIN, LENSE_MS2_PIN, LENSE_MS3_PIN,
            chosenMode, STEPS_PER_REV, FULL_STEPS_PER_INTERVAL);

motor target(TARGET_STEP_PIN, TARGET_DIRECTION_PIN, TARGET_MS1_PIN, TARGET_MS2_PIN, TARGET_MS3_PIN,
            chosenMode, STEPS_PER_REV, FULL_STEPS_PER_INTERVAL);



// Para la prueba 2
void loop() {
	bool thereIsInput = false;
	int myOpt;


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

// TODO: eliminar porque ya no sirve
int initial_menu() {
    char aux;
    int opt_function;
    
    // Funcion deseada
    Serial.println("Enter desired function: 1- Arquimedes 2- Linear");
    aux = customKeypad.waitForKey(); 
    opt_function = aux - '0'; // TODO: devolver esto
    Serial.print("You've chosen "); 
    Serial.println(opt_function);

    return opt_function;
}
