#include <LedControl.h>
LedControl Dote = LedControl(7, 6, 5, 1); 

int LED = 13;                                     // LED 출력
int sensor = 12;                                 // 센서 입력값
int value = 0;                                   // loop에서 사용할 value 변수 설정

byte digits[2][8] = 
{
  {                                        // 숫자 0 
    B00111000,                             // 총 64개의 LED에 원하는 모양의 신호를 준다
    B01000100,
    B01000100,
    B01000100,
    B01000100,
    B01000100,
    B01000100,
    B00111000
  },
  
  {                                                  // 하트
    B00000000,
    B01000100,
    B10101010,
    B10010010,
    B10000010,
    B01000100,
    B00101000,
    B00010000 
  }
};

void setup() 
{
  pinMode (LED, OUTPUT);            // 핀모드 LED 출력으로 설정
  pinMode (sensor, INPUT);          // 핀모드 센서 입력값으로 설정

  Dote.shutdown(0, false);          // 절전모드로 (첫번째 모듈, 절전모드를 하지 않는다) 설정 
  Dote.setIntensity(0, 7);          // 밝기를 조절 (첫번째 모듈, 1~15까지 원하는 밝기) 설정
  Dote.clearDisplay(0);
}



void loop() 
{ 
  int i, j;
  value = digitalRead(sensor);        // 변수 value에 디지털 센서값 저장
  if(value == HIGH)                   // value가 high라면
  {
    digitalWrite(LED, HIGH);          // LED를 켜라
    i = 0;
  }

  else                                // 그렇지 않다면
  {
    digitalWrite(LED, LOW);           //  LED를 꺼라
    i= 1;
  }
  for (j = 0; j <8; j++) {  
    Dote.setRow(0, j, digits[i][j]);
  } 
}