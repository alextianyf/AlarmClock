#include <Arduino.h>
#include "5641AS.h"
#include <TimeLib.h>
#include "alarmAndPushbutton.h"

#define Seg_on HIGH
#define D_control_on LOW

int currentHour = hour();
int currentMinute = minute();


void setup() {
  Serial.begin(9600);
  LED_Init();
  alarm_Init();
}

void loop() {
  //LOW detection when finger release the switch
  if(button1Pressed && digitalRead(pushButton1) == LOW){
    Serial.println("button1");
    button1Pressed = false;
  }
  
  if(button2Pressed && digitalRead(pushButton2) == LOW){
    Serial.println("button2");
    button2Pressed = false;
  }
  //displayTime();
}

