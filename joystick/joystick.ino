#include "Arduino.h"

const int joyStick_VRX = A0; 
const int joyStick_VRY = A1;
const int joyStick_SW = A2; 

int ledPin_13 = 13;
int ledPin_6 = 6;
int sensorValue = 0;
int outputValue = 0;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin_13, OUTPUT);
  pinMode(ledPin_6, OUTPUT);
}

void loop() {
  int VRX = analogRead(joyStick_VRX);
  int VRY = analogRead(joyStick_VRY); 
  int SW = analogRead(joyStick_SW);

  if (SW == LOW) {
    digitalWrite(ledPin_13, HIGH);
  } else {
    digitalWrite(ledPin_13, LOW);
  } 

  if (VRY == LOW)
  {
    analogWrite(6, outputValue);
  }
  
}


