#ifndef _ALARMANDPUSHBUTTON_H
#define _ALARMANDPUSHBUTTON_H

#include <Arduino.h>

extern const int pushButton1;
extern const int pushButton2;

extern volatile bool button1Pressed;
extern volatile bool button2Pressed;

extern volatile unsigned long lastDebounceTime1;
extern volatile unsigned long lastDebounceTime2;
extern const unsigned long debounceDelay; // Adjust as needed
extern int currentDisplayPosition;

void alarm_Init();
void alarmTimeDisplay();
bool button1Release();

#endif