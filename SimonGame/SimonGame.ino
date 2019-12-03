/*Simon Game
 * ESC 120 Section 51 Group 4 Final Project
 */

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

//JoyStick pin defintions
const int joyStickSel = 5;
const int joyStickX = 2;
const int joyStickY = 26;

//Buttons
#define button1 33
#define button2 32


//Set all initial variables
int joyStickSelState = 0;
int joyStickXState, joyStickYState;
int score = 0;
int directionsArray[250];
String directionsArrayString[250];
boolean sequenceDisplayed = false;
boolean startButton = false;


//Initial setup Method
void setup() {
  //This is necessary for the screen to work properly. 
  analogReadResolution(12);
  //Begin Screen
  Serial.begin(9600);
  myScreen.begin();
  //Set pin modes of buttons and the joystick to be input
  pinMode(joyStickSel, INPUT_PULLUP);
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
  //Set screen font properties
  myScreen.setFontSize(1);
  //Fill the Array
  fillArray();
  //Call the start Screen Method
  startScreen();
}

//Method Call to write the start Screen
void startScreen(){
  myScreen.gText(30,0, "Simon Game");
  myScreen.gText(20, 50, "Press button 1");
  myScreen.gText(40, 70, "to start");
}

//Method to return the direction the joystick is currently positioned in
//Over 3800 or less than 300 in a given direction is considered to be pushed in that direction
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

//Returns true if joystick is centered, and false if not
//Centered is considered to be between a value of 300 and 3800 in both the X and Y directions
boolean joyStickCentered(){
  joyStickXState = analogRead(joyStickX);
  joyStickYState = analogRead(joyStickY);
  if((joyStickYState < 3800) && (joyStickYState > 300) && (joyStickXState < 3800) && (joyStickXState > 300)){
    return true;
  }else{
    return false;
  }
}
//Method to fill both the array containing the numerical values of the directions, as well as the String values
// Iterates through all 250 values of the directionsArray and gives each a value of either 1, 2, 3, or 4
// After a value has been assigned, it corresponds that to a string containing the direction, and then inputs that in to another array which can be used to display the sequence of directions required to be input
void fillArray(){
  int temp;
  for(int i=0; i<250; i++){  
    randomSeed(analogRead(A0));
    temp = random(1,5);
    directionsArray[i] = temp;
    switch(temp){
      case 1:
        directionsArrayString[i] = "Up";
        break;
      case 2:
        directionsArrayString[i] = "Down";
        break;
      case 3:
        directionsArrayString[i] = "Left";
        break;
      case 4:
        directionsArrayString[i] = "Right";
        break;
      default:
        directionsArrayString[i] = "You should never see this";
        break;
    }
  } 
}

//Called when the player fails the game, outputs the score and prompts the user to play again
void endGame(){
  myScreen.clear();
  myScreen.gText(0,0, "You Lost! :(");
  myScreen.gText(0, 20, "Your final score");
  myScreen.gText(0, 40, "Was: " + String(score));
  myScreen.gText(0, 60, "Unplug me to");
  myScreen.gText(0, 80, "Play again"); 
  while(1){
    
  }
}

//Displays the sequence up until the given point, using the score as the variable to determine how far into the sequence to display
void displaySequence(int level){
  String direction;
  for(int i=0; i<=level; i++){
    direction = directionsArrayString[i];
    myScreen.gText(0,0,direction);
    myScreen.gText(0,120,"Score: " + String(score)); 
    delay(500);
    myScreen.clear();
  }
}

//Is called after each round. First clears the sreen, then displays the sequence. Waits until the joystick moves then calls entering sequnce. If the sequence returns true, increase score and recursively call itself. If not, then call the endGame function
void playGame(){
    myScreen.clear();
    displaySequence(score);
    while(joyStickCentered()){
      myScreen.gText(0,0, "Enter the sequence");
    }
    if(enteringSequence(0)){
      score++;
      playGame();
    }else{
      endGame();
    }
    
}

//Finds the direction the joystick has been moved in, and the direction which is required at this point in the sequence. If it is correct, and the location is sequence is the same as the score returns true;
//If it's correct, but not the end of the sequence then it recurisvely calls itself, however increases the variable which determines location.
//If the joystick is in the incorrect posiiton, it returns false.
boolean enteringSequence(int locationInSequence){
  int currentDirection = returnDirection();
  int requiredDirection = directionsArray[locationInSequence];
  if(currentDirection == requiredDirection){
    if(locationInSequence == score){
      return true;
    }else{
      delay(200);
      while(joyStickCentered()){
      myScreen.gText(0,0, "Enter the sequence");
    }
    return enteringSequence((++locationInSequence));
    }
  } else if(currentDirection != requiredDirection){
    return false;
  }
}

//Calls the playGame function the initial time once the start button has been pressed
void loop() {
  if(digitalRead(button1) == 0){
    startButton = true;
    myScreen.clear();
  }
  if(startButton){ 
    playGame();
  }
  
}
