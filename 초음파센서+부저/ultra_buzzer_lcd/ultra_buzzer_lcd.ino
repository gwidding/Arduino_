// LCD를 쉽게 제어하기 위한 라이브러리를 추가합니다.
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#define TRIG 9 //TRIG 핀 설정 (초음파 보내는 핀)
#define ECHO 8 //ECHO 핀 설정 (초음파 받는 핀)
#include "pitches.h"
#include <stdio.h>


int melody[] = {
  NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_A4, NOTE_B4, NOTE_C5
};

// 0x3F I2C 주소를 가지고 있는 16x2 LCD객체를 생성합니다.(I2C 주소는 LCD에 맞게 수정해야 합니다.)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// 실행시 가장 먼저 호출되는 함수이며, 최초 1회만 실행됩니다.
// 변수를 선언하거나 초기화를 위한 코드를 포함합니다.
void setup() {
  
	// I2C LCD를 초기화 합니다..
	lcd.init();
	// I2C LCD의 백라이트를 켜줍니다.
	lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Hi, OK ~");

  delay(1000);
  lcd.clear();
  Serial.begin(9600); //PC모니터로 센서값을 확인하기위해서 시리얼 통신을 정의해줍니다. 
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
}

// setup() 함수가 호출된 이후, loop() 함수가 호출되며,
// 블록 안의 코드를 무한히 반복 실행됩니다.
void loop() { 
  long duration, distance;
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  duration = pulseIn (ECHO, HIGH);
  distance = duration * 17 / 1000; 

  char buffer[100];

  lcd.setCursor(0,0);
  lcd.print("Distance : ");
  lcd.setCursor(0,1);
  int a = sprintf(buffer, "%d cm", distance);
  lcd.print(buffer);
  // lcd.print(distance);
  // lcd.print(" cm");
  
  if (distance <= 5)
  {
    tone(12, melody[0],500);
    delay(400);
    
  }
  else if(distance >5 && distance <=10){
    tone(12, melody[1],500);
    delay(400);
  }
    else if(distance >10 && distance <=20)
  {
    tone(12, melody[2],500);
    delay(400);
  }
    else if(distance >20 && distance <=30)
  {
    tone(12, melody[3],500);
    delay(400);
  }
    else if(distance >30 && distance <=50)
  {
    tone(12, melody[4],500);
    delay(400);
  }
    else if(distance >50 && distance <=70)
  {
    tone(12, melody[5],500);
    delay(400);
  }
    else if(distance >70 && distance <=90)
  {
    tone(12, melody[6],500);
    delay(400);
  }
  else
  {
    tone(12, melody[7],500);
    delay(400);
  }
	delay(1000);
	// LCD의 모든 내용을 삭제합니다.
	lcd.clear();
}