#ifndef _LCD_HPP_INCLUDED_
#define _LCD_HPP_INCLUDED_

#include <Arduino.h>
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
// TODO: cambiar a pines posibles del mega
#define LED_RS_PIN 12
#define LED_EN_PIN 11
#define LED_D4_PIN 4
#define LED_D5_PIN 3
#define LED_D6_PIN 2
#define LED_D7_PIN 13 // Ver que onda
#define LED_ROWS_NUMBER 2
#define LED_COLUMNS_NUMBER 16

#define WELCOME_MSG "Hello Master!"

void lcd_init();

#endif