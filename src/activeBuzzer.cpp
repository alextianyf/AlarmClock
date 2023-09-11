#include "activeBuzzer.h"
#include "alarmAndPushbutton.h"

#define buzzer 6

unsigned long LastBuzzerOnTime = 0;
const int beepDuration = 500;
const int silenceDuration = 2000;
int beepCount = 0;
bool BuzzerState = false;

/**
 * OBJECTIVE: init buzzer
 * PARAMETERS: void
 * RETURN: void
 * NOTE: avoid
 */
void buzzer_init(){
    pinMode(buzzer, OUTPUT);
    digitalWrite(buzzer, LOW);
}

/**
 * OBJECTIVE: turn on buzzer every half second
 * PARAMETERS: void
 * RETURN: void
 * NOTE: void
 */
void buzzer_on(){
    unsigned long realTime = millis();
    if (realTime - LastBuzzerOnTime > beepDuration) {
        LastBuzzerOnTime = millis();
        BuzzerState = !BuzzerState;//toggle buzzer on/off
        digitalWrite(buzzer, BuzzerState);
    }
}

/**
 * OBJECTIVE: turn off buzzer
 * PARAMETERS: void
 * RETURN: void
 * NOTE: void
 */
void buzzer_off(){
    digitalWrite(buzzer, LOW);
    BuzzerState = false;
}
