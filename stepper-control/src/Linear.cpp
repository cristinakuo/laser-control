#include "Linear.hpp"

linear_param_t get_linear_params() {
    linear_param_t current_param;
    EEPROM.get(LINEAR_PARAM_ADDR, current_param);
    return current_param;
}

void show_current_linear_params() {
    linear_param_t param = get_linear_params();

    String aStr = "lin: ";
    String bStr = "ang: ";

    lcd.clear();
    lcd.setCursor(0,0); 
    lcd.print(aStr + param.linear_speed);
    lcd.setCursor(0,1); 
    lcd.print(bStr + param.angular_speed);
    
    delay(2000);
}

// TODO: modularizar porque esta gigante
void ask_for_new_linear_params() {

    show_current_linear_params();

    lcd.clear();
    lcd.setCursor(0,0); 
    lcd.print("New params?"); 
    lcd.setCursor(0,1); 
    lcd.print("1. Yes 3. No");

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
    lcd.clear();
    lcd.setCursor(0,0); 
    lcd.print("Linear speed:");
    lcd.setCursor(0,1); 
    delay(1000); 
    float linear_speed = receive_number();

    lcd.clear();
    lcd.setCursor(0,0); 
    lcd.print("Angular speed:");
    lcd.setCursor(0,1); 
    delay(1000); 
    float angular_speed = receive_number();

    linear_param_t new_param;
    new_param.linear_speed = linear_speed;
    new_param.angular_speed = angular_speed;
    

    EEPROM.put(LINEAR_PARAM_ADDR, new_param);
}