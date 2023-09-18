#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <TimerOne.h>
#include "pitches.h"
#include <EEPROM.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
const int xPin = A0;
const int yPin = A1;
const int zPin = 2;

bool setMode = false;
bool alarmMode = false;
bool melodyMode = false;

int hour = 11;
int minute = 0;
int second = 0;
int alarmHour = 12;
int alarmMinute = 0;
int alarmSecond = 0;
int melodyIndex = 0;

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
int* selectedMelody;

char input = ' ';

bool zPressed = false;
unsigned long zPressTime = 0;

int switchState = HIGH;  // 현재 스위치 상태
int lastSwitchState = HIGH;  // 이전 스위치 상태
unsigned long lastDebounceTime = 0;  // 디바운싱을 위한 시간 기록
unsigned long debounceDelay = 50;

void setup() {
 lcd.init();
 lcd.backlight();
 Serial.begin(9600);
 Timer1.initialize(1000000);
 Timer1.attachInterrupt(sec);
//loadSettings();
}

void loop() {
  int reading = digitalRead(zPin);
  currentTime = millis();
  elapsedTime = currentTime - lastTime; // 경과시간

  if (elapsedTime >= interval) {
    lastTime = currentTime;
    // updateClock();
  }
  Serial.println(digitalRead(zPin));

  if (digitalRead(zPin) == LOW) {
    setMode = true;
    alarmMode = false;
    melodyMode = false;
    Serial.println("시간 설정 모드 시작");
  }
  else if (digitalRead(zPin) && !zPressed) {
    unsigned long currentMillis = millis();
    if (currentMillis - zPressTime < 300) {
      if (!setMode) {
        alarmMode = !alarmMode;
        Serial.print("알람 모드 : ");
        Serial.println(alarmMode);
      }
    }
    else if (currentMillis - zPressTime >= 3000) {
      if (!setMode && !alarmMode) {
        melodyMode = !melodyMode;
        Serial.print("멜로디 선택 모드 : ");
        Serial.println(melodyMode);
      }
    }
    zPressTime = currentMillis;
    zPressed = true;
  }
    else if (digitalRead(zPin) == HIGH) {
      zPressed = false;
    } 

  if (hour == alarmHour && minute == alarmMinute && second == alarmSecond) {
    playMelody();
  }
  saveTime();
}

void playMelody() {
  int notes;
  if (melodyIndex == 1)
    notes = sizeof(melody1) / sizeof(selectedMelody[0]) / 2;
  else
    notes = sizeof(melody2) / sizeof(selectedMelody[0]) / 2;
  int wholenote = (60000 * 4) / 144;
  int divider = 0, noteDuration = 0;
  for (int i = 0; i < notes * 2; i = i + 2) {
    Serial.println("알람시간입니다!!");
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

    int z = digitalRead(zPin);
    if (z == LOW) {
      Serial.println("알람을 끕니다.");
      break;
    }
  }
  delay(500);
}

void sec() {
  if (!setMode) {
    second++;
  }
}

void printTime() {
  lcd.setCursor(0, 0);
  if (hour >= 12)
    lcd.print("PM ");
  else
    lcd.print("AM ");
  if (hour < 10)
    lcd.print(0);
  lcd.print(hour);
  lcd.print(":");
  if (minute < 10)
    lcd.print(0);
  lcd.print(minute);
  lcd.print(":");
  if (second < 10)
    lcd.print(0);
  lcd.print(second);

  lcd.setCursor(0,1);
  lcd.print("Alarm ");
  if (alarmHour >= 12)
    lcd.print("PM ");
  else
    lcd.print("AM ");
  if (alarmHour < 10)
    lcd.print(0);
  lcd.print(alarmHour);
  lcd.print(":");
  if (alarmMinute < 10)
    lcd.print(0);
  lcd.print(alarmMinute);
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

void setTimeJoystick() {
  int x = analogRead(xPin);
  int y = analogRead(yPin);
  int z = digitalRead(zPin);

  if (setMode) {
    static int setStep = 0;
    constrain(setStep, 0, 2);
    if (y < 100) {
      if (setStep == 0) {
        hour = (hour + 1) % 24;
      }
      else if (setStep == 1) {
        minute = (minute + 1) % 60;
      }
      else if (setStep == 2) {
        second = (second + 1) % 60;
      }
      delay(300);
    }

    if (y > 900) {
      if (setStep == 0) {
        hour = (hour + 23) % 24;
      }
      else if (setStep == 1) {
        minute = (minute + 59) % 60;
      }
      else if (setStep == 2) {
        second = (second + 59) % 60;
      }
      delay(300);
    }

    if (x > 900) {
      setStep++;
      delay(300);
    }
    if (x < 100) {
      setStep--;
      delay(300);
    }
    if (z == LOW) {
      setMode = false;
    }
    printTime();
  } 
  else if (alarmMode) {
    static int setStep = 0;
    constrain(setStep, 0, 2);

    if (y < 100) {
      if (setStep == 0) {
        alarmHour = (alarmHour + 1) % 24;
      }
      else if (setStep == 1) {
        alarmMinute = (alarmMinute + 1) % 60;
      }
      else if (setStep == 2) {
        alarmSecond = (alarmSecond + 1) % 60;
      }
      delay(300);
    }

    if (y > 900) {
      if (setStep == 0) {
        alarmHour = (alarmHour + 23) % 24;
      }
      else if (setStep == 1) {
        alarmMinute = (alarmMinute + 59) % 60;
      }
      else if (setStep == 2) {
        alarmSecond = (alarmSecond + 59) % 60;
      }
      delay(300);
    }

    if (x > 900) {
      setStep++;
      delay(300);
    }
    if (x < 100) {
      setStep--;
      delay(300);
    }
    if (z == LOW) {
      alarmMode = false;
    }
    printTime();
  }
  else if(melodyMode) {
    if (y < 100) {
      selectedMelody = melodyList[0];
      melodyIndex = 1;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Harry Potter");
      Serial.println("멜로디 선택 완료1");
      melodyMode = false;
      lcd.clear();
    }
    if (y > 900) {
      selectedMelody = melodyList[1];
      melodyIndex = 2;
      lcd.print("Merry Christmas");
      Serial.println("멜로디 선택 완료2");
      melodyMode = false;
      lcd.clear();
    }
    saveSound();
  }
  else {
    printTime();
  }
}
