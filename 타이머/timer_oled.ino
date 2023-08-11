//버튼 누르면 주기 바뀌기
#include <TimerOne.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

volatile int cnt = 0;
volatile int state = true;
const int bottonPin = 2;

void setup() {
  Serial.begin(9600);
  Timer1.initialize(100000);
  
  //attachInterrupt(2, bott, FALLING);
  //Timer1.attachInterrupt(plus);
  Timer1.attachInterrupt(bott);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  pinMode(bottonPin, INPUT);
}

void loop() {
  display.clearDisplay();
  display.setCursor(0,0);
  display.print("count:"); 
  display.setCursor(80, 0);
  display.print(cnt);
  display.display();
  
  plus();
}

void plus() {
    cnt += 1;
  if (cnt > 1000) cnt = 0; 
}

void bott(){
  state = digitalRead(bottonPin);
  if (!state) {
    Timer1.setPeriod(10000000);
    Timer1.restart();
  }
  else {
    //Serial.println(state);
    Timer1.resume();
  } 
}

