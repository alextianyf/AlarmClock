#include <Arduino.h>
#include "5641AS.h"
#include <TimeLib.h>
#include "alarmAndPushbutton.h"
#include "activeBuzzer.h"

#define REALTIME 0
#define REALTIMEADJUSTMENT 1
#define ALARM 2
#define BUZZER 3
int currentState = REALTIME;

int currentHour = hour();
int currentMinute = minute();

void setup() {
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
      alarmResume();

      if(alarm_triggered() && alarmActive){
        currentState = BUZZER;
      }

      if(button2Release() && alarmActive){
        if(currentState == REALTIME){
          alarmActive = false;
          buzzer_off();    
        }
        button2Pressed = false;
      }

      if(button1Release() && currentState == REALTIME){
        currentState = REALTIMEADJUSTMENT;
        button1Pressed = false;
      }

      if(button2Release() && currentState == REALTIME){
        currentState = REALTIME;
        button2Pressed = false;
      }

    break;

    case REALTIMEADJUSTMENT:
      RealTimeaAdjustment();
      
      if(button1Release()){
        currentRealTimeDisplayPosition++;
        button1Pressed = false;
        if(currentRealTimeDisplayPosition >= 4){
          currentRealTimeDisplayPosition = 0;
          currentState = ALARM;
          button1Pressed = false;
        }
      }

      if(button2Release() && currentState == REALTIMEADJUSTMENT){
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

      if(button2Release() && currentState == ALARM){
        AlarmTimeInc();
        button2Pressed = false;
      }
    break;
  
    case BUZZER:
      if(alarmActive){
        buzzer_on();
      }
      currentState = REALTIME;
    break;
    
    default:
      break;
  }
  
}

