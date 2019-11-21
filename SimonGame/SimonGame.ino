// Core library for code-sense
#if defined(WIRING) // Wiring specific
#include "Wiring.h"
#elif defined(MAPLE_IDE) // Maple specific
#include "WProgram.h"
#elif defined(MPIDE) // chipKIT specific
#include "WProgram.h"
#elif defined(DIGISPARK) // Digispark specific
#include "Arduino.h"
#elif defined(ENERGIA) // LaunchPad MSP430, Stellaris and Tiva, Experimeter Board FR5739 specific
#include "Energia.h"
#elif defined(CORE_TEENSY) // Teensy specific
#include "WProgram.h"
#elif defined(ARDUINO) && (ARDUINO >= 100) // Arduino 1.0 and 1.5 specific
#include "Arduino.h"
#elif defined(ARDUINO) && (ARDUINO < 100) // Arduino 23 specific
#include "WProgram.h"
#else // error
#error Platform not defined
#endif

// Include application, user and local libraries
#include "SPI.h"

// Screen selection
#define HX8353E // HX8353E K35_SPI

#if defined(K35_SPI)
#include "Screen_K35_SPI.h"
Screen_K35_SPI myScreen;

#elif defined(HX8353E)
#include "Screen_HX8353E.h"
Screen_HX8353E myScreen;

#else
#error Unknown screen
#endif

/*Simon Game
 * ESC 120 Group 4 Final Project
 */

const int joyStickSel = 5;
const int joyStickX = 2;
const int joyStickY = 26;

int joyStickSelState = 0;
int joyStickXState, joyStickYState;

#define RGB_BLUE_LED 37 
#define RGB_GREEN_LED 38
#define RGB_RED_LED 39

void setup() {
  analogReadResolution(12);
  pinMode(joyStickSel, INPUT_PULLUP);
  Serial.begin(9600);
  pinMode(RGB_BLUE_LED, OUTPUT);
  pinMode(RGB_GREEN_LED, OUTPUT);
  pinMode(RGB_RED_LED, OUTPUT);
 
}

void loop() {
   myScreen.setFontSize(1);
    myScreen.setFontSolid(false);
    myScreen.gText(0, 60, "Hello there");
  // put your main code here, to run repeatedly: 
  joyStickXState = analogRead(joyStickX);
  joyStickYState = analogRead(joyStickY);
  Serial.print(joyStickYState);
  if(joyStickYState < 1500){
    digitalWrite(RGB_BLUE_LED, HIGH); 
    delay(300); 
    digitalWrite(RGB_BLUE_LED, LOW); 
    delay(300);
  }else if(joyStickYState >2500){
    digitalWrite(RGB_GREEN_LED, HIGH); 
    delay(300); 
    digitalWrite(RGB_GREEN_LED, LOW); 
    delay(300);
  }else{
    digitalWrite(RGB_RED_LED, HIGH); 
    delay(300); 
    digitalWrite(RGB_RED_LED, LOW); 
    delay(300);
  }
}
