#include <Arduino.h>
#include <math.h>
#include "Keypad.h"
#include "LiquidCrystal.h"
#include "menu.hpp"

#define BAUDRATE 9600 // Serial communication


// Keypad
#define KEYPAD_R1_PIN 53
#define KEYPAD_R2_PIN 51
#define KEYPAD_R3_PIN 49
#define KEYPAD_R4_PIN 47
#define KEYPAD_C1_PIN 45
#define KEYPAD_C2_PIN 43
#define KEYPAD_C3_PIN 41
#define KEYPAD_C4_PIN 39

#define KEYPAD_ROWS 4
#define KEYPAD_COLS 4

//typedef enum {ARCHIMEDEAN, LINEAR} functions_t;

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
