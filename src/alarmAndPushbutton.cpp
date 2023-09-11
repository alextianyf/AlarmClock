#include <Arduino.h>
#include "alarmAndPushbutton.h"
#include "5641AS.h"
#include <TimeLib.h>

const int pushButton1 = 2;//first(interfacing) button is assigned to pin2
const int pushButton2 = 3;//second(increment) button is assigned to pin3
volatile bool button1Pressed = false;//button1 flag
volatile bool button2Pressed = false;//button2 flag

volatile unsigned long currentMillis = 0;
const unsigned long debounceDelay = 150; // Adjust as needed

unsigned long lastLEDChange = 0;
const int segmentUpdateInterval = 500;
int counter = 0;//used to track and flash the alarm triggering time
int currentDisplayPosition = 0;

int alarmDigits[4] = {0,7,0,0};
int alarmbuffer[4] = {0,0,0,0};//used to store alarm trigger time

bool alarmActive = true;//We want alarm stay active when it first initialize

unsigned long AlarmLastResetTime = 0;
unsigned long AlarmResetDuration = 1000;


/**
 * OBJECTIVE: Detect if First button(Intefacing button) is being pressed
 * PARAMETERS: void
 * RETURN:
      Boolean type
 * NOTE: function is designed to detect when after button is released, not pressed
 */
bool button1Release(){
  //debouncing with button1Pressed flag
  if(button1Pressed && digitalRead(pushButton1) == LOW){
    return true;
  }
  return false;
}

/**
 * OBJECTIVE: Detect if Second button(Increment button) is being pressed
 * PARAMETERS: void
 * RETURN:
      Boolean type
 * NOTE: function is designed to detect when after button is released, not pressed
 */
bool button2Release(){
  //debouncing with button2Pressed flag
  if(button2Pressed && digitalRead(pushButton2) == LOW){
    return true;
  }
  return false;
}

/**
 * OBJECTIVE: debouncing delay
 * PARAMETERS: void
 * RETURN: void
 * NOTE: pin2 ISR
 */
void Pin2Interrupt(){
  if (millis() - currentMillis > debounceDelay) {
    button1Pressed = true;
  }
  currentMillis = millis();
}

/**
 * OBJECTIVE: debouncing delay
 * PARAMETERS: void
 * RETURN: void
 * NOTE: pin3 ISR
 */
void Pin3Interrupt(){
  if (millis() - currentMillis > debounceDelay) {
    button2Pressed = true;
  }
  currentMillis = millis();
}

/**
 * OBJECTIVE: initialize two buttons 
 * PARAMETERS: void
 * RETURN: void
 * NOTE: interrupt will be triggered at RISING edge. 
       two buttons are connected with pulldown resistor
 */
void btn_Init(){
    pinMode(pushButton1, INPUT);
    pinMode(pushButton2, INPUT);

    //trigger interrupt at rising edge
    attachInterrupt(digitalPinToInterrupt(pushButton1), Pin2Interrupt, RISING);
    attachInterrupt(digitalPinToInterrupt(pushButton2), Pin3Interrupt, RISING);
}

/**
 * OBJECTIVE: display the alarm trigger time and flash the selected position
 * PARAMETERS: void
 * RETURN: void
 * NOTE: flash the selected position according to currentDisplayPosition
 */
void alarmTimeDisplay(){
  //flashing duration every half second
  unsigned long time = millis();
  if (time - lastLEDChange > segmentUpdateInterval) {
    counter ++;
    lastLEDChange = millis();
  }

  //counter is used to be tracked every half second 
  bool displayDigits = ((counter % 2) == 0);

  //scan the 4 LED positions, and skip(flash) the selected position
  for(int i = 0; i < 4; i++){
    if (displayDigits) {
      display_single(i, alarmDigits[i]);
    }
    else{
      if( i != currentDisplayPosition){
        display_single(i, alarmDigits[i]);
      }
    }
    LED_Clear();
    digitalWrite(Control_Pin_Array[i], HIGH);
  }
}

/**
 * OBJECTIVE: set/increment the alarm trigger time
 * PARAMETERS: void
 * RETURN: void
 * NOTE: void
 */
void AlarmTimeInc() {
  alarmDigits[currentDisplayPosition]++;
    //check if the value is overflowed after increment
  if(alarmDigits[0]>=3){
    alarmDigits[0] = 0;
  }
  if(alarmDigits[1]>=10){
    alarmDigits[1] = 0;
  }
  if(alarmDigits[2]>=6){
    alarmDigits[2] = 0;
  }
  if(alarmDigits[3]>=9){
    alarmDigits[3] = 0;
  }
}

/**
 * OBJECTIVE: check if alarm is trigger
 * PARAMETERS: void
 * RETURN: 
      boolean type
 * NOTE: alarm trigger time need to be stored in the buffer in order to be operated in the future
 */
bool alarm_triggered(){
  //only will be trigger is alarm is actived
  if(alarmActive){
    int hour10 = hour() / 10;
    int hour01 = hour() % 10;
    int min10 = minute() / 10;
    int min01 = minute() % 10;
    if(alarmDigits[0] == hour10 && alarmDigits[1] == hour01 && alarmDigits[2] == min10 && alarmDigits[3] == min01){
      for(int i = 0; i < 4; i++){
        //buff the alarm triggering time
        alarmbuffer[i] = alarmDigits[i];
      }
      return true;
    }
    return false;
  }
  return false;
  
}

/**
 * OBJECTIVE: resume the alarmActive flag
 * PARAMETERS: void
 * RETURN: 
      boolean type
 * NOTE: alarmActive flag will be resume at the next minute once alarm is triggered
 */
bool alarmResume(){
  //only resume if alarm is not actived
  if(!alarmActive){
    int hour10 = alarmbuffer[0];
    int hour01 = alarmbuffer[1];
    int min10 = alarmbuffer[2];
    int min01 = alarmbuffer[3];

    //check if the resume time is overflowed
    min01 = min01 + 1;
    if(min01>=9){
      min01 = 0;
      min10 = min10 + 1;
      if(min10>=6){
        min10 = 0;
        hour01 = hour01 + 1;
        if(hour01>=10){
          hour01 = 0;
          hour10 = hour10 + 1;
          if(hour10>=3){
            hour10 = 0;
          }
        }
      }
    }
    
    if(digits[0] == hour10 && digits[1] == hour01 && digits[2] == min10 && digits[3] == min01){
      return true;
    }else{
      return false;
    }
  }
  else{
    return false;
  }

}
