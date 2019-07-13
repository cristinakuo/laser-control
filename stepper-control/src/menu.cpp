#include "menu.hpp"
void menu_init(menu_state_t &st) {
	st.input = 0;	
	st.levels_options[0]=0;
	st.levels_options[1]=0;
	st.levels_options[2]=0;
    st.has_opt = false;
}

void refresh(menu_state_t &st, menu_list_t menu_list[NUM_MENU_ITEMS]) {
	int opt;

	refresh_input(st);
	opt = getOpt(st,st.level);
	//if (menu_list+opt == sub1Menu) {
	//	Serial.println("Opcion invalida");
	//}
	menu_list[opt](st);
}

void refresh_input(menu_state_t &st) {
	int first_zero;
	int set_level;
	if (st.input!=0) {
		
		first_zero = find_val(st.levels_options,0); // Basicamente seria _level++;  
		if ( first_zero != VALUE_ERROR) {
			set_level = first_zero;
		}
		else
			return;
		setOpt(st,set_level,st.input);
		st.input = 0;
	}
}

void setOpt(menu_state_t &st, int lev, int value) {
	if (lev < NUM_LEVELS) {
		st.levels_options[lev] = value;
	}
}

int getOpt(menu_state_t &st, int lev) {
	return st.levels_options[lev];
}

void menu_set_input(menu_state_t &st, int val) {
	st.input = val;
}

//---- Execution functions


void execute_main_menu(menu_state_t &st, menu_list_t menu_list[NUM_MENU_ITEMS]) {
	Serial.println("This is MAIN MENU:");
	Serial.println(" 1. Control manual");
	Serial.println(" 2. Barrido");
}

void execute_sub1_menu(menu_state_t &st, menu_list_t menu_list[NUM_MENU_ITEMS]) {
	Serial.println("This is CONTROL MANUAL:");
	Serial.println("1. Espejo");
	Serial.println("2. Blanco");
	Serial.println("3. Go back"); 
	// Aca podria ir para ingresar un dato. con una funcion para ingresar un dato.
	// Keypad.receive y LCD execute
	// Guardarlo en una variable
}

void execute_sub2_menu(menu_state_t &st, menu_list_t menu_list[NUM_MENU_ITEMS]) {
	Serial.println("This is BARRIDO:");
	Serial.println("1. Arquimedes");
	Serial.println("2. Lineal");
	Serial.println("3. Go back"); 
}


void execute_sub1sub1_menu(menu_state_t &st, menu_list_t menu_list[NUM_MENU_ITEMS]) {
	Serial.println("This is ESPEJO:");
	Serial.println("1. Go back"); 
}

void execute_sub1sub2_menu(menu_state_t &st, menu_list_t menu_list[NUM_MENU_ITEMS]) {
	Serial.println("This is BLANCO:");
	Serial.println("1. Go back");
}

void execute_sub2sub1_menu(menu_state_t &st, menu_list_t menu_list[NUM_MENU_ITEMS]) {
	Serial.println("This is Arquimedes:");
	//Serial.println("1. Go back");

    // TODO: agregar barrido
    delay(1000);
	st.has_opt = true;
	st.input = 1; 
}

void execute_sub2sub2_menu(menu_state_t &st, menu_list_t menu_list[NUM_MENU_ITEMS]) {
	Serial.println("This is Lineal:");
	Serial.println("1. Go back"); 
}



void goBack(menu_state_t &st, menu_list_t menu_list[NUM_MENU_ITEMS]) {
	if (st.level > 0 && st.level < NUM_LEVELS) {
		setOpt(st,st.level,0);
		setOpt(st,st.level-1,0);
		mainMenu(st, menu_list); // Con esto llegara a un nivel anterior
	}
	else
		Serial.println("Error in Go back!");
}

// Returns index of wanted value
int find_val(int array[NUM_LEVELS], int i) {
	size_t n;
	for(n = 0; n < NUM_LEVELS; n++) {
		if (array[n] == i)
			return n;
	}
	return VALUE_ERROR;
}

// ------ Public functions
void mainMenu(menu_state_t &st, menu_list_t menu_list[NUM_MENU_ITEMS]) {
	menu_list[0] = execute_main_menu;
	menu_list[1] = sub1Menu; // TODO: cambiar al sub1
	menu_list[2] = sub2Menu;
	menu_list[3] = NULL;

	st.level = 0;
	refresh(st, menu_list);
}

void sub1Menu(menu_state_t &st, menu_list_t menu_list[NUM_MENU_ITEMS]) {
	menu_list[0] = execute_sub1_menu;
	menu_list[1] = sub1sub1Menu;
	menu_list[2] = sub1sub2Menu;
	menu_list[3] = goBack;

	st.level = 1;
	refresh(st, menu_list);

}

void sub2Menu(menu_state_t &st, menu_list_t menu_list[NUM_MENU_ITEMS]) {
	menu_list[0] = execute_sub2_menu;
	menu_list[1] = sub2sub1Menu;
	menu_list[2] = sub2sub2Menu;
	menu_list[3] = goBack;
	
	st.level = 1;
	
	refresh(st, menu_list);
}


void sub1sub1Menu(menu_state_t &st, menu_list_t menu_list[NUM_MENU_ITEMS]) {
	menu_list[0] = execute_sub1sub1_menu;
	menu_list[1] = goBack;
	menu_list[2] = NULL;
	menu_list[3] = NULL;

	st.level = 2;
	refresh(st, menu_list);
}

void sub1sub2Menu(menu_state_t &st, menu_list_t menu_list[NUM_MENU_ITEMS]) {
	menu_list[0] = execute_sub1sub2_menu;
	menu_list[1] = goBack;
	menu_list[2] = NULL;
	menu_list[3] = NULL;

	st.level = 2;
	refresh(st, menu_list);
}

void sub2sub1Menu(menu_state_t &st, menu_list_t menu_list[NUM_MENU_ITEMS]) {
	menu_list[0] = execute_sub2sub1_menu;
	menu_list[1] = goBack;
	menu_list[2] = NULL;
	menu_list[3] = NULL;

	st.level = 2;
	refresh(st, menu_list);	
}

void sub2sub2Menu(menu_state_t &st, menu_list_t menu_list[NUM_MENU_ITEMS]) {
	menu_list[0] = execute_sub2sub2_menu;
	menu_list[1] = goBack;
	menu_list[2] = NULL;
	menu_list[3] = NULL;

	st.level = 2;
	refresh(st, menu_list);
}


void barrido(functions_t func) {
    carrito.isCarrito = true; // DEBUG: esto es harcodeado para imprimir
    target.isCarrito = false; // DEBUG: esto es harcodeado
    
    target.setDirection(chosenDir);
    carrito.setDirection(chosenDir);
    
    // Creacion de la tabla
    table_size = create_table(func, chosenMode);

    // Teclado para empezar
    wait_to_start();
    
    // Stop program if button is hit 
    //attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), button_ISR, RISING);

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

// TODO: mover a otro lado
void button_ISR() {
    must_stop = true;
}

// TODO: mover a otro lado

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

// Calculates time table in micro seconds
size_t create_table(functions_t f, step_mode_t mode) {
    int mm_per_rev = 1;
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
            min_delay = 100;
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

