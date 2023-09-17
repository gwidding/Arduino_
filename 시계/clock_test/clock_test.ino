#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <TimerOne.h>
#include "pitches.h"
#include <EEPROM.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

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

void setup() {
 lcd.init();
 lcd.backlight();
 Serial.begin(9600);
 Timer1.initialize(1000000);
 Timer1.attachInterrupt(sec);
//  loadSettings();
}

void loop() {
  if (hour >= 24) {
    hour = 0;
  }
  if (second >= 60) {
    minute++;
    second = 0;
  }
  if (minute >= 60) {
    hour++;
    minute = 0;
  }
  if (second >= 60) {
    minute++;
    second = 0;
  }
  
  if (Serial.available()) {
    input = Serial.read();
    Serial.print("input = ");
    Serial.println(input);

    if (input == 'z') {
      setMode = !setMode;
      alarmMode = false;
      melodyMode = false;
      Serial.println(setMode);
    }
    else if (input == 'x') {
     alarmMode = !alarmMode;
     setMode = false;
     melodyMode = false;
     Serial.println(alarmMode);
    }
    else if (input == 'c') {
      setMode = false;
      alarmMode = false;
      melodyMode = !melodyMode;
      lcd.clear();
      Serial.println(melodyMode);
    }
  }

  if (setMode) {
    static int setStep = 0;
    setStep = constrain(setStep, 0, 2);
    // 깜빡거리게 .........
    if (input == 'w') {
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
    if (input == 's') {
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
    if (input == 'd') {
      setStep++;
      Serial.println(setStep);
      delay(300);
    }
    if (input == 'a') {
      setStep--;
      Serial.println(setStep);
      delay(300);
    }
    printTime();
    saveTime();
  }
  else if (alarmMode) {
    static int setStep = 0;
    setStep = constrain(setStep, 0, 1);
    // 깜빡거리게 .........
    if (input == 'w') {
      if (setStep == 0) {
        alarmHour = (alarmHour + 1) % 24;
      }
      else if (setStep == 1) {
        alarmMinute = (alarmMinute + 1) % 60;
      }
      delay(300);
    }
    if (input == 's') {
      if (setStep == 0) {
        alarmHour = (alarmHour + 23) % 24;
      }
      else if (setStep == 1) {
        alarmMinute = (alarmMinute + 59) % 60;
      }
      delay(300);
    }
    if (input == 'd') {
      setStep++;
      Serial.println(setStep);
      delay(300);
    }
    if (input == 'a') {
      setStep--;
      Serial.println(setStep);
      delay(300);
    }
    printTime();
    saveTime();
  }
  else if (melodyMode) {
    lcd.setCursor(0,0);
    if (melodyIndex == 1) {
      lcd.print("Harry Potter");
    }
    else
      lcd.print("Merry Christmas");
    if (input == '1') {
      selectedMelody = melodyList[0];
      melodyIndex = 1;
      Serial.println("멜로디 선택 완료1");
      delay(500);
      lcd.clear();
    }
    else if (input == '2') {
      selectedMelody = melodyList[1];
      melodyIndex = 2;
      Serial.println("멜로디 선택 완료2");
      delay(500);
      lcd.clear();
    }
    else if (input == '0'){
      melodyMode = false;
      lcd.setCursor(0,1);
      lcd.print("Selected!");
      delay(1000);
      lcd.clear();
      melodyMode = false;
    }
    saveSound();
  }
  else {
    printTime();
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

    if (Serial.available()) {
      input = Serial.read();
      if (input == 's') {
        Serial.println("알람을 끕니다.");
        break;
      }
    }
  }
  delay(500);
}

void sec() {
  if (!setMode)
    second++;
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
