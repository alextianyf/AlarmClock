#include "5641AS.h"
#include <TimeLib.h>
#include "alarmAndPushbutton.h"

unsigned long RealTimelastLEDChange = 0;
int currentRealTimeDisplayPosition = 0;
const int RealTimeUpdateInterval = 500;
int realTimeCounter = 0;

//use to store LED displayed number
int digits[4] = {0,0,0,0};

//Segment Pin number
int A = 7;
int B = 8;
int C = 9;
int D = 10;
int E = 11;
int F = 12;
int G = 13;

int Segment_Array[7] = {A, B, C, D, E, F, G};

//LED Control Pin number
int D1 = 14;//A0
int D2 = 15;//A1
int D3 = 16;//A2
int D4 = 17;//A3

int Control_Pin_Array[4] = {D1, D2, D3, D4};

int Number_Array[10][7] = {
   //A  B  C  D  E  F  G
    {1, 1, 1, 1, 1, 1, 0},//NUMBER 0
    {0, 1, 1, 0, 0, 0, 0},//NUMBER 1
    {1, 1, 0, 1, 1, 0, 1},//NUMBER 2
    {1, 1, 1, 1, 0, 0, 1},//NUMBER 3
    {0, 1, 1, 0, 0, 1, 1},//NUMBER 4
    {1, 0, 1, 1, 0, 1, 1},//NUMBER 5
    {1, 0, 1, 1, 1, 1, 1},//NUMBER 6
    {1, 1, 1, 0, 0, 0, 0},//NUMBER 7
    {1, 1, 1, 1, 1, 1, 1},//NUMBER 8
    {1, 1, 1, 1, 0, 1, 1},//NUMBER 9
};

void LED_Init(){
  for(int i = 0; i < 4; i++){
    digits[i] = 0;
  }
  //setting segment array connected port as OUTPUT mode and LOW
  for (int i = 0; i < 7; i++){
    pinMode(Segment_Array[i] ,OUTPUT);
    digitalWrite(Segment_Array[i], LOW);
  }
    
  //setting LED control connect port as OUTPUT mode and HIGH
  for (int i = 0; i < 4; i++){
    pinMode(Control_Pin_Array[i] ,OUTPUT);
    digitalWrite(Control_Pin_Array[i], HIGH);
  }
}

/*
OBJECTIVE: Display number on a designated position
Parameters:
    pos: the position of the LED
    num: the actual number will be displayed
*/
void display_single(int pos, int num) {
  for (int i = 0; i < 7; i++) {
    digitalWrite(Segment_Array[i], Number_Array[num][i]);
  }
}

void LED_Clear(){
  for (int i = 0; i < 7; i++) {
    digitalWrite(Segment_Array[i], LOW);
  }
}

void time_on(){
  digits[0] = minute() / 10;
  digits[1] = minute() % 10;
  digits[2] = second() / 10;
  digits[3] = second() % 10;
}

void RealTimeDisplay() {
  for (int i = 0; i < 4; i++) {
    digitalWrite(Control_Pin_Array[i], LOW); // Activate the current digit
    display_single(i, digits[i]); // Display the current digit
    
    //delay(1); // Optional - A brief delay to reduce flicker
    
    LED_Clear();//Clear all the segment
    
    digitalWrite(Control_Pin_Array[i], HIGH); // Deactivate the current digit
  }
}


void RealTimeaAdjustment(){

  unsigned long realTime = millis();
  if (realTime - RealTimelastLEDChange > RealTimeUpdateInterval) {
    realTimeCounter ++;
    RealTimelastLEDChange = millis();
  }

  bool displayDigits = ((realTimeCounter % 2) == 0);

  
  // if(displayDigits){
  //   for (int i = 0; i < 4; i++) {
      

  //     if( i == currentRealTimeDisplayPosition){
  //       digitalWrite(Control_Pin_Array[i], LOW);
  //       display_single(i,digits[i]);
  //     }else{
  //       LED_Clear();
  //     }
  //     digitalWrite(Control_Pin_Array[i], HIGH);
  //   }
  // }


}
  // if(displayDigits){
  //   digitalWrite(Control_Pin_Array[currentRealTimeDisplayPosition], HIGH);
  // }else{
    
  //   for (int i = 0; i < 4; i++) {
  //     if(i!=currentRealTimeDisplayPosition){
  //       digitalWrite(Control_Pin_Array[i], LOW);
  //     }
  //   }
  // }

  // for(int i = 0; i < 4; i++){
  //   digitalWrite(Control_Pin_Array[i], LOW);
  //   if (displayDigits) {
  //     display_single(i, digits[i]);
  //     LED_Clear();
  //     //RealTimeDisplay();
  //   }
  //   else{
  //     LED_Clear();
  //     for (int i = 0; i < 4; i++) {
  //       if (i != currentRealTimeDisplayPosition){
  //         digitalWrite(Control_Pin_Array[i], LOW); // Activate the current digit
  //         display_single(i, digits[i]); // Display the current digit
  //       }
  //       //delay(1); // Optional - A brief delay to reduce flicker
        
  //       LED_Clear();//Clear all the segment
        
  //       digitalWrite(Control_Pin_Array[i], HIGH); // Deactivate the current digit
  //     }

  //     // if( i == currentRealTimeDisplayPosition){
  //     //   LED_Clear();
  //     // }
  //     // else{
  //     //   display_single(i, digits[i]);
  //     // }
  //   }
  //   LED_Clear();
  //   digitalWrite(Control_Pin_Array[i], HIGH);
  // }

//}

// void Pin2Interrupt(){
//   //button1Pressed = true;
//   //Serial.println("test");
//   if (millis() - lastDebounceTime1 > debounceDelay) {
//     button1Pressed = true;
//   }
//   lastDebounceTime1 = millis();

//   // for (int i = 0; i < 4; i++) {
//   //   digitalWrite(Control_Pin_Array[i], LOW); // Activate the current digit
//   //   display_single(i, alarmDigits[i]); // Display the current digit
    
//   //   //delay(1); // Optional - A brief delay to reduce flicker
    
//   //   LED_Clear();//Clear all the segment
    
//   //   digitalWrite(Control_Pin_Array[i], HIGH); // Deactivate the current digit
//   // }
// }

