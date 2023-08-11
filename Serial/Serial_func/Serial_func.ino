void setup() {
  Serial.begin(9600);
  // Serial.println("Serial Test Start");
  // char test[5] = {'K', 'O', 'R', 'E', 'A'};
  // Serial.write(33); //아스키코드 33의 문자인 !출력
  // Serial.print(33); //문자열 33출력
  // Serial.write('a'); //a출력
  // Serial.print('a'); //a출력
  // Serial.write("hello"); //hello출력
  // Serial.write(test, 3); //test에서 5개 출력
}

void loop() {
  char string[80];
  if(Serial.available()) //1234, a, 1000 차례로 입력했을 때. a는 0
  {
    //int indata = Serial.read();
    //unsigned char tmp = Serial.read();
    //Serial.println(tmp);

  //  long value = Serial.parseInt();
  // float value = Serial.parseFloat();
  // Serial.print("value = ");
  // Serial.println(value);
  //  byte leng = Serial.readBytes(string, 80);
  byte leng = Serial.readBytesUntil('e', string, 80);
    Serial.print("Input data Length : ");
    Serial.println(leng);
    Serial.write(string, leng);
    Serial.println();
  
  }

}
