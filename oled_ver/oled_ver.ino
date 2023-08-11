//시리얼 통신으로 들어온 문자를 OLED에 출력
#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif

#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);


void setup() {
  Serial.begin(9600);
  u8g2.begin();
  u8g2.enableUTF8Print();
}

void loop() {
  u8g2.setFont();
  u8g2.setFontDirection(0);
  u8g2.firstPage();

  char string[100];
  char tmp;
  do {
    if(Serial.available()){
      byte leng = Serial.readBytes(string, 100);
      Serial.print("length : ");
      Serial.println(leng);
      tmp = Serial.write(string, leng);

      u8g2.setCursor(0, 15);
      u8g2.print("string : ");
      u8g2.setCursor(0, 60);
      u8g2.print(tmp);
      Serial.println();

    }   
  } while (u8g2.nextPage());
  delay(1000);
}

