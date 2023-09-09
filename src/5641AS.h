#ifndef _5641AS_H
#define _5641AS_H

#include <Arduino.h>

extern int digits[4];

void LED_Init();
void display_single(int pos, int num);
void LED_Clear();
void RealTimeDisplay();
void RealTimeFlash();
void alarm_Init();
void time_on();
void RealTimeInc();

extern int Control_Pin_Array[4];
extern int currentRealTimeDisplayPosition;


#endif