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

    lcd.clear();
    lcd.setCursor(0,0); 
    lcd.print("1. Manual"); 
    lcd.setCursor(0,1); 
    lcd.print("2. Automatic");
}

void execute_sub1_menu(menu_state_t &st, menu_list_t menu_list[NUM_MENU_ITEMS]) {
	Serial.println("This is CONTROL MANUAL:");
	Serial.println("1. Espejo");
	Serial.println("2. Blanco");
	Serial.println("3. Go back"); 
	// Aca podria ir para ingresar un dato. con una funcion para ingresar un dato.
	// Keypad.receive y LCD execute
	// Guardarlo en una variable

    lcd.clear();
    lcd.setCursor(0,0); 
    lcd.print("1.Espj 2.Blnc"); 
    lcd.setCursor(0,1); 
    lcd.print("3. Atras");
}

void execute_sub2_menu(menu_state_t &st, menu_list_t menu_list[NUM_MENU_ITEMS]) {
	Serial.println("This is BARRIDO:");
	Serial.println("1. Arquimedes");
	Serial.println("2. Lineal");
	Serial.println("3. Go back"); 

    lcd.clear();
    lcd.setCursor(0,0); 
    lcd.print("1.Arq 2.Lin"); 
    lcd.setCursor(0,1); 
    lcd.print("3. Atras");

}


void execute_sub1sub1_menu(menu_state_t &st, menu_list_t menu_list[NUM_MENU_ITEMS]) {
	Serial.println("This is ESPEJO:");
	Serial.println("1. Go back");
    manual_control(carrito);
    delay(250);
    st.has_opt = true;
	st.input = 1;  // Vuelve automaticamente atras 
}

void execute_sub1sub2_menu(menu_state_t &st, menu_list_t menu_list[NUM_MENU_ITEMS]) {
	Serial.println("This is BLANCO:");
	Serial.println("1. Go back");

    manual_control(target);
    delay(1000);
    st.has_opt = true;
	st.input = 1;  // Vuelve automaticamente atras
}

void execute_sub2sub1_menu(menu_state_t &st, menu_list_t menu_list[NUM_MENU_ITEMS]) {
	Serial.println("This is Arquimedes:");
	Serial.println("Press * to go back");

    barrido_archim(ARCHIMEDEAN);
	delay(1000);
    st.has_opt = true;
	st.input = 1;  // Vuelve automaticamente atras
}

void execute_sub2sub2_menu(menu_state_t &st, menu_list_t menu_list[NUM_MENU_ITEMS]) {
	Serial.println("This is Lineal:");
	Serial.println("1. Go back"); 

    Serial.println("This is Arquimedes:");
	Serial.println("Press * to go back");

    barrido_linear(LINEAR);
	delay(1000);
    st.has_opt = true;
	st.input = 1;  // Vuelve automaticamente atras
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


// TODO: mover a otro lado
//void button_ISR() {
 //   must_stop = true;
//}

// TODO: mover a otro lado


