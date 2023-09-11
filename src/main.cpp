/**
 * PROJECT: Simple Arduino Made Alarm Clock

 * AUTHOR: Alex Tian
 * DATE: Aug 2023
  
 * FUNCTION:
    1. Time display
    2. Time display interface - Time increment interface - Alarm time setting interface and switching interface
    3. Time modification
    4. Alarm time setting and display interface
    5. Dismiss alarm
  
 * SUMMARY:
    This is a simple Arduino Uno made alarm clock project. The purpose of this project is to have a good understanding of state machine, 
    and gain experience of creating driver without relaying on the pre-built libraries. It is a good practice for 4 x 7 segments.
    After completing this project, you will get good practice of "Array, For Loop, State Machine, Creating Driver, and understanding how Segment LED works"
    I hope everyone can learn something from this mid level project since there are no points, structures, algorithm, and data structures involved
    This project still can be improved significantly. Feel free to use it as template and improve the project your own!
 */

#include <Arduino.h>
#include "5641AS.h"
#include <TimeLib.h>
#include "alarmAndPushbutton.h"
#include "activeBuzzer.h"

#define IDLE      0  //initial state Real Time Interface
#define RTINC     1  //Real Time Increment Interface
#define ALARM     2  //Alarm interface
#define BUZZER    3  //Buzzer active state
#define DISMISS   4  //Dismiss buzzer state
int currentState;
unsigned long startTime = 0;// measuring the duration during which the system maintains its current state before transitioning gracefully to an IDLE state.
unsigned long delayDuration = 5000;// System enters IDLE state (Real Time Increment and Alarm Interface) after 5s of inactivity.
unsigned long AlarmDuration = 30000;// Auto-dismiss alarm after 30s if no action.

void setup() {
  currentState = IDLE;
  Serial.begin(9600);
  LED_Init();
  btn_Init();
  buzzer_init();
  time_on();//have the system start counting minutes and hours
}

void loop() {
  switch (currentState)
  {
    case IDLE://IDLE state
      RealTimeDisplay();

      //check if alarm is triggered
      if(alarm_triggered()){
        startTime = millis();//starting counting if buzzer over 30s
        currentState = BUZZER;
      }
      
      //check if alarm can be resume
      //if so, reset alarmAtive flag
      if(alarmResume()){
        alarmActive = true;
      }

      //at current state, if interface button is pressed, move to RTINC state
      if(button1Release()){
        startTime = millis();//active timer and ensure if there is no operation within 5s, then go back to IDLE State
        currentState = RTINC;
        button1Pressed = false;
      }

      //at current state, if interface button is pressed, back to IDLE state and do nothing
      if(button2Release()){
        currentState = IDLE;
        button2Pressed = false;
      }

    break;

    case RTINC://Real-Time Increment state
      RealTimeFlash();
      
      //BTN1 is pressed and shift flashing position
      if(button1Release()){
        startTime = millis();
        currentRealTimeDisplayPosition++;
        button1Pressed = false;
        if(currentRealTimeDisplayPosition >= 4){
          currentRealTimeDisplayPosition = 0;
          currentState = ALARM;
        }
      }else if (millis() - startTime >= delayDuration) {//no operation over 5s, go back to IDLE state
        currentRealTimeDisplayPosition = 0;
        currentState = IDLE;
      }
      
      //BTN2 is pressed and display value increment at selected position
      if(button2Release()){
        startTime = millis();
        RealTimeInc();
        button2Pressed = false;
      }else if (millis() - startTime >= delayDuration) {//no operation over 5s, go back to IDLE state
        currentRealTimeDisplayPosition = 0;
        currentState = IDLE;
      }
    break;

    case ALARM://Alarm setting state
      alarmTimeDisplay();//Display alarm time setting interface
      
      //BTN1 is pressed and shift flashing position
      if(button1Release()){
        startTime = millis();
        currentDisplayPosition++;
        button1Pressed = false;
        if(currentDisplayPosition >= 4){
          currentDisplayPosition = 0;
          currentState = IDLE;
          button1Pressed = false;
        }
      }else if (millis() - startTime >= delayDuration) {//no operation over 5s, go back to IDLE state
        currentDisplayPosition = 0;
        currentState = IDLE;
      }
      
      //BTN2 is pressed and display value increment at selected position
      if(button2Release()){
        startTime = millis();
        AlarmTimeInc();
        button2Pressed = false;
      }
      else if (millis() - startTime >= delayDuration) {//no operation over 5s, go back to IDLE state
        currentDisplayPosition = 0;
        currentState = IDLE;
      }
      
    break;
  
    case BUZZER://Buzzering state after alarm is triggered
      if(alarmActive){//if alarm is actived, then turn on buzzer
        buzzer_on();
      }
      currentState = DISMISS;
    break;

    case DISMISS://you can dismiss alarm at this state
      RealTimeDisplay();//if alarm is triggered, you are still able to observe the displayed time
      currentState = BUZZER;

      //if BTN2 is pressed, dismiss the alarm
      if(button2Release() && alarmActive){
        alarmActive = false;
        buzzer_off();    
        currentState = IDLE;
        button2Pressed = false;
      }else if(millis() - startTime >= AlarmDuration){//or if no operation over 30s after alarm is triggered, go back to IDLE state
        alarmActive = false;
        buzzer_off();    
        currentState = IDLE;
        button2Pressed = false;       
      } 
    break;

    default:
    break;
  }
  
}

