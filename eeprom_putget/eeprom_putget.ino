#include <EEPROM.h>

void setup() {
 float a = 3.14, b=1234.1234;
 float value;

  Serial.begin(9600);
  EEPROM.put(0,a);
  EEPROM.put(sizeof(float),b); // float가 4바이트이므로 주소 1에다 적으면x
  //Serial.print(sizeof(float));

  EEPROM.get(0, value);

  // 사이즈가 크면 put, get 슴
  Serial.print(value);
}

void loop() {
  // put your main code here, to run repeatedly:

}
