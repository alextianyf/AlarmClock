#include <Arduino.h>
#include "5641AS.h"
#include <TimeLib.h>
#include "alarmAndPushbutton.h"
#include "activeBuzzer.h"

#define REALTIME 1    //Real Time Interface
#define RTINC 2       //Real Time Increment Interface
#define ALARM 3       //Alarm interface
#define BUZZER 4      //Buzzer active state
#define DISMISS 5     //Dismiss buzzer state
int currentState;

void setup() {
  currentState = REALTIME;
  Serial.begin(9600);
  LED_Init();
  alarm_Init();
  buzzer_init();
}

void loop() {
  time_on();
  switch (currentState)
  {
    case REALTIME:
      RealTimeDisplay();

      //check if alarm is triggered
      if(alarm_triggered()){
        currentState = BUZZER;
      }

      //at current state, if interface button is pressed, move to RTINC state
      if(button1Release()){
        currentState = RTINC;
        button1Pressed = false;
      }

      //at current state, if interface button is pressed, do nothing
      if(button2Release()){
        currentState = REALTIME;
        button2Pressed = false;
      }

    break;

    case RTINC:
      RealTimeFlash();
      
      if(button1Release()){
        currentRealTimeDisplayPosition++;
        button1Pressed = false;
        if(currentRealTimeDisplayPosition >= 4){
          currentRealTimeDisplayPosition = 0;
          currentState = ALARM;
        }
      }

      if(button2Release()){
        RealTimeInc();
        button2Pressed = false;
      }
    break;

    case ALARM:
      alarmTimeDisplay();
      
      if(button1Release()){
        currentDisplayPosition++;
        button1Pressed = false;
        if(currentDisplayPosition >= 4){
          currentDisplayPosition = 0;
          currentState = REALTIME;
          button1Pressed = false;
        }
      }

      if(button2Release()){
        AlarmTimeInc();
        button2Pressed = false;
      }
    break;
  
    case BUZZER:
      if(alarmActive){
        buzzer_on();
      }
      currentState = DISMISS;
    break;

    case DISMISS:
      RealTimeDisplay();
      currentState = BUZZER;
      if(button2Release() && alarmActive){
        alarmActive = false;
        buzzer_off();    
        alarmResume();//need opt
        currentState = REALTIME;
        button2Pressed = false;
      }      
    break;

    default:
      break;
  }
  
}

