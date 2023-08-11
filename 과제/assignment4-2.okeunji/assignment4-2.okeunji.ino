//시리얼 통신으로 1~5의 숫자가 들어오면 그에 해당하는 임의의 각도로 서보모터를 회전
// 문자 'a'가 입력되면 가스 센서의 현재 값을 응답함 
#include <Servo.h>
Servo mysc; //Servo 클래스 사용

int sensorValue = 0;

void setup() {
  Serial.begin(9600);


  mysc.attach(8); 
}

void loop() {
  int input;
  if(Serial.available())
  {
    input = Serial.read();
    if (input == '1') mysc.write(0);
    else if (input == '2') mysc.write(45);
    else if (input == '3') mysc.write(90);
    else if (input == '4') mysc.write(135);
    else if (input == '5') mysc.write(180);
    else if (input == 'a') 
    {
      sensorValue = analogRead(A0);
      Serial.print("가스 : ");
      Serial.println(sensorValue);
    }
  }
}
