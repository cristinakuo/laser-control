#include "manual_control.hpp"

step_param_t get_step_param() {

    step_param_t current_param;
    EEPROM.get(STEP_PARAM_ADDR, current_param);
    return current_param;
}

void show_current_step_param() {
    step_param_t param = get_step_param();

    String fineStr = "FineIs: ";
    String coarseStr = "CoarseIs: ";

    lcd.clear();
    lcd.setCursor(0,0); 
    lcd.print(fineStr + param.fine);
    lcd.setCursor(0,1); 
    lcd.print(coarseStr + param.coarse);
    
    delay(2000);
}

// TODO: modularizar porque esta gigante
void ask_for_new_step_param() {

    show_current_step_param();

    lcd.clear();
    lcd.setCursor(0,0); 
    lcd.print("New params?"); 
    lcd.setCursor(0,1); 
    lcd.print("1. Yes 2. No");

    // TODO: Mover a una clase keypad propia
    // If answer is no, get out of function
    char gottenKey = ' ';
    while(gottenKey != '1' && gottenKey != '3') {
        Serial.println("Antes del waitForKey");
        gottenKey = customKeypad.waitForKey();
        Serial.println(gottenKey);
        Serial.println("Dsp del waitForKey");
    }
    if (gottenKey == '3') {// No
        Serial.println("Returned from asking parameters without asking for new");
        return;
    }
    
    // Proceed with setting new params
    lcd.clear();
    lcd.setCursor(0,0); 
    lcd.print("Enter Fine:");
    lcd.setCursor(0,1); 
    delay(1000); 
    float fine_step = receive_number();

    lcd.clear();
    lcd.setCursor(0,0); 
    lcd.print("Enter Coarse:");
    lcd.setCursor(0,1); 
    delay(1000); 
    float coarse_step = receive_number();

    step_param_t new_param;
    new_param.fine = fine_step;
    new_param.coarse = coarse_step;

    EEPROM.put(STEP_PARAM_ADDR, new_param);
}