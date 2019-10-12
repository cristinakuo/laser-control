#include "archimedean.hpp"

archimedean_param_t get_archimedean_params() {
    archimedean_param_t current_param;
    EEPROM.get(ARCHIM_PARAM_ADDR, current_param);
    return current_param;
}

void show_current_archimedean_params() {
    archimedean_param_t param = get_archimedean_params();

    String aStr = "a: ";
    String bStr = "b: ";

    lcd.clear();
    lcd.setCursor(0,0); 
    lcd.print(aStr + param.a);
    lcd.setCursor(0,1); 
    lcd.print(bStr + param.b);
    
    delay(2000);
}

// TODO: modularizar porque esta gigante
void ask_for_new_archimedean_params() {

    show_current_archimedean_params();

    lcd.clear();
    lcd.setCursor(0,0); 
    lcd.print("New params?"); 
    lcd.setCursor(0,1); 
    lcd.print("1. Yes 2. No");

    // TODO: Mover a una clase keypad propia
    // If answer is no, get out of function
    char gottenKey = ' ';
    while(gottenKey != '1' && gottenKey != '3') {
        gottenKey = customKeypad.waitForKey();
        Serial.println(gottenKey);
    }
    if (gottenKey == '3') {// No
        return;
    }
    
    // Proceed with setting new params
    // TODO: Add X_min
    lcd.clear();
    lcd.setCursor(0,0); 
    lcd.print("Enter a:");
    lcd.setCursor(0,1); 
    delay(1000); 
    float a = receive_number();

    lcd.clear();
    lcd.setCursor(0,0); 
    lcd.print("Enter b:");
    lcd.setCursor(0,1); 
    delay(1000); 
    float b = receive_number();

    lcd.clear();
    lcd.setCursor(0,0); 
    lcd.print("Enter Xmin:");
    lcd.setCursor(0,1); 
    delay(1000); 
    float X_min = receive_number();

    archimedean_param_t new_param;
    new_param.a = a;
    new_param.b = b;
    new_param.X_min = X_min;

    EEPROM.put(ARCHIM_PARAM_ADDR, new_param);
}