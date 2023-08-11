#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();
}


void loop() {
  char string[100]="";
  if(Serial.available())
  {
    byte leng = Serial.readBytes(string, 100);
    Serial.print("length : ");
    Serial.println(leng-1);

    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);

    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("string : ");
    display.setCursor(0, 20);
    display.print(string);
    display.display();
  }
}
