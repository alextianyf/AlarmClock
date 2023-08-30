#include <Arduino.h>
#include "5641AS.h"

#define Seg_on HIGH
#define D_control_on LOW

void setup() {
  Serial.begin(9600);
  LED_Init();
}

void loop() {
  LED_on();
}

