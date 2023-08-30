#include "5641AS.h"

#define Duration 5

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
    //setting segment array connected port as OUTPUT mode and LOW
    for (int i = 0; i < 8; i++){
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
OBJECTIVE: Display a number
Parameters:
    pos: the position of the LED
    num: the actual number will be displayed
*/
void display(int pos, int num){
    
    digitalWrite(Control_Pin_Array[(pos-1)], LOW);
    
    for(int i = 0; i < 8; i ++){
        digitalWrite(Segment_Array[i], Number_Array[num][i]);
    }
}

void LED_Clear(){
    for (int i = 0; i < 4; i++){
        pinMode(Control_Pin_Array[i] ,OUTPUT);
        digitalWrite(Control_Pin_Array[i], HIGH);
    }
}

void LED_on(){
    display(1,1);
    delay(Duration);
    LED_Clear();

    display(2,6);
    delay(Duration);
    LED_Clear();

    display(3,4);
    delay(Duration);
    LED_Clear();

    display(4,8);
    delay(Duration);
    LED_Clear();
}

