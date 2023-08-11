#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif

#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
//U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
#define MQ2pin A0
#define RED 1
#define BLUE 2

float sensor;
const int redPin = 11; 

void setup(void) 
{
  u8g2.begin();
  u8g2.enableUTF8Print();

  pinMode(redPin, OUTPUT);
  ledColor(BLUE);
}

void loop(void) 
{
  sensor = analogRead(MQ2pin);
  u8g2.setFont(u8g2_font_unifont_t_korean1);  
  u8g2.setFontDirection(0);
  u8g2.firstPage();
  do 
  {
    u8g2.setCursor(0, 15);
    u8g2.print("가스 : ");
    u8g2.print(sensor);
    u8g2.setCursor(0, 40);
    Serial.print(sensor);

    if (sensor >= 500){
      u8g2.print("가스 상태 위험!");
      ledColor(RED);
    }
    else {
      u8g2.print("가스 상태 양호");
      ledColor(BLUE);
    }

  } while ( u8g2.nextPage() );
  delay(1000);
}

void ledColor