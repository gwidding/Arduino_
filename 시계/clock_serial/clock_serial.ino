#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include "Arduino.h"
#include <EEPROM.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

bool setMode = false;
int hour = 14;
int minute = 0;
int second = 0;

void setup() {
  Wire.begin();
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);
}
void loop() {
  if (hour == 24) {
    hour = 0;
  }
  if (second == 59) {
    minute++;
    second = 0;
  }
  if (minute == 59) {
    hour++;
    minute = 0;
  }
  char input;
  if (Serial.available()) {
    input = Serial.read();
    Serial.print("input = ");
    Serial.println(input);

    if (input == 'a'){
      setMode = true;
    }
  }

  if (setMode) {
    settingTime(hour, minute, second, input); 
  }
  else {
    printTime();
    second++;
    delay(1000);
  }

  lcd.clear();
}


void ampm_check (int &hour) {
  if (hour >= 12)
    lcd.print("PM ");
  else
    lcd.print("AM ");
  hour = hour % 24;
}

void printZero(int time) {
  if (time < 10) {
    lcd.print("0");
  }
  lcd.print(time);
}

void settingTime(int &hour, int &minute, int &second, char  input) {
  Serial.println("시간 설정 모드");
  static unsigned long blinkMillis = 0;
  unsigned long currentMillis = millis();

  if (Serial.available()){
    if(input == 'q') {
      hour++;
      twentyfourMax(hour);
    }
    else if (input == 'w') {
      hour--;
      twentyfourMax(hour);
    }
    else if (input == 'e') {
      minute++;
      sixtyMax(minute);
    }
    else if (input == 'r') {
      minute--;
      sixtyMax(minute);
    } 
    else if (input == 't') {
      setMode = false;
      Serial.println("시간 모드 끝");
    }
    
  }
  printTime();  
}

void twentyfourMax(int &hour) {
  if (hour < 0) {
    hour = 23;
  }
  else if (hour > 23) {
    hour = 0;
  }
}

void sixtyMax(int &time) {
  if (time < 0) {
    time = 59;
  }
  else if (time > 59) {
    time = 0;
  }
}

void printTime() {
  lcd.setCursor(0, 1);
  ampm_check(hour);
  printZero(hour);
  lcd.print(":");
  printZero(minute);
  lcd.print(":");
  printZero(second);
}
