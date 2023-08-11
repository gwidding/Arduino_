#include <TimerOne.h>

int count = 0;
void setup() {
  pinMode(13, OUTPUT);
  Serial.begin(9600);
  Timer1.initialize(10000); //TImer1을 initialize함수 통해 us단위로 인터럽트 주기서정
  Timer1.attachInterrupt(timerIsr); //Timer1을 attachInterrupt함수 통해 인터럽트 핸들러 등록
}

void loop() {
}

void timerIsr(){
  digitalWrite(13, digitalRead(13)^1); //^연산자는 비트 XOR 수행 => 13번의 현재 상태와 반대로 된 값
  //i.e. 디지털 핀 13의 상태를 토글
  count++;
  if(count%100==0){
    Serial.println(count);
  }
}