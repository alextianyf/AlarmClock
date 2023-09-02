#include <Arduino.h>
#include "alarmAndPushbutton.h"

const int pushButton1 = 2;
const int pushButton2 = 3;

volatile bool button1Pressed = false;
volatile bool button2Pressed = false;

volatile unsigned long lastDebounceTime1 = 0;
volatile unsigned long lastDebounceTime2 = 0;
const unsigned long debounceDelay = 150; // Adjust as needed

void Pin2Interrupt(){
  //button1Pressed = true;
  //Serial.println("test");
  if (millis() - lastDebounceTime1 > debounceDelay) {
    button1Pressed = true;
  }
  lastDebounceTime1 = millis();

  // for (int i = 0; i < 4; i++) {
  //   digitalWrite(Control_Pin_Array[i], LOW); // Activate the current digit
  //   display_single(i, alarmDigits[i]); // Display the current digit
    
  //   //delay(1); // Optional - A brief delay to reduce flicker
    
  //   LED_Clear();//Clear all the segment
    
  //   digitalWrite(Control_Pin_Array[i], HIGH); // Deactivate the current digit
  // }
}

void Pin3Interrupt(){
  //button1Pressed = true;
  //Serial.println("test");
  if (millis() - lastDebounceTime2 > debounceDelay) {
    button2Pressed = true;
  }
  lastDebounceTime2 = millis();

  // for (int i = 0; i < 4; i++) {
  //   digitalWrite(Control_Pin_Array[i], LOW); // Activate the current digit
  //   display_single(i, alarmDigits[i]); // Display the current digit
    
  //   //delay(1); // Optional - A brief delay to reduce flicker
    
  //   LED_Clear();//Clear all the segment
    
  //   digitalWrite(Control_Pin_Array[i], HIGH); // Deactivate the current digit
  // }
}

void alarm_Init(){
    pinMode(pushButton1, INPUT);
    pinMode(pushButton2, INPUT);

    attachInterrupt(digitalPinToInterrupt(pushButton1), Pin2Interrupt, RISING);
    attachInterrupt(digitalPinToInterrupt(pushButton2), Pin3Interrupt, RISING);
}