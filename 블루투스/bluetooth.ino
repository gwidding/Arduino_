#include <SoftwareSerial.h>

SoftwareSerial mySerial(2,3); //블루투스의 Tx, Rx핀을 2,3번 핀으로 설정

void setup() {
  Serial.begin(9600);
  Serial.println("HM - 10 BLE");
  mySerial.begin(9600); //블루투스의 시리얼 통신속도 설정
  pinMode(13, OUTPUT);
}

void loop() {
  byte input = 0;
  if (mySerial.available()) { //블루투스에서 넘어온 데이터가 있다면
    input = mySerial.read(); //그 데이터 출력
    Serial.write(input);
  }
  
  if (Serial.available()) { //시리얼 모니터에 입력된 데이터가 있다면
    mySerial.write(Serial.read()); //블루투스 통해 그 데이터 전달
  }
}
