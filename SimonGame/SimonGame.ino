// Include application, user and local libraries
#include "SPI.h"
#include <LCD_screen.h>
#include <LCD_screen_font.h>
#include <LCD_utilities.h>
#include <Screen_HX8353E.h>
#include <Terminal12e.h>
#include <Terminal6e.h>
#include <Terminal8e.h>
Screen_HX8353E myScreen;


// Screen selection
#define HX8353E // HX8353E K35_SPI

#if defined(K35_SPI)
#include "Screen_K35_SPI.h"
Screen_K35_SPI myScreen;

#elif defined(HX8353E)

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

//LED colors
#define RGB_BLUE_LED 37 
#define RGB_GREEN_LED 38
#define RGB_RED_LED 39

//Buttons
#define button1 33
#define button2 32

void startScreen(){
  myScreen.gText(30,0, "Simon Game");
  myScreen.gText(20, 50, "Press button 1");
  myScreen.gText(40, 70, "to start");
}

void setup() {
  analogReadResolution(12);
  pinMode(joyStickSel, INPUT_PULLUP);
  Serial.begin(9600);
  /*pinMode(RGB_BLUE_LED, OUTPUT);
  pinMode(RGB_GREEN_LED, OUTPUT);
  pinMode(RGB_RED_LED, OUTPUT);*/
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
  myScreen.begin();
  myScreen.setFontSize(1);
  myScreen.setFontSolid(false);
  fillArray();
  startScreen();
}

int returnDirection(){
    joyStickXState = analogRead(joyStickX);
    joyStickYState = analogRead(joyStickY);
    if((joyStickYState > 3800)  && (joyStickXState > 300) && (joyStickXState < 3800)){
      //up
      return 1;
    }else if((joyStickYState <300) && (joyStickXState > 300) && (joyStickXState < 3800)){
      //down
     return 2;
    }else if((joyStickXState <300) && (joyStickYState > 300) && (joyStickYState < 3800)){
      //left
      return 3;
    }else if((joyStickXState > 3800) && (joyStickYState > 300) && (joyStickYState < 3800)){
      //right
      return 4;
    }
}

boolean joyStickCentered(){
  joyStickXState = analogRead(joyStickX);
  joyStickYState = analogRead(joyStickY);
  if((joyStickYState < 3800) && (joyStickYState > 300) && (joyStickXState < 3800) && (joyStickXState > 300)){
    return true;
  }else{
    return false;
  }
}

void fillArray(){
  int directionsArray[250];
  for(int i=0; i<250; i++){
    directionsArray[i] = random(1,4);
  }
}

int score = 0;


boolean startButton = false;
void loop() {
  Serial.println(startButton);
  if(digitalRead(button1) == 0){
    startButton = true;
    myScreen.clear();
  }
  if(startButton){ 
    if(joyStickCentered() == true){
      myScreen.gText(0,0,"Hello bois");
    }else{
      myScreen.clear();
      myScreen.gText(0,0,"Goodbi bois"); 
    }
  }
  
}
