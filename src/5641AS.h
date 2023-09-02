#ifndef _5641AS_H
#define _5641AS_H

#include <Arduino.h>

void LED_Init();
void display_single(int pos, int num);
void display();
void LED_Clear();
void LED_on();
void displayTime();
void alarm_Init();

//volatile extern bool button2Pressed = false;

#endif