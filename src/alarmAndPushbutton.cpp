#include <Arduino.h>
#include "alarmAndPushbutton.h"
#include "5641AS.h"
#include <TimeLib.h>

const int pushButton1 = 2;
const int pushButton2 = 3;

int counter = 0;
volatile bool button1Pressed = false;
volatile bool button2Pressed = false;

volatile unsigned long currentMillis = 0;
volatile unsigned long lastDebounceTime1 = 0;
//volatile unsigned long lastDebounceTime2 = 0;
const unsigned long debounceDelay = 150; // Adjust as needed
unsigned long lastLEDChange = 0;

int currentDisplayPosition = 0;
const int segmentUpdateInterval = 500;

int alarmDigits[4] = {0,0,0,1};

bool alarmActive = true;

/*
OBJECTIVE: Detect if Intefacing button is being pressed
PARAMETERS: void
RETURN:
  Boolean type
*/
bool button1Release(){
  if(button1Pressed && digitalRead(pushButton1) == LOW){
    return true;
  }
  return false;
}

bool button2Release(){
  if(button2Pressed && digitalRead(pushButton2) == LOW){
    return true;
  }
  return false;
}

void Pin2Interrupt(){
  if (millis() - currentMillis > debounceDelay) {
    button1Pressed = true;
  }
  currentMillis = millis();
}

void Pin3Interrupt(){
  if (millis() - currentMillis > debounceDelay) {
    button2Pressed = true;
  }
  currentMillis = millis();
}

void alarm_Init(){
    pinMode(pushButton1, INPUT);
    pinMode(pushButton2, INPUT);

    attachInterrupt(digitalPinToInterrupt(pushButton1), Pin2Interrupt, RISING);
    attachInterrupt(digitalPinToInterrupt(pushButton2), Pin3Interrupt, RISING);

    alarmActive = true;
}

void alarmTimeDisplay(){

  unsigned long time = millis();
  if (time - lastLEDChange > segmentUpdateInterval) {
    counter ++;
    lastLEDChange = millis();
  }

  bool displayDigits = ((counter % 2) == 0);

  for(int i = 0; i < 4; i++){
    digitalWrite(Control_Pin_Array[i], LOW);
    if (displayDigits) {
      display_single(i, alarmDigits[i]);
    }
    else{
      if( i != currentDisplayPosition){
        display_single(i, alarmDigits[i]);
      }
    }
    LED_Clear();
    digitalWrite(Control_Pin_Array[i], HIGH);
  }

}

void AlarmTimeInc() {
  alarmDigits[currentDisplayPosition]++;
  if(alarmDigits[0]>=3){
    alarmDigits[0] = 0;
  }
  if(alarmDigits[1]>=10){
    alarmDigits[1] = 0;
  }
  if(alarmDigits[2]>=6){
    alarmDigits[2] = 0;
  }
  if(alarmDigits[3]>=9){
    alarmDigits[3] = 0;
  }
}

bool alarm_triggered(){
  if(alarmActive){
    int hour10 = hour() / 10;
    int hour01 = hour() % 10;
    int min10 = minute() / 10;
    int min01 = minute() % 10;
    if(alarmDigits[0] == hour10 && alarmDigits[1] == hour01 && alarmDigits[2] == min10 && alarmDigits[3] == min01){
      return true;
    }
    return false;
  }
  return false;
}

void alarmResume(){

  int temp = alarmDigits[3] + 1;
  if(alarmActive == false && temp == digits[3]){
    alarmActive = true;
  }
}