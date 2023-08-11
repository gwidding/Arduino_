#include <Servo.h>
Servo mysv; //Servo 클래스 사용


int sensorValue = 0;
int outputValue = 0;

void setup() {
  Serial.begin(9600);

  mysv.attach(8); //8번 핀으로 초기화
}

void loop() {
  sensorValue = analogRead(0);
  outputValue = map(sensorValue, 0, 1023, 0, 255);
  analogWrite(9, outputValue);

  mysv.write(map(analogRead(A0), 0, 1023, 0, 90));
  
  Serial.print("sensor = ");
  Serial.print(sensorValue);
  Serial.print("\t output = ");
  Serial.println(outputValue);

  delay(100);

}
