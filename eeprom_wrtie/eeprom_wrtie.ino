#include <EEPROM.h>

int address = 0;
byte value;

void setup() {
  Serial.begin(9600);
  EEPROM.write(0,0);
  EEPROM.write(1,1);
  EEPROM.write(2,2);
  EEPROM.write(3,3);
  EEPROM.write(4,0xab);

  for (int i=0; i<10; i++){
  value =  EEPROM.read(i);
  Serial.print(i);
  Serial.print("\t");
  Serial.print(value, DEC);
  Serial.println();
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
