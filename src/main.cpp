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
      //Serial.println(currentRealTimeDisplayPosition);
      RealTimeDisplay();

      if(button1Release()){
        currentState = REALTIMEADJUSTMENT;
        button1Pressed = false;
      }
      
      if(button2Pressed && digitalRead(pushButton2) == LOW){
        currentState = INCREMENT;
      }

    break;

    case REALTIMEADJUSTMENT:
      //Serial.println(currentRealTimeDisplayPosition);
      RealTimeaAdjustment();
      
      if(button1Release()){
        currentRealTimeDisplayPosition++;
        button1Pressed = false;
        if(currentRealTimeDisplayPosition >= 4){
          currentRealTimeDisplayPosition = 0;
          currentState = REALTIME;
          button1Pressed = false;
        }
      }

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
    break;
  
    case INCREMENT:
      Serial.println("button2");
      button2Pressed = false;
      currentState = REALTIME;
      break;
    
    default:
      break;
  }
  // //LOW detection when finger release the switch
  // if(button1Pressed && digitalRead(pushButton1) == LOW){
  //   Serial.println("button1");
  //   button1Pressed = false;
  // }
  

  
}

