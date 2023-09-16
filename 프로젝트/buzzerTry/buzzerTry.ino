const int VRX = A0; //답 입력 조이스틱
const int VRY = A1;
const int SW = A2;

const int LED_U = 10; //문제, 답 출력 led 핀 번호
const int LED_R = 11;
const int LED_D = 12;
const int LED_L = 9;

const int RED = A3; // 목숨 표시 RGB led
const int GREEN = A4;
const int BLUE = A5;

const int pins[] = {2,3,4,5,6,7,8};    //세그먼트 연결 
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
                            B10001110  //F
                            };
#include "pitches.h"

const int buzzer = 13;
//시작 노래
int melody[] = { 
  NOTE_B4, 16, NOTE_B5, 16, NOTE_FS5, 16, NOTE_DS5, 16, //1
  NOTE_B5, 32, NOTE_FS5, -16, NOTE_DS5, 8, NOTE_C5, 16,
  NOTE_C6, 16, NOTE_G6, 16, NOTE_E6, 16, NOTE_C6, 32, NOTE_G6, -16, NOTE_E6, 8,
  NOTE_B4, 16,  NOTE_B5, 16,  NOTE_FS5, 16,   NOTE_DS5, 16,  NOTE_B5, 32,  //2
  NOTE_FS5, -16, NOTE_DS5, 8,  NOTE_DS5, 32, NOTE_E5, 32,  NOTE_F5, 32,
  NOTE_F5, 32,  NOTE_FS5, 32,  NOTE_G5, 32,  NOTE_G5, 32, NOTE_GS5, 32,  NOTE_A5, 16, NOTE_B5, 8
};
// 끝 노래
int melodyOver[] = {NOTE_C4, 4, NOTE_G3, 8, NOTE_G3, 8, NOTE_A3, 4, NOTE_G3, 4, REST, 4, NOTE_B3, 4, NOTE_C4, 4};

int input_cnt = 0, level = 1, lifeN = 3; //1레벨, 목숨 3개로 시작

void setup() {
  Serial.begin(9600);
  pinMode(SW, INPUT_PULLUP);
  pinMode(LED_U, OUTPUT);
  pinMode(LED_R, OUTPUT);
  pinMode(LED_D, OUTPUT);
  pinMode(LED_L, OUTPUT);
  for (int i = 0; i < 7; i++){ //7세그먼트
    pinMode(pins[i], OUTPUT);
  }
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  randomSeed(analogRead(0));
  digitalWrite(GREEN, HIGH);   //시작 목숨 알림
  showScore(level);            //시작 level
  sing(melody, 120);           //시작 알림 노래
}


void loop() {
  while (lifeN){
    int answer[level] = {}, response[level] ={}; //레벨 당 게임 끝날 때마다 초기화
    for (int i = 0; i < 2; i++){                 //시작 의미로 반복
      for (int j = 9; j < 13; j++){
        ledOn(j, 50); 
      }
    }
    randQuestion(answer);           //문제 난수 생성
    inputJoy(response);             //정답 입력
    correctCheck(answer, response); //정답 확인
    showScore(level);               //레벨 확인
  }
}


void ledOn(int n, int t){
  digitalWrite(n, HIGH);
  delay(t);
  digitalWrite(n, LOW);
  delay(t);
}

void randQuestion(int ans[]){      //정답 배열 만들기
  for (int i = 0; i < level; i++){ //레벨 수만큼 문제 만들기
    int seed = random(9,13); 
    ans[i] = seed;
    Serial.print("정답은 : ");
    Serial.print(ans[i]);          //정답 내용 확인용
    ledOn(ans[i],300);
  }
  Serial.println();
}

bool isJoystickMoved(int x, int y, int z){
  return (x < 100 || x > 900 || y < 100 || y > 900 || z < 50);
}

void inputJoy(int resp[]){ //n = 레벨
  input_cnt = 0;
  while(input_cnt < level){
    int x = analogRead(VRX);
    int y = analogRead(VRY);
    int z = analogRead(SW);

    if (isJoystickMoved(x, y, z)){

      Serial.print(x); //잠시 확인용
      Serial.print("   ");
      Serial.print(y);
      Serial.print("    ");
      Serial.println(z);


      if (z < 100){
        Serial.println("다시 입력");
        input_cnt = 0;
          
      }
      else if (y < 100){
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
        sing(melodyOver, 180);
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
  Serial.print("***level : ");
  Serial.println(num);
  for (int bitP = 0; bitP < 8; bitP++){ //세그먼트
    segmentBit = bitRead(numberCode[num], bitP); //해당 레벨 세그먼트 패턴 가져와서 넣음
    digitalWrite(pins[bitP], segmentBit); //해당하는 세그먼트 led 켜기
  }
}

void sing(int mel[], int tem){
  int notes = sizeof(melody) / sizeof(mel[0]) / 2;
  int wholenote = (60000 * 4) / tem;
  int divider = 0, noteDuration = 0;
  for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {
    divider = mel[thisNote + 1];
    if (divider > 0) {
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5;
    }
    tone(buzzer, mel[thisNote], noteDuration * 0.9);
    delay(noteDuration);
    noTone(buzzer);
  }
}
