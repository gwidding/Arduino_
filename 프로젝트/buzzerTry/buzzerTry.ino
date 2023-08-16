const int VRX = A0; //조이스틱
const int VRY = A1;
//const int SW = 2;

const int LED_U = 10; //위오아왼
const int LED_R = 11;
const int LED_D = 12;
const int LED_L = 9;

const int RED = A3; // 목숨 표시
const int GREEN = A4;
const int BLUE = A5;

const int pins[] = {A2,3,4,5,6,7,8}; //세그먼트 연결 
const int numberCode[15] = {
                            B11000000, //0 dp/gfedcba
                            B11111001, //1
                            B10100100, //2
                            B10110000, //3
                            B10011001, //4
                            B10010010, //5
                            B10000010, //6
                            B11011000, //7
                            B10000000, //8
                            B10010000, //9
                            B10001000, //A
                            B10000011, //b
                            B10100111, //c
                            B10100001, //E
                            B10001110 //F
                            };
#include "pitches.h"
int tempo = 105;
int buzzer = 2;
int melody[] = {
  // Pacman
  NOTE_B4, 16, NOTE_B5, 16, NOTE_FS5, 16, NOTE_DS5, 16, //1
  NOTE_B5, 32, NOTE_FS5, -16, NOTE_DS5, 8, NOTE_C5, 16,
  NOTE_C6, 16, NOTE_G6, 16, NOTE_E6, 16, NOTE_C6, 32, NOTE_G6, -16, NOTE_E6, 8,

  NOTE_B4, 16,  NOTE_B5, 16,  NOTE_FS5, 16,   NOTE_DS5, 16,  NOTE_B5, 32,  //2
  NOTE_FS5, -16, NOTE_DS5, 8,  NOTE_DS5, 32, NOTE_E5, 32,  NOTE_F5, 32,
  NOTE_F5, 32,  NOTE_FS5, 32,  NOTE_G5, 32,  NOTE_G5, 32, NOTE_GS5, 32,  NOTE_A5, 16, NOTE_B5, 8
};
int notes = sizeof(melody) / sizeof(melody[0]) / 2;
int wholenote = (60000 * 4) / tempo;
int divider = 0, noteDuration = 0;

int input_cnt = 0, level = 1, lifeN = 3; //1레벨, 목숨 3개로 시작, 목숨 0되면 죽음

void setup() {
  Serial.begin(9600);
  //pinMode(SW, INPUT_PULLUP);
  pinMode(LED_U, OUTPUT);
  pinMode(LED_R, OUTPUT);
  pinMode(LED_D, OUTPUT);
  pinMode(LED_L, OUTPUT);
  for (int i = 0; i < 7; i++){ //세그먼트
    pinMode(pins[i], OUTPUT);
  }
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  randomSeed(analogRead(0));
  digitalWrite(GREEN, HIGH); //목숨 3으로 시작
  showScore(level);
  sing();
}


void loop() {
  while (lifeN){
    int answer[level] = {}, response[level] ={};
    for (int i = 0; i < 2; i++){
      for (int j = 9; j < 13; j++){
        ledOn(j, 50); //시작 의미로 반복
      }
    }
    delay(500);

    randQuestion(answer); //문제 난수 생성
    inputJoy(response); //정답 입력
    correctCheck(answer, response); //정답확인

    showScore(level);
    
    // Serial.println(digitalRead(SW));
    // if (digitalRead(SW) == LOW){
    //   level = 1;
    //   lifeN = 3;
    //   digitalWrite(RED, LOW);
    //   digitalWrite(BLUE, LOW);
    //   digitalWrite(GREEN, LOW);
    //   digitalWrite(GREEN, HIGH);     
    // }
  }
}


void ledOn(int n, int m){
  digitalWrite(n, HIGH);
  delay(m);
  digitalWrite(n, LOW);
  delay(m);
}

void randQuestion(int ans[]){ //n = 레벨
  for (int i = 0; i < level; i++){ //1레벨이면 하나
    int seed = random(9,13); 
    ans[i] = seed;
    Serial.print("정답은 : ");
    Serial.print(ans[i]); //정답 내용 확인용
    ledOn(ans[i],300);
  }
  Serial.println();
}

bool isJoystickMoved(int x, int y){
  return (x < 100 || x > 900 || y < 100 || y > 900);
}

void inputJoy(int resp[]){ //n = 레벨
  input_cnt = 0;
  while(input_cnt < level){
    int x = analogRead(VRX);
    int y = analogRead(VRY);

    if (isJoystickMoved(x, y)){

      Serial.print(x); //잠시 확인용
      Serial.print("   ");
      Serial.println(y);


      if (y < 100){
        Serial.println("입력 : 위");
        ledOn(LED_U, 300);
        resp[input_cnt] = 10;
        input_cnt++;
      }
      else if (x > 900){
        Serial.println("입력 : 오른쪽");
        ledOn(LED_R, 300);
        resp[input_cnt] = 11;
        input_cnt++;
      }
      else if (y > 900){
        Serial.println("입력 : 아래");
        ledOn(LED_D, 300);
        resp[input_cnt] = 12;
        input_cnt++;
      }
      else if (x < 100){
        Serial.println("입력 : 왼");
        ledOn(LED_L, 300);
        resp[input_cnt] = 9;
        input_cnt++;
      }
    }
  }
}

void correctCheck(int ans[], int resp[]){
  bool correct = true;
  for (int i = 0; i < level; i++){
    if (ans[i] != resp[i]) {
      Serial.println("틀렸습니다");
      correct = false;
      lifeN--;
      if(lifeN == 2){ //파랑
        digitalWrite(GREEN, LOW);
        digitalWrite(BLUE, HIGH);
      }
      else if (lifeN == 1){ //자주
        digitalWrite(RED, HIGH);
      }
      else if (lifeN == 0){ //빨강
        digitalWrite(BLUE, LOW);
      }
      break;
    }
  }
  if (correct == true){
    level++;
  }
}

void showScore(int num){ //num = 레벨
  boolean segmentBit;
  Serial.print("level : ");
  Serial.println(num);
  for (int bitP = 0; bitP < 8; bitP++){ //세그먼트
    segmentBit = bitRead(numberCode[num], bitP); //해당 레벨 세그먼트 패턴 가져와서 넣음
    digitalWrite(pins[bitP], segmentBit); //해당하는 세그먼트 led 켜기
  }
}

void sing(){
  for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {
    divider = melody[thisNote + 1];
    if (divider > 0) {
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5;
    }
    tone(buzzer, melody[thisNote], noteDuration * 0.9);
    delay(noteDuration);
    noTone(buzzer);
  }
}
