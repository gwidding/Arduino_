//스마트폰으로부터 서보모터 각도 명령을 전달받아, 서보모터 회전 + OLED에 값 출력
#include <SoftwareSerial.h>
SoftwareSerial mySerial(2,3); //Tx, Rx핀
#include <Servo.h>
Servo mysv;
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(9600);
  Serial.println("HM - 10 BLE");
  mySerial.begin(9600); 

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();
  mysv.attach(8);
}

void loop() {
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.clearDisplay();
  
  display.setCursor(0,0);
  display.print("Servoangle");
  display.setCursor(0,20);
  display.print("degree: ");
  display.setCursor(80,20);

  if (mySerial.available()) { 
    long angle = mySerial.parseInt();
    angle %= 181;
    mysv.write(angle);
    display.print(angle);
    display.display();
  }
}
