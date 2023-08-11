//5v를 가운데에
// 온도, 습도 olcd
#include <DFRobot_DHT11.h>
DFRobot_DHT11 DHT;
#define DHT11_PIN A0
const byte ledPin = 13;

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup(){
  Serial.begin(9600);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  
  pinMode(ledPin, OUTPUT);
}

void loop(){
  DHT.read(DHT11_PIN);
  Serial.print("temp:");
  Serial.print(DHT.temperature);
  Serial.print("  humi:");
  Serial.println(DHT.humidity);

  display.clearDisplay();
  display.setCursor(0,0);
  display.print("temp : "); 
  display.println(DHT.temperature);
  display.print("humi : ");
  display.print(DHT.humidity);
  display.display();

  if (DHT.temperature > 30) 
    digitalWrite(ledPin, HIGH);
  else digitalWrite(ledPin, LOW);

  delay(1000);
}
