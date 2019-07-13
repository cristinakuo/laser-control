// Arduino
#include <Arduino.h>
#include "motor.hpp"
#include "Keypad.h"
#include "motor.hpp"
#define NUM_LEVELS 3
#define NUM_MENU_ITEMS 4
#define VALUE_ERROR -1

// TODO: temp here

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
// Interrupt pins
#define BUTTON_PIN 2 // Start/Stop button

#define ARRAY_MAX_LEN 200 // Maximum length of Time interval array
#define STEPS_PER_REV 200
#define FULL_STEPS_PER_INTERVAL 10

typedef struct {
	int input;
	int levels_options[NUM_LEVELS];
	int level;
    bool has_opt;
} menu_state_t;

typedef void (*menu_list_t)(menu_state_t &st);
typedef enum {ARCHIMEDEAN, LINEAR} functions_t;

extern Keypad customKeypad;
void menu_init(menu_state_t &st);
void menu_set_input(menu_state_t &st, int val);
void refresh(menu_state_t &st, menu_list_t menu_list[NUM_MENU_ITEMS]);
void setOpt(menu_state_t &st, int lev, int value);
int getOpt(menu_state_t &st, int lev);

void refresh_input(menu_state_t &st);

int find_val(int array[NUM_LEVELS],int i);

// Level 0
void mainMenu(menu_state_t &st, menu_list_t menu_list[NUM_MENU_ITEMS]);
void execute_main_menu(menu_state_t &st, menu_list_t menu_list[NUM_MENU_ITEMS]);

// Level 1
void sub1Menu(menu_state_t &st, menu_list_t menu_list[NUM_MENU_ITEMS]);
void execute_sub1_menu(menu_state_t &st, menu_list_t menu_list[NUM_MENU_ITEMS]);

void sub2Menu(menu_state_t &st, menu_list_t menu_list[NUM_MENU_ITEMS]);
void execute_sub2_menu(menu_state_t &st, menu_list_t menu_list[NUM_MENU_ITEMS]);

// Level 2
void sub1sub1Menu(menu_state_t &st, menu_list_t menu_list[NUM_MENU_ITEMS]);
void execute_sub1sub1_menu(menu_state_t &st, menu_list_t menu_list[NUM_MENU_ITEMS]);
void sub1sub2Menu(menu_state_t &st, menu_list_t menu_list[NUM_MENU_ITEMS]);
void execute_sub1sub2_menu(menu_state_t &st, menu_list_t menu_list[NUM_MENU_ITEMS]);

void sub2sub1Menu(menu_state_t &st, menu_list_t menu_list[NUM_MENU_ITEMS]);
void execute_sub2sub1_menu(menu_state_t &st, menu_list_t menu_list[NUM_MENU_ITEMS]);
void sub2sub2Menu(menu_state_t &st, menu_list_t menu_list[NUM_MENU_ITEMS]);
void execute_sub2sub2_menu(menu_state_t &st, menu_list_t menu_list[NUM_MENU_ITEMS]);

void goBack(menu_state_t &st, menu_list_t menu_list[NUM_MENU_ITEMS]);

void barrido(functions_t f);

// Interrupt Services
void button_ISR(); // TODO: mover a otro lado
void wait_to_start(); // TODO: mover


// Functions
size_t create_table(functions_t, step_mode_t);