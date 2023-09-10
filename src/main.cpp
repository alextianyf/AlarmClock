#include <Arduino.h>
#include "5641AS.h"
#include <TimeLib.h>
#include "alarmAndPushbutton.h"
#include "activeBuzzer.h"

#define IDLE 0    //initial state Real Time Interface
#define RTINC 1       //Real Time Increment Interface
#define ALARM 2       //Alarm interface
#define BUZZER 3      //Buzzer active state
#define DISMISS 4     //Dismiss buzzer state
int currentState;
unsigned long startTime = 0;
unsigned long delayDuration = 5000;
unsigned long AlarmDuration = 30000;

void setup() {
  currentState = IDLE;
  Serial.begin(9600);
  LED_Init();
  alarm_Init();
  buzzer_init();
  time_on();
}

void loop() {
  switch (currentState)
  {
    case IDLE:
      RealTimeDisplay();

      //check if alarm is triggered
      if(alarm_triggered()){
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

      //at current state, if interface button is pressed, do nothing
      if(button2Release()){
        currentState = IDLE;
        button2Pressed = false;
      }

    break;

    case RTINC:
      RealTimeFlash();
      
      if(button1Release()){
        startTime = millis();
        currentRealTimeDisplayPosition++;
        button1Pressed = false;
        if(currentRealTimeDisplayPosition >= 4){
          currentRealTimeDisplayPosition = 0;
          currentState = ALARM;
        }
      }else{
        if (millis() - startTime >= delayDuration) {
          currentRealTimeDisplayPosition = 0;
          currentState = IDLE;
        }
      }
      
      if(button2Release()){
        startTime = millis();
        RealTimeInc();
        button2Pressed = false;
      }else{
        if (millis() - startTime >= delayDuration) {
          currentRealTimeDisplayPosition = 0;
          currentState = IDLE;
        }
      }
    break;

    case ALARM:
      alarmTimeDisplay();
      
      if(button1Release()){
        startTime = millis();
        currentDisplayPosition++;
        button1Pressed = false;
        if(currentDisplayPosition >= 4){
          currentDisplayPosition = 0;
          currentState = IDLE;
          button1Pressed = false;
        }
      }else{
        if (millis() - startTime >= delayDuration) {
          currentDisplayPosition = 0;
          currentState = IDLE;
        }
      }

      if(button2Release()){
        startTime = millis();
        AlarmTimeInc();
        button2Pressed = false;
      }
      else{
        if (millis() - startTime >= delayDuration) {
          currentDisplayPosition = 0;
          currentState = IDLE;
        }
      }
    break;
  
    case BUZZER:
      if(alarmActive){
        buzzer_on();
      }
      currentState = DISMISS;
    break;

    case DISMISS:
      startTime = millis();
      RealTimeDisplay();
      currentState = BUZZER;
      if(button2Release() && alarmActive){
        alarmActive = false;
        buzzer_off();    
        currentState = IDLE;
        button2Pressed = false;
      }
      if(millis() - startTime >= AlarmDuration){
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

