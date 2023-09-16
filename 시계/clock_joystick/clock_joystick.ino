#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include "Arduino.h"
#include "pitches.h"
#include <EEPROM.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
const int VRX = A0; 
const int VRY = A1;
const int SW = A2;
const int buzzer = 12;

bool setMode = false;
bool alarmMode = false;
bool soundMode = false;

unsigned long clickTime = 0;
bool clicked = false;

int melody_index = 0;

int hour = 14;
int minute = 0;
int second = 0;
int alarmHour = 12;
int alarmMinute = 0;


int melodyBear[] = {
  NOTE_C5,NOTE_C5,NOTE_C5,NOTE_C5,NOTE_C5,
  NOTE_E5,NOTE_G5,NOTE_G5,NOTE_E5,NOTE_C5,
  NOTE_G5,NOTE_G5,NOTE_E5,NOTE_G5,NOTE_G5,NOTE_E5,
  NOTE_C5,NOTE_C5,NOTE_C5,

  NOTE_G5,NOTE_G5,NOTE_E5,NOTE_C5,
  NOTE_G5,NOTE_G5,NOTE_G5,
  NOTE_G5,NOTE_G5,NOTE_E5,NOTE_C5,
  NOTE_G5,NOTE_G5,NOTE_G5,

  NOTE_G5,NOTE_G5,NOTE_E5,NOTE_C5,
  NOTE_G5,NOTE_G5,NOTE_G5,NOTE_A5,NOTE_G5,
  NOTE_C6,NOTE_G5,NOTE_C6,NOTE_G5,
  NOTE_E5,NOTE_D5,NOTE_C5
  };

int noteDurationsBear[] = {
  4, 8, 8, 4, 4,
  4, 8, 8, 4, 4,
  8, 8, 4, 8, 8, 4,
  4, 4, 2,
  4, 4, 4, 4,
  4, 4, 2,
  4, 4, 4, 4,
  4, 4, 2,
  4, 4, 4, 4,
  8, 8, 8, 8, 2,
  4, 4, 4, 4,
  4, 4, 2
};

int melodyHarry[] = {
  REST, NOTE_D4, NOTE_G4, NOTE_AS4, NOTE_A4,
  NOTE_G4, NOTE_D5, NOTE_C5, NOTE_A4,
  NOTE_G4, NOTE_AS4, NOTE_A4, NOTE_F4, NOTE_GS4,
  NOTE_D4, NOTE_D4, NOTE_G4, NOTE_AS4, NOTE_A4,
  NOTE_G4, NOTE_D5, NOTE_F5, NOTE_E5, NOTE_DS5, NOTE_B4,
  NOTE_DS5, NOTE_D5, NOTE_CS5, NOTE_CS4, NOTE_B4,
  NOTE_G4, NOTE_AS4, NOTE_D5, NOTE_AS4, NOTE_D5, NOTE_AS4,
  NOTE_DS5, NOTE_D5, NOTE_CS5, NOTE_A4, NOTE_AS4,
  REST, NOTE_AS4, NOTE_D5, NOTE_AS4, NOTE_D5, NOTE_AS4,
  NOTE_F5, NOTE_E5, NOTE_DS5, NOTE_B4,
  NOTE_DS5, NOTE_D5, NOTE_CS5, NOTE_CS4, NOTE_AS4, NOTE_G4,
};

int noteDurationsHarry[] = {
  2, 4, -4, 8, 4,
  2, 4, -2, -2,
  -4, 8, 4, 2, 4,
  -1, 4, -4, 8, 4,
  2, 4, 2, 4, 2, 4,
  -4, 8, 4, 2, 4, -1,

  2, 4, 2, 4, 2, 4,
  2, 4, -4, 8, 4, 2, 4,
  -1, 4, 2, 4, 2, 4,
  2, 4, 2, 4, -4, 8, 4,
  2, 4, -1, 4, 2, 4,
  2, 4, 2, 4, 2, 4,
  -4, 8, 4, 2, 4,
  -1, 4,

  2, 4, 2, 4, 2, 4,
  2, 4, -4, 8, 4,
  2, 4, -1, 4
};


void setup() {
  Wire.begin();
  lcd.init();
  lcd.backlight();
  pinMode(buzzer, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  lcd.setCursor(0, 0);
  lcd.print("Alarm ");
  printZero(alarmHour);
  lcd.print(":");
  printZero(alarmMinute);

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

  int x = analogRead(VRX);
  int y = analogRead(VRY); 
  int z = digitalRead(SW);
  Serial.print(x); // x의 값을 출력
  Serial.print(" "); // 공백 문자열을 출력
  Serial.print(y); // y의 값을 출력
  Serial.print(" "); // 공백 문자열을 출력
  Serial.println(z); // z의 값을 출력하고 줄 바꿈

  // if (z == LOW && !clicked) {
  //   unsigned long currentMillis = millis();
  //   if (currentMillis - clickTime < 500) {
  //     clicked = true;
  //     soundMode = true;
  //   }
  //   clickTime = currentMillis;
  // }
  // if (z == HIGH) {
  //   clicked = false;
  // }

  // if (z == LOW) {
  //   setMode = true;
  // }

  if (setMode) {
    settingTime(hour, minute, second);    
  }
  else if (alarmMode) {
    settingAlarm(alarmHour, alarmMinute);
  }
  else if (soundMode) {
    settingSound();
  }
  else {
    printTime();
  }

  if (z == LOW) {
    saveTime();  
    setMode = false;  
  }

  second++;
  delay(1000);
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

void alarmTime(int alarmHour, int alarmMinute, int melody_index) {
  if (alarmHour == hour && alarmMinute == minute) {
    if (melody_index == 0) {
      playMelody(melodyBear, noteDurationsBear);
    }
    else if (melody_index == 1) {
      playMelody(melodyHarry, noteDurationsHarry);
    }
  }
}

void playMelody(int *melody, int *noteDurations) {
  for (int i = 0; i < sizeof(melody); i++) {
    int Durations = 1000 / noteDurations[i];
    tone(buzzer, melody[i], Durations);

    if (digitalRead(SW) == LOW) {
      noTone(buzzer);
      break;
    }

    delay(Durations * 1.3);
    noTone(buzzer);
  }
}

void modeSwitch() {
  static unsigned long lastSwitchTime = 0;
  const int z = digitalRead(SW);

  if (z == LOW) {
    unsigned long currentTime = millis();

    if (currentTime - lastSwitchTime > 3000) {
      alarmMode = true;      
    }
    else if (!setMode && !alarmMode && !soundMode) {
      setMode = true; // 처음 누른 경우
    }

    lastSwitchTime = currentTime;
    delay(500); // 디바운스
  }
}

void settingTime(int &hour, int &minute, int &second) {
  static unsigned long blinkMillis = 0;
  unsigned long currentMillis = millis();

  if (currentMillis - blinkMillis >= 500) { // 0.5초마다 깜빡이게
    blinkMillis = currentMillis;
    
    int y = analogRead(VRY);
    if (y < 100) {
      hour++;
    }
    else if (y > 900) {
      hour--;
    }
    twentyfourMax(hour);

    int x = analogRead(VRX);

    if (x > 900) {
      if (y < 100) {
        minute++;
      }
      else if (y > 900) {
        minute--;
      }
      sixtyMax(minute);

      if (x > 900) {
        if (y < 100) {
          second++;
        }
        else if (y > 900) {
          second--;
        }
        sixtyMax(second);

        if (x > 900) {
          setMode = false;
        }

      }
    }

  }

}

void settingAlarm(int &alarmHour, int &alarmMinute) {
  static unsigned long blinkMillis = 0;
  unsigned long currentMillis = millis();

  if (currentMillis - blinkMillis >= 500) { // 0.5초마다 깜빡이게
    blinkMillis = currentMillis;
    
    int y = analogRead(VRY);

    if (y < 100) {
      alarmHour++;
    }
    else if (y > 900) {
      alarmHour--;
    }
    twentyfourMax(hour);

    int x = analogRead(VRX);

    if (x > 900) {
      if (y < 100) {
        alarmMinute++;
      }
      else if (y > 900) {
        alarmMinute--;
      }
      sixtyMax(minute);
    } 
  }

}

void settingSound() {
  int y = analogRead(VRY);
  if (y < 100) {
    melody_index = 0;
  }
  else if (y > 900) {
    melody_index = 1;
  }
  saveSound();
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


void saveTime() {
  EEPROM.put(0, hour);
  EEPROM.put(sizeof(int), minute);
  EEPROM.put(sizeof(int)*2 , second);
}

void saveAlarm() {
  EEPROM.put(sizeof(int) * 3, alarmHour);
  EEPROM.put(sizeof(int) * 4, alarmMinute);
}

void saveSound() {
  EEPROM.put(sizeof(int) * 5, melody_index);
}
