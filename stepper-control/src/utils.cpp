#include "utils.hpp"
// DEBUG: move to somewhere else
float receive_number() {
	const size_t MAX_DIGITS = 10;
	const char ENTER_KEY = '#'; // TODO: universal enter
	char digits[MAX_DIGITS];
	char pressed_key;
	size_t i = 0;
	bool finished_input = false;

    //lcd.autoscroll(); 

	while (finished_input != true) {
		pressed_key = customKeypad.waitForKey();
		
		if (pressed_key == ENTER_KEY) {
			finished_input = true;
		} else if (i >= MAX_DIGITS){
			
    		lcd.setCursor(0,2); 
    		lcd.print("Err:Reached max"); 
			finished_input = true;
		} else { 
			if (pressed_key == '*')
                pressed_key = '.'; 
            digits[i] = pressed_key;
            lcd.print(pressed_key);
			i++;
		}
	}
    
    // DEBUG
    Serial.print("Input number is:"); 
    Serial.println(atof(digits));

    //lcd.noAutoscroll();

	return atof(digits); // DEBUG
}

// TODO: initialize arrays with all zeros