#ifndef _5641AS_H
#define _5641AS_H

#include <Arduino.h>

void LED_Init();
void display_single(int pos, int num);
void display();
void LED_Clear();
void LED_on();
void RealTimeDisplay();
void RealTimeaAdjustment();
void alarm_Init();
void time_on();

//volatile extern bool button2Pressed = false;
extern int Control_Pin_Array[4];
extern int currentRealTimeDisplayPosition;
void RealTimeInc();

#endif