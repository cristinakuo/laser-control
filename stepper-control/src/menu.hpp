// Arduino
#include <Arduino.h>

#define NUM_LEVELS 3
#define NUM_MENU_ITEMS 4
#define VALUE_ERROR -1
typedef struct {
	int input;
	int levels_options[NUM_LEVELS];
	int level;
} menu_state_t;

typedef void (*menu_list_t)(menu_state_t &st);
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