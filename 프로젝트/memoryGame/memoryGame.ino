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
  pinMode(SW, INPUT_PULLUP);  //스위치
  pinMode(LED_U, OUTPUT);     //LED
  pinMode(LED_R, OUTPUT);
  pinMode(LED_D, OUTPUT);
  pinMode(LED_L, OUTPUT);
  for (int i = 0; i < 7; i++){ //7세그먼트
    pinMode(pins[i], OUTPUT);
  }
  pinMode(RED, OUTPUT);        //RGB LED
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


void ledOn(int n, int t){     //led OnOff함수
  digitalWrite(n, HIGH);
  delay(t);                   //켜진 상태로 유지
  digitalWrite(n, LOW);       
  delay(t);                   //끈 상태로 유지
}

void randQuestion(int ans[]){      //문제 만들기 함수
  for (int i = 0; i < level; i++){ //레벨 수만큼 문제 만들기
    int seed = random(9,13);       //난수 받아서 값 만들기
    ans[i] = seed;
    ledOn(ans[i],300);             //해당 값의 LED 점등
  }
}

bool isJoystickMoved(int x, int y, int z){ //조이스틱의 반응이 있는지 확인
  return (x < 100 || x > 900 || y < 100 || y > 900 || z < 50);
}

void inputJoy(int resp[]){        //답 입력 함수
  input_cnt = 0;                  //시작 시 초기화
  while(input_cnt < level){       //level 수만큼 입력될 때까지 실행
    int x = analogRead(VRX);      //조이스틱 값
    int y = analogRead(VRY);
    int z = analogRead(SW);

    if (isJoystickMoved(x, y, z)){ //조이스틱의 반응이 있을 경우

      if (z < 50){                //스위치 누르면 재입력
        input_cnt = 0;
      }
      else if (y < 100){
        ledOn(LED_U, 300);        //해당 LED 점등
        resp[input_cnt] = 10;     //답 배열에 해당 값 넣기
        input_cnt++;
      }
      else if (x > 900){
        ledOn(LED_R, 300);
        resp[input_cnt] = 11;
        input_cnt++;
      }
      else if (y > 900){
        ledOn(LED_D, 300);
        resp[input_cnt] = 12;
        input_cnt++;
      }
      else if (x < 100){
        ledOn(LED_L, 300);
        resp[input_cnt] = 9;
        input_cnt++;
      }
    }
  }
}

void correctCheck(int ans[], int resp[]){ //정답과 답 일치 확인 함수
  bool correct = true;
  for (int i = 0; i < level; i++){ //하나라도 틀린 게 있을 경우
    if (ans[i] != resp[i]) { 
      correct = false;
      lifeN--;                     //목숨 하나 깎임
      if(lifeN == 2){              //목숨2 = 파랑
        digitalWrite(GREEN, LOW);
        digitalWrite(BLUE, HIGH);
      }
      else if (lifeN == 1){        //목숨1 = 자주
        digitalWrite(RED, HIGH);
      }
      else if (lifeN == 0){        //목숨0 = 빨강
        digitalWrite(BLUE, LOW);
        sing(melodyOver, 180);     //끝나는 노래 재생
      }
      break;
    }
  }
  if (correct == true){  //틀린 게 없을 경우
    level++;             //다음 레벨 진행
  }
}

void showScore(int num){ //num=레벨
  boolean segmentBit;
  for (int bitP = 0; bitP < 8; bitP++){          //세그먼트
    segmentBit = bitRead(numberCode[num], bitP); //해당 레벨 세그먼트 패턴 가져와서 넣음
    digitalWrite(pins[bitP], segmentBit);        //세그먼트 LED 켜기
  }
}

void sing(int mel[], int tem){  //멜로디 재생 함수
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
