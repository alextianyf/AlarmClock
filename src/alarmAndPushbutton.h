#ifndef _ALARMANDPUSHBUTTON_H
#define _ALARMANDPUSHBUTTON_H

#include <Arduino.h>

extern const int pushButton1;
extern const int pushButton2;

extern volatile bool button1Pressed;
extern volatile bool button2Pressed;

extern const unsigned long debounceDelay; 
extern int currentDisplayPosition;

extern bool alarmActive;

void btn_Init();
void alarmTimeDisplay();
bool button1Release();
bool button2Release();
void AlarmTimeInc();
bool alarm_triggered();
bool alarmResume();


#endif