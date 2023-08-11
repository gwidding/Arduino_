#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif

#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif
int sensor;
U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
//U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

void setup(void) 
{
  u8g2.begin();
  u8g2.enableUTF8Print();    // enable UTF8 support for the Arduino print() function
}

void loop(void) 
{
  sensor = analogRead(A0);
  u8g2.setFont(u8g2_font_unifont_t_korean1);  
  u8g2.setFontDirection(0);
  u8g2.firstPage();
  do 
  {
    u8g2.setCursor(0, 15);
    u8g2.print(sensor);
    u8g2.setCursor(0, 40);
    u8g2.print("안녕 세상");    // Korean "Hello World" 
  } while ( u8g2.nextPage() );
  delay(1000);
}