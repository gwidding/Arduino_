#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <TimerOne.h>
#include "pitches.h"
#include <EEPROM.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
const int xPin = A0;
const int yPin = A1;
const int zPin = 8;

bool setMode = false;
bool alarmMode = false;
bool melodyMode = false;

int hour = 11;
int minute = 0;
int second = 0;
int alarmHour = 12;
int alarmMinute = 0;
int alarmSecond = 0;
unsigned int melodyIndex = 0;

unsigned long lastTime = 0;
unsigned long currentTime = 0;
unsigned long elapsedTime = 0;
const unsigned long interval = 1000; // 1초마다 업데이트

int melody1[] = {
  REST, 2, NOTE_D4, 4,
  NOTE_G4, -4, NOTE_AS4, 8, NOTE_A4, 4,
  NOTE_G4, 2, NOTE_D5, 4,
  NOTE_C5, -2, 
  NOTE_A4, -2,
  NOTE_G4, -4, NOTE_AS4, 8, NOTE_A4, 4,
  NOTE_F4, 2, NOTE_GS4, 4,
  NOTE_D4, -1, 
  NOTE_D4, 4,

  NOTE_G4, -4, NOTE_AS4, 8, NOTE_A4, 4, //10
  NOTE_G4, 2, NOTE_D5, 4,
  NOTE_F5, 2, NOTE_E5, 4,
  NOTE_DS5, 2, NOTE_B4, 4,
  NOTE_DS5, -4, NOTE_D5, 8, NOTE_CS5, 4,
  NOTE_CS4, 2, NOTE_B4, 4,
  NOTE_G4, -1,
  NOTE_AS4, 4,
     
  NOTE_D5, 2, NOTE_AS4, 4,//18
  NOTE_D5, 2, NOTE_AS4, 4,
  NOTE_DS5, 2, NOTE_D5, 4,
  NOTE_CS5, 2, NOTE_A4, 4,
  NOTE_AS4, -4, NOTE_D5, 8, NOTE_CS5, 4,
  NOTE_CS4, 2, NOTE_D4, 4,
  NOTE_D5, -1, 
  REST,4, NOTE_AS4,4,  

  NOTE_D5, 2, NOTE_AS4, 4,//26
  NOTE_D5, 2, NOTE_AS4, 4,
  NOTE_F5, 2, NOTE_E5, 4,
  NOTE_DS5, 2, NOTE_B4, 4,
  NOTE_DS5, -4, NOTE_D5, 8, NOTE_CS5, 4,
  NOTE_CS4, 2, NOTE_AS4, 4,
  NOTE_G4, -1, 
};


int melody2[] = {
  NOTE_C5,4, //1
  NOTE_F5,4, NOTE_F5,8, NOTE_G5,8, NOTE_F5,8, NOTE_E5,8,
  NOTE_D5,4, NOTE_D5,4, NOTE_D5,4,
  NOTE_G5,4, NOTE_G5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F5,8,
  NOTE_E5,4, NOTE_C5,4, NOTE_C5,4,
  NOTE_A5,4, NOTE_A5,8, NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8,
  NOTE_F5,4, NOTE_D5,4, NOTE_C5,8, NOTE_C5,8,
  NOTE_D5,4, NOTE_G5,4, NOTE_E5,4,

  NOTE_F5,2, NOTE_C5,4, //8 
  NOTE_F5,4, NOTE_F5,8, NOTE_G5,8, NOTE_F5,8, NOTE_E5,8,
  NOTE_D5,4, NOTE_D5,4, NOTE_D5,4,
  NOTE_G5,4, NOTE_G5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F5,8,
  NOTE_E5,4, NOTE_C5,4, NOTE_C5,4,
  NOTE_A5,4, NOTE_A5,8, NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8,
  NOTE_F5,4, NOTE_D5,4, NOTE_C5,8, NOTE_C5,8,
  NOTE_D5,4, NOTE_G5,4, NOTE_E5,4,
  NOTE_F5,2, NOTE_C5,4,

  NOTE_F5,4, NOTE_F5,4, NOTE_F5,4,//17
  NOTE_E5,2, NOTE_E5,4,
  NOTE_F5,4, NOTE_E5,4, NOTE_D5,4,
  NOTE_C5,2, NOTE_A5,4,
  NOTE_AS5,4, NOTE_A5,4, NOTE_G5,4,
  NOTE_C6,4, NOTE_C5,4, NOTE_C5,8, NOTE_C5,8,
  NOTE_D5,4, NOTE_G5,4, NOTE_E5,4,
  NOTE_F5,2, NOTE_C5,4, 
  NOTE_F5,4, NOTE_F5,8, NOTE_G5,8, NOTE_F5,8, NOTE_E5,8,
  NOTE_D5,4, NOTE_D5,4, NOTE_D5,4,
  
  NOTE_G5,4, NOTE_G5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F5,8, //27
  NOTE_E5,4, NOTE_C5,4, NOTE_C5,4,
  NOTE_A5,4, NOTE_A5,8, NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8,
  NOTE_F5,4, NOTE_D5,4, NOTE_C5,8, NOTE_C5,8,
  NOTE_D5,4, NOTE_G5,4, NOTE_E5,4,
  NOTE_F5,2, NOTE_C5,4,
  NOTE_F5,4, NOTE_F5,4, NOTE_F5,4,
  NOTE_E5,2, NOTE_E5,4,
  NOTE_F5,4, NOTE_E5,4, NOTE_D5,4,
  
  NOTE_C5,2, NOTE_A5,4,//36
  NOTE_AS5,4, NOTE_A5,4, NOTE_G5,4,
  NOTE_C6,4, NOTE_C5,4, NOTE_C5,8, NOTE_C5,8,
  NOTE_D5,4, NOTE_G5,4, NOTE_E5,4,
  NOTE_F5,2, NOTE_C5,4, 
  NOTE_F5,4, NOTE_F5,8, NOTE_G5,8, NOTE_F5,8, NOTE_E5,8,
  NOTE_D5,4, NOTE_D5,4, NOTE_D5,4,
  NOTE_G5,4, NOTE_G5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F5,8, 
  NOTE_E5,4, NOTE_C5,4, NOTE_C5,4,
  
  NOTE_A5,4, NOTE_A5,8, NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8,//45
  NOTE_F5,4, NOTE_D5,4, NOTE_C5,8, NOTE_C5,8,
  NOTE_D5,4, NOTE_G5,4, NOTE_E5,4,
  NOTE_F5,2, NOTE_C5,4,
  NOTE_F5,4, NOTE_F5,8, NOTE_G5,8, NOTE_F5,8, NOTE_E5,8,
  NOTE_D5,4, NOTE_D5,4, NOTE_D5,4,
  NOTE_G5,4, NOTE_G5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F5,8,
  NOTE_E5,4, NOTE_C5,4, NOTE_C5,4,
  
  NOTE_A5,4, NOTE_A5,8, NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8, //53
  NOTE_F5,4, NOTE_D5,4, NOTE_C5,8, NOTE_C5,8,
  NOTE_D5,4, NOTE_G5,4, NOTE_E5,4,
  NOTE_F5,2, REST,4
};

int* melodyList[] = {melody1, melody2};
int* selectedMelody = melodyList[0];

unsigned int timeStep = 0;
unsigned int alarmStep = 0;
char input = ' ';

bool zPressed = false;
unsigned long zPressTime = 0;

int switchState = HIGH;  // 현재 스위치 상태
int lastSwitchState = HIGH;  // 이전 스위치 상태
unsigned long lastDebounceTime = 0;  // 디바운싱을 위한 시간 기록
unsigned long debounceDelay = 50;

int states=0;
int counts=0;

unsigned long cnt=0;




void setup() {
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);
  Timer1.initialize(1000000);
  Timer1.attachInterrupt(sec);
  pinMode(zPin,INPUT_PULLUP);
  loadSettings();
}

void loop() {
  if (hour >= 24) hour = 0;
  if (second >= 60) { minute++; second = 0; }
  if (minute >= 60) { hour++; minute = 0; }
  int reading = digitalRead(zPin);
  Serial.println(reading);

  if (digitalRead(zPin) == LOW && counts == 0) {
    cnt = millis();
    counts++;
  }
  else if (millis() > cnt+500) {
    states = counts;
    counts = 0;
  }
  else if (digitalRead(zPin) && millis() > cnt + 200) {
    cnt = millis();
    counts++;

    if (counts >= 3) {
      cnt = 0;
      states = counts;
      counts = 0;
    }
  }
  switch(states) {
    case 0:
      break;
    case 1:
      setMode = true;
      Serial.println("시간 설정 모드 켜짐");
      break;
    case 2:
      alarmMode = true;
      Serial.println("알람 설정해");
      break;
    case 3:
      melodyMode = true;
      Serial.println("소리 설정");
      break;
  }

  if (states > 0)
	{
		cnt=0;
		states=0;
		counts=0;
	}



  // if (digitalRead(zPin) == LOW) {
  //   setMode = true;
  //   alarmMode = false;
  //   melodyMode = false;
  //   Serial.println("시간 설정 모드 시작");
  // }
  // else if (digitalRead(zPin) == LOW && !zPressed) {
  //   unsigned long currentMillis = millis();
  //   if (currentMillis - zPressTime < 300) {
  //     if (!setMode) {
  //       alarmMode = !alarmMode;
  //       Serial.print("알람 모드 : ");
  //       Serial.println(alarmMode);
  //     }
  //   }
  //   else if (currentMillis - zPressTime >= 3000) {
  //     if (!setMode && !alarmMode) {
  //       melodyMode = !melodyMode;
  //       Serial.print("멜로디 선택 모드 : ");
  //       Serial.println(melodyMode);
  //     }
  //   }
  //   zPressTime = currentMillis;
  //   zPressed = true;
  // }
  //   else if (digitalRead(zPin) == HIGH) {
  //     zPressed = false;
  //   } 

  if (hour == alarmHour && minute == alarmMinute && second == alarmSecond) {
    playMelody();
  }
  setTimeJoystick();
  saveTime();
}

void playMelody() {
  int notes;
  if (melodyIndex == 1)
    notes = sizeof(melody1) / sizeof(melody1[0]) / 2;
  else
    notes = sizeof(melody2) / sizeof(melody2[0]) / 2;
  int wholenote = (60000 * 4) / 160;
  int divider = 0, noteDuration = 0;
  for (int i = 0; i < notes * 2; i = i + 2) {
    divider = selectedMelody[i+1];
    if (divider > 0) {
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5;
    }
    tone(11, selectedMelody[i], noteDuration*0.9);
    delay(noteDuration);
    noTone(11);

    if (digitalRead(zPin) == LOW) 
        break;
    printTime();
  }
  
  delay(500);
}

void sec() {
  if (!setMode) second++;
}


void printTime() {
  lcd.setCursor(0, 0);
  if (hour >= 12){
    lcd.print("PM ");
    if (hour >= 13 && hour <= 21) lcd.print("0");
  }
  else {
    lcd.print("AM ");
    if (hour < 10) lcd.print("0");
  }
  lcd.print(hour <= 12 ? hour : hour - 12);
  lcd.print(":");
  lcd.print(minute < 10 ? "0" : "");
  lcd.print(minute);
  lcd.print(":");
  lcd.print(second < 10 ? "0" : "");
  lcd.print(second);
  lcd.setCursor(11,0);
  lcd.print(" ");

  lcd.setCursor(0,1);
  lcd.print("Alarm ");
  if (alarmHour >= 12){
    lcd.print("PM ");
    if (alarmHour >= 13 && alarmHour <= 21) lcd.print("0");
  }
  else {
    lcd.print("AM ");
    if (alarmHour < 10) lcd.print("0");
  }
  lcd.print(alarmHour <= 12 ? alarmHour : alarmHour - 12);
  lcd.print(":");
  lcd.print(alarmMinute < 10 ? "0" : "");
  lcd.print(alarmMinute);
}

void setTimeJoystick() {
  int x = analogRead(xPin);
  int y = analogRead(yPin);
  int z = digitalRead(zPin);

  if (setMode) {
    timeStep %= 3;
     if (timeStep == 0) {
      lcd.setCursor(3,0);
      lcd.print("  ");
      delay(200);
    }
    else if (timeStep == 1) {
      lcd.setCursor(6,0);
      lcd.print("  ");
      delay(200);
    }
    else {
      lcd.setCursor(9,0);
      lcd.print("  ");
      delay(200);
    }

    if (y < 100) {
      if (timeStep == 0) hour = (hour + 1) % 24;
      else if (timeStep == 1) minute = (minute + 1) % 60;
      else if (timeStep == 2) second = (second + 1) % 60;
    }

    if (y > 900) {
      if (timeStep == 0) hour = (hour + 23) % 24;
      else if (timeStep == 1) minute = (minute + 59) % 60;
      else if (timeStep == 2) second = (second + 59) % 60;
    }

    if (x > 900) timeStep++;
    if (x < 100) timeStep--;
    if ( digitalRead(zPin) == LOW) {
      setMode = false;
      Serial.println("시간 설정 끝");
    }
    printTime();
    delay(200);
  }
  else if (alarmMode) {
    alarmStep %= 2;
    delay(500);

    if (alarmStep == 0) {
      lcd.setCursor(9,1);
      lcd.print("  ");
      delay(100);
    }
    else if (alarmStep == 1) {
      lcd.setCursor(12,1);
      lcd.print("  ");
      delay(100);
    }

    if (y < 100) {
      if (alarmStep == 0) alarmHour = (alarmHour + 1) % 24;
      else if (alarmStep == 1) alarmMinute = (alarmMinute + 1) % 60;
    }

    if (y > 900) {
      if (alarmStep == 0) alarmHour = (alarmHour + 23) % 24;
      else if (alarmStep == 1) alarmMinute = (alarmMinute + 59) % 60;
    }

    if (x > 900) alarmStep++;
    if (x < 100) alarmStep--;
    if (z == LOW) alarmMode = false;
    printTime();
    delay(200);
    saveTime();
  }
  else if (melodyMode) {
    lcd.setCursor(0,0);
    lcd.print("Melody : ");
    lcd.setCursor(0,1);

    if (y < 100) melodyIndex++;
    else if (y > 900) melodyIndex--;

    melodyIndex %= 2;
    Serial.print("melodyIndex");
    Serial.println(melodyIndex);

    if (melodyIndex == 1) {
      selectedMelody = melodyList[0];
      lcd.print("Harry Potter");
      delay(500);
      lcd.clear();
    }
    else {
      lcd.print("Merry Christmas");
      selectedMelody = melodyList[1];
      delay(500);
      lcd.clear();
    }
    saveSound();
  }
  else {
    printTime();
  }
}

void saveTime() {
  EEPROM.put(0, hour);
  EEPROM.put(sizeof(int), minute);
  EEPROM.put(sizeof(int)*2 , second);
  EEPROM.put(sizeof(int) * 3, alarmHour);
  EEPROM.put(sizeof(int) * 4, alarmMinute);
  
}

void saveSound() {
  EEPROM.put(sizeof(int) * 5, melodyIndex);
}

void loadSettings() {
  hour = EEPROM.get(0, hour);
  minute = EEPROM.get(sizeof(int), minute);
  second = EEPROM.get(sizeof(int) * 2 , second);
  alarmHour = EEPROM.get(sizeof(int) * 3, alarmHour);
  alarmMinute = EEPROM.get(sizeof(int) * 4, alarmMinute);
  melodyIndex = EEPROM.get(sizeof(int) * 5, melodyIndex);
}