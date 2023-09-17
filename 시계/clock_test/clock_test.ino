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
int* selectedMelody = melodyList[0];

int timeStep = 0;
int alarmStep = 0;

char input = ' ';

void setup() {
 lcd.init();
 lcd.backlight();
 Serial.begin(9600);
 Timer1.initialize(1000000);
 Timer1.attachInterrupt(sec);
 loadSettings();
}

void loop() {
  if (hour >= 24) hour = 0;
  if (second >= 60) { minute++; second = 0; }
  if (minute >= 60) { hour++; minute = 0; }
  
  if (Serial.available()) {
    input = Serial.read();

    if (input == 'z') {
      setMode = !setMode;
      alarmMode = false;
      melodyMode = false;
    }
    else if (input == 'x') {
     alarmMode = !alarmMode;
     setMode = false;
     melodyMode = false;
    }
    else if (input == 'c') {
      setMode = false;
      alarmMode = false;
      melodyMode = !melodyMode;
      lcd.clear();
    }
  }

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
    if (input == 'w') {
      if (timeStep == 0) hour = (hour + 1) % 24;
      else if (timeStep == 1) minute = (minute + 1) % 60;
      else if (timeStep == 2) second = (second + 1) % 60;
    }
    if (input == 's') {
      if (timeStep == 0) hour = (hour + 23) % 24;
      else if (timeStep == 1) minute = (minute + 59) % 60;
      else if (timeStep == 2) second = (second + 59) % 60;
    }
    if (input == 'd') timeStep++;
    if (input == 'a') timeStep--;
    
    printTime();
    delay(200);
    saveTime();
  }
  else if (alarmMode) {
    static int setStep = 0;
    alarmStep %= 2;

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

    if (input == 'w') {
      if (alarmStep == 0) alarmHour = (alarmHour + 1) % 24;
      else if (alarmStep == 1) alarmMinute = (alarmMinute + 1) % 60;
    }
    if (input == 's') {
      if (alarmStep == 0) alarmHour = (alarmHour + 23) % 24;
      else if (alarmStep == 1) alarmMinute = (alarmMinute + 59) % 60;
    }
    if (input == 'd') alarmStep++;
    if (input == 'a') alarmStep--;

    printTime();
    delay(200);
    saveTime();
  }
  else if (melodyMode) {
    lcd.setCursor(0,0);
    lcd.print("Melody : ");
    lcd.setCursor(0,1);
    melodyIndex %= 2;

    if (input == 'w') melodyIndex++;
    else if (input == 's') melodyIndex--;

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

  if (hour == alarmHour && minute == alarmMinute && second == alarmSecond) {
    playMelody();
  }

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

    if (Serial.available()) {
      input = Serial.read();
      if (input == 's') {
        break;
      }
    }
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
  lcd.print("");

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

void saveTime() {
  EEPROM.put(0, hour);
  EEPROM.put(sizeof(int), minute);
  EEPROM.put(sizeof(int) * 2 , second);
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
