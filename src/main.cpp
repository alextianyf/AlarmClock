#include <Arduino.h>
#include "5641AS.h"
#include <TimeLib.h>
#include "alarmAndPushbutton.h"

#define REALTIME 0
#define REALTIMEADJUSTMENT 1
#define ALARM 2
#define INCREMENT 3
int currentState = REALTIME;

int currentHour = hour();
int currentMinute = minute();

void setup() {
  Serial.begin(9600);
  LED_Init();
  alarm_Init();
}

void loop() {
  time_on();
  switch (currentState)
  {
    case REALTIME:
      RealTimeDisplay();

      if(button1Release()){
        currentState = REALTIMEADJUSTMENT;
        button1Pressed = false;
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
  
    case INCREMENT:
      Serial.println("button2");
      button2Pressed = false;
      currentState = REALTIME;
    break;
    
    default:
      break;
  }
  
}

