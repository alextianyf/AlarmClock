#include <Arduino.h>
#include "alarmAndPushbutton.h"
#include "5641AS.h"

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

int alarmDigits[4] = {0,0,0,0};

bool button1Release(){
  if(button1Pressed && digitalRead(pushButton1) == LOW){
    return true;
  }
  return false;
}

void Pin2Interrupt(){
  if (millis() - currentMillis > debounceDelay) {
    button1Pressed = true;
  }
  currentMillis = millis();
  // Serial.print("Pin2: ");
  // Serial.println(currentMillis);
}

void Pin3Interrupt(){
  if (millis() - currentMillis > debounceDelay) {
    button2Pressed = true;
  }
  currentMillis = millis();
  // Serial.print("Pin3: ");
  // Serial.println(currentMillis);
}

void alarm_Init(){
    pinMode(pushButton1, INPUT);
    pinMode(pushButton2, INPUT);

    attachInterrupt(digitalPinToInterrupt(pushButton1), Pin2Interrupt, RISING);
    attachInterrupt(digitalPinToInterrupt(pushButton2), Pin3Interrupt, RISING);
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

