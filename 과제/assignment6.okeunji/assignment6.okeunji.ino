//임의로 거리 설정 -> 바깥 led만 -> 더 가까워지면 -> 안쪽 led도 켬 -> 최종적으로 다 채워지고 깜박이도록
//부저도 처음 30cm면 -> 간격 있는 소리 -> 가까워지면 -> 소리 빨리
#include <LedControl.h>
LedControl Dote = LedControl(7, 6, 5, 1); 

#define TRIG 9 //(초음파 보내는 핀)
#define ECHO 8 //(초음파 받는 핀)
#include "pitches.h"

#include "dot.h"
int melody[] = {
  NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_A4, NOTE_B4, NOTE_C5
};

void setup() {
  Dote.shutdown(0, false);          // 절전모드로 (첫번째 모듈, 절전모드를 하지 않는다) 설정 
  Dote.setIntensity(0, 1);          // 밝기를 조절 (첫번째 모듈, 1~15까지 원하는 밝기) 설정
  Dote.clearDisplay(0);

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  Serial.begin(9600);
}

int i, j ;
long duration, distance;

void loop() {
  measureDistance(duration, distance);
  dotMatrixDistance();
  
  for (j = 0; j <8; j++) {  
    Dote.setRow(0, j, digits[i][j]);
  }
  sound();
}

void measureDistance(long &dur, long &dist)
{
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  dur = pulseIn (ECHO, HIGH);
  dist = dur * 17 / 1000;

  Serial.print("\nDIstance : ");
  Serial.print(dist);
  Serial.println(" Cm");
  delay(100); 
}

void dotMatrixDistance() 
{
   if (distance <= 5){
    i=4;
    Dote.clearDisplay(0);
    for(int k = 0; k < 20; k++){
      for (j = 0; j <8; j++){  
      Dote.setRow(0, j, digits[0][j]);
      } 
    }  
  }
  else if(distance >5 && distance <=10) i=3;
  else if(distance >10 && distance <=20) i=2;
  else if(distance >20 && distance <=30) i=1;
  else i=0;
}

void sound(){
  tone(12, melody[3],200);
  int k = (5-i) * 100;
  delay(k);
}