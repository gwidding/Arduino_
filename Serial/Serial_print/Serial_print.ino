

void setup() {
  Serial.begin(9600);

  Serial.println(78); // 10진수
  Serial.println(78, BIN); // 2진수
  Serial.println(78, HEX); // 16진수
  Serial.println(1.23456, 0); // 소수점 0자리 남김
  Serial.println(1.23456, 2); // 소수점 2자리
  Serial.println(1.23456, 4); // 소수점 4자리
}



void loop() {
}
