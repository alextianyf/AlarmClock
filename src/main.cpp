#include <Arduino.h>
#include "5641AS.h"
#include <TimeLib.h>

#define Seg_on HIGH
#define D_control_on LOW

int currentHour = hour();
int currentMinute = minute();


void setup() {
  Serial.begin(9600);
  LED_Init();
}

void loop() {
  displayTime();
}

