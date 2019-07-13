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

// Keypad variables
byte rowPins[KEYPAD_ROWS] = {KEYPAD_R1_PIN, KEYPAD_R2_PIN, KEYPAD_R3_PIN, KEYPAD_R4_PIN};
byte colPins[KEYPAD_COLS] = {KEYPAD_C1_PIN, KEYPAD_C2_PIN, KEYPAD_C3_PIN, KEYPAD_C4_PIN}; 

char hexaKeys[KEYPAD_ROWS][KEYPAD_COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

