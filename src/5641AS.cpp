#include "5641AS.h"
#include <TimeLib.h>
#include "alarmAndPushbutton.h"

unsigned long RealTimelastLEDChange = 0;
int currentRealTimeDisplayPosition = 0;
const int RealTimeUpdateInterval = 500;
int realTimeCounter = 0;

//use to store LED displayed number
int digits[4] = {0,0,0,0};

//Segment Pin number
int A = 7;
int B = 8;
int C = 9;
int D = 10;
int E = 11;
int F = 12;
int G = 13;

int Segment_Array[7] = {A, B, C, D, E, F, G};

//LED Control Pin number
int D1 = 14;//A0
int D2 = 15;//A1
int D3 = 16;//A2
int D4 = 17;//A3

int Control_Pin_Array[4] = {D1, D2, D3, D4};

int Number_Array[10][7] = {
   //A  B  C  D  E  F  G
    {1, 1, 1, 1, 1, 1, 0},//NUMBER 0
    {0, 1, 1, 0, 0, 0, 0},//NUMBER 1
    {1, 1, 0, 1, 1, 0, 1},//NUMBER 2
    {1, 1, 1, 1, 0, 0, 1},//NUMBER 3
    {0, 1, 1, 0, 0, 1, 1},//NUMBER 4
    {1, 0, 1, 1, 0, 1, 1},//NUMBER 5
    {1, 0, 1, 1, 1, 1, 1},//NUMBER 6
    {1, 1, 1, 0, 0, 0, 0},//NUMBER 7
    {1, 1, 1, 1, 1, 1, 1},//NUMBER 8
    {1, 1, 1, 1, 0, 1, 1},//NUMBER 9
};

/*
OBJECTIVE: Initialize 5641as module
PARAMETERS: void
RETURN: void
NOTE: Init segment in LOW to avoid Segment flashing in the initial state.
*/
void LED_Init(){
  //reset initial displayed number as "0000"
  for(int i = 0; i < 4; i++){
    digits[i] = 0;
  }

  //setting segment array connected port as OUTPUT mode and LOW
  for (int i = 0; i < 7; i++){
    pinMode(Segment_Array[i] ,OUTPUT);
    digitalWrite(Segment_Array[i], LOW);
  }
    
  //setting LED control connect port as OUTPUT mode and HIGH
  for (int i = 0; i < 4; i++){
    pinMode(Control_Pin_Array[i] ,OUTPUT);
    //LEDs all turn off when initialize
    digitalWrite(Control_Pin_Array[i], HIGH);
  }
}

/*
OBJECTIVE: Display number on a designated position
PARAMETERS:
    pos: the position of the LED
    num: the actual number will be displayed
RETURN: void
NOTE: void
*/
void display_single(int pos, int num) {
  for (int i = 0; i < 7; i++) {
    digitalWrite(Segment_Array[i], Number_Array[num][i]);
  }
}

/*
OBJECTIVE: Clear all the LED segment to avoid the Ghosting on 5641as Module
PARAMETERS: void
RETURN: void
NOTE: void
*/
void LED_Clear(){
  for (int i = 0; i < 7; i++) {
    digitalWrite(Segment_Array[i], LOW);
  }
}

/*
OBJECTIVE: Read <TimeLib.h> provided hour and minute value
PARAMETERS: void
RETURN: void
NOTE: This function should be used inside of loop() and always active
*/
void time_on(){
  digits[0] = hour() / 10;
  digits[1] = hour() % 10;
  digits[2] = minute() / 10;
  digits[3] = minute() % 10;
}

/*
OBJECTIVE: Display all four LEDs in 5641as module
PARAMETERS: void
RETURN: void
NOTE: void
*/
void RealTimeDisplay() {
  time_on();
  for (int i = 0; i < 4; i++) {
    digitalWrite(Control_Pin_Array[i], LOW); // Activate the current digit
    display_single(i, digits[i]); // Display the current digit
    
    //delay(1); // Optional - A brief delay to reduce flicker
    
    LED_Clear();//Clear all the segment
    
    digitalWrite(Control_Pin_Array[i], HIGH); // Deactivate the current digit
  }
}

/*
OBJECTIVE: Flashing the a single displayed real-time value
PARAMETERS: void
RETURN: void
NOTE: once Interface Switch buttone is pressed, enter REALTIMEADJUSTMENT state
the real-time display value will start flashing at the first position
Flashing position depends on currentRealTimeDisplayPosition parameter
*/
void RealTimeFlash(){
  time_on();
  unsigned long realTime = millis();
  if (realTime - RealTimelastLEDChange > RealTimeUpdateInterval) {
    realTimeCounter ++;
    RealTimelastLEDChange = millis();
  }

  bool displayDigits = ((realTimeCounter % 2) == 0);

  if(displayDigits){
    RealTimeDisplay();
  }else{
    for (int i = 0; i < 4; i++) {
      if(i != currentRealTimeDisplayPosition){
        digitalWrite(Control_Pin_Array[i], LOW); // Activate the current digit
        display_single(i, digits[i]); // Display the current digit
        
        LED_Clear();//Clear all the segment
      }
      digitalWrite(Control_Pin_Array[i], HIGH); // Deactivate the current digit
    }
  }

}

/*
OBJECTIVE: Increase LED displayed value at selected position
PARAMETERS: void
RETURN: void
NOTE: once Increment buttone is pressed, displayed value will increase at selected position at REALTIMEADJUSTMENT state
Increasing position depends on currentRealTimeDisplayPosition parameter
*/
void RealTimeInc() {
  int newHour = hour();
  int newMinute = minute();
  int newSecond = second();

  // Determine which time component to increment based on currentRealTimeDisplayPosition
  if (currentRealTimeDisplayPosition == 0) {
    newHour += 10;
  } else if (currentRealTimeDisplayPosition == 1) {
    newHour += 1;
  } else if (currentRealTimeDisplayPosition == 2) {
    newMinute += 10;
  } else if (currentRealTimeDisplayPosition == 3) {
    newMinute += 1;
  }

  // Handle minute and second overflow
  if (newMinute >= 60) {
    newMinute = 0;
    newHour += 1;
  }
  if (newHour >= 24) {
    newHour = 0; // Reset hours if it goes beyond 23
  }

  // Set the new time
  setTime(newHour, newMinute, newSecond, 0, 0, 0);
}

