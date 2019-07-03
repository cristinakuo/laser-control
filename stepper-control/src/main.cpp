#include <Arduino.h>
#include <math.h>
#include "Keypad.h"
#include "motor.hpp"
#include "LiquidCrystal.h"
#include "menu.hpp"

#define BAUDRATE 9600 // Serial communication

// Stepper pins
#define DIRECTION_PIN 25
#define STEP_PIN 24
#define LENSE_MS1_PIN 50 // DEBUG: En realidad no sirven para nada
#define LENSE_MS2_PIN 51
#define LENSE_MS3_PIN 52

// Target stepper pins
#define TARGET_DIRECTION_PIN 27
#define TARGET_STEP_PIN 26
#define TARGET_MS1_PIN 50 // DEBUG: En realidad no sirven para nada
#define TARGET_MS2_PIN 51
#define TARGET_MS3_PIN 52

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

// Interrupt pins
#define BUTTON_PIN 2 // Start/Stop button

#define ARRAY_MAX_LEN 200 // Maximum length of Time interval array
#define STEPS_PER_REV 200
#define FULL_STEPS_PER_INTERVAL 10

typedef enum {ARCHIMEDEAN, LINEAR} functions_t;

// Interrupt Services
void button_ISR();

// Functions
size_t create_table(functions_t, step_mode_t);
void wait_to_start();
int initial_menu();
void barrido(functions_t f);

const int mm_per_rev = 1;

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
    
    wait_to_start();
    barrido(ARCHIMEDEAN);
}

void loop() {
}

// Para la prueba 2
//void loop() {
//	bool thereIsInput = false;
//	int myOpt;
//
//	mainMenu(my_menu, my_menu_list);
//	
//    while (thereIsInput != true) {
//		    if (my_menu.has_opt == true) {
//                myOpt = my_menu.input;
//                my_menu.has_opt = false;
//                break;
//          }
//          if (Serial.available() > 0) {
//			myOpt = Serial.read();
//			myOpt = myOpt - 48;
//			thereIsInput = true;
//		}
//		else 
//			myOpt = 0;
//	}
//    
//	menu_set_input(my_menu, myOpt);
//}

// Calculates time table in micro seconds
size_t create_table(functions_t f, step_mode_t mode) {
    if (f == ARCHIMEDEAN) {
        long X0_measured = 10; // [mm]
        long X_min = 1;
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
            min_delay = 75;
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

// TODO: Fix
// Loop until button is hit
void wait_to_start() {
    char button = '1';
    Serial.println("Press 0 to start:");
    while(button!='0') {
        button = customKeypad.getKey();
    }
    Serial.println("Starting..."); 
}

void button_ISR() {
    must_stop = true;
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

void barrido(functions_t func) {
    
    step_mode_t chosenMode = EIGHTH; // DEBUG: despues deberia ser const!
    byte chosenDir = HIGH; // DEBUG: solo para cambiar facilmente en las pruebas
    
    // Configurate stepper
    motor carrito(STEP_PIN, DIRECTION_PIN, LENSE_MS1_PIN, LENSE_MS2_PIN, LENSE_MS3_PIN,
            chosenMode, STEPS_PER_REV, FULL_STEPS_PER_INTERVAL);
    carrito.isCarrito = true; // DEBUG: esto es harcodeado para imprimir

    motor target(TARGET_STEP_PIN, TARGET_DIRECTION_PIN, TARGET_MS1_PIN, TARGET_MS2_PIN, TARGET_MS3_PIN,
            chosenMode, STEPS_PER_REV, FULL_STEPS_PER_INTERVAL);
    target.isCarrito = false; // DEBUG: esto es harcodeado

    target.setDirection(chosenDir);
    carrito.setDirection(chosenDir);
    
    // Creacion de la tabla
    table_size = create_table(func, chosenMode);

    // Teclado para empezar
    wait_to_start();
    // Stop program if button is hit 
    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), button_ISR, RISING);

    initial_time = micros();
    carrito.init();
    target.init();
    // Loop
    char stopkey = '0';
    while(stopkey != '*') {
        carrito.move();
        target.move();
        stopkey = customKeypad.getKey();
    }
    Serial.println("Stopped.");
}