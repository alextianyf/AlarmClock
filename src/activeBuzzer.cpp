#include "activeBuzzer.h"
#include "alarmAndPushbutton.h"

#define buzzer 6
unsigned long LastBuzzerOnTime = 0;
const int beepDuration = 500;
const int silenceDuration = 2000;
int beepCount = 0;
bool BuzzerState = false;

void buzzer_init(){
    pinMode(buzzer, OUTPUT);
    digitalWrite(buzzer, LOW);
}

void buzzer_on(){
    unsigned long realTime = millis();
    if (realTime - LastBuzzerOnTime > beepDuration) {
        LastBuzzerOnTime = millis();
        BuzzerState = !BuzzerState;
        digitalWrite(buzzer, BuzzerState);
    }
}

void buzzer_off(){
    digitalWrite(buzzer, LOW);
    BuzzerState = false;
}

void alarmCheck(){
  //if(alarm_triggered && alarmActive){
    buzzer_on();
    if(button1Release()){
        alarmActive = false;
        buzzer_off();
    }
  //}
}