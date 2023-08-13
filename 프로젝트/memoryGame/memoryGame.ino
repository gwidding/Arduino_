#define MAX 9
const int VRX = A0; //조이스틱
const int VRY = A1;
const int SW = A2; //스위치

const int LED_U = 10; //위오아왼
const int LED_R = 11;
const int LED_D = 12;
const int LED_L = 13;

int input_cnt = 0, level = 1, lifeN = 2; //1레벨, 목숨 3개로 시작, 목숨 0되면 죽음
// 목숨 표시 -> RGB led
const int RED = A3;
const int GREED = A2;
const int BLUE = A1;

const int pins[] = {2,3,4,5,6,7,8}; //세그먼트 연결 
const int numberCode[10] = {//B01111111, //dp
                            B11000000, //0 dp/gfedcba
                            B11111001, //1
                            B10100100, //2
                            B10110000, //3
                            B10011001, //4
                            B10010010, //5
                            B10000010, //6
                            B11011000, //7
                            B10000000, //8
                            B10010000};//9

void setup() {
  Serial.begin(9600);
  pinMode(SW, INPUT_PULLUP); //z축 플로팅 현상 방지
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
  digitalWrite(GREEN, HIGH);
}


void loop() {
  int answer[level], response[level];
  for (int i = 0; i < 3; i++){ //led 4개
    for (int j = 10; j < 14; j++){ //10,11,12,13번 핀
      ledOn(j, 100);
    }
  }
  randQuestion(level); //문제 난수 생성
  inputJoy(level); //정답 입력
  collectCheck(answer, response); //정답확인

  delay(500);
}




void ledOn(int n, int m){
  digitalWrite(n, HIGH);
  delay(50);
  digitalWrite(n, LOW);
  delay(50);
}

void randQuestion(int n){ //n = 레벨
  for (int i = 0; i < n; i++){ //1레벨이면 하나
    int seed = random(10,14); 
    answer[i] = seed;
  }
  for (int i = 0; i < answer.size(); i++){ 
    Serial.println(answer[i]); //정답 내용 확인용
    ledOn(answer[i],300); //추가할 옵션 : level에 따라 속도 빨라지게,,?300 -10*level
  }
}

void inputJoy(int n){ //조이스틱으로 입력, n = 레벨
  while(input_cnt < n){
    int x = analogRead(VRX);
    int y = analogRead(VRY);
    int z = digitalRead(SW);
    Serial.print(x); //잠시 확인용
    Serial.print("   ");
    Serial.print(y);
    Serial.print("   ");
    Serial.println(z);

    if (y == 1023){ //위
      ledOn(LED_U, 300);
      response[cnt] = 10;
    }
    else if (x == 1023){
      ledOn(LED_R, 300);
      response[cnt] = 11;
    }
    else if (y == 0){
      ledOn(LED_D, 300);
      response[cnt] = 12;
    }
    else if (x == 0){
      ledOn(LED_L, 300);
      response[cnt] = 13;
    }
    input_cnt++;
  }
}

void collectCheck(&ans, &resp){
  bool equalAns = true;
  for (int i = 0; i < ans.size(); i++){
    if (ans[i] != resp[i]) { //틀리면
      lifeN--;
      if(lifeN == 1){
        digitalWrite(RED, LOW);
        digitalWrite(BLUE, HIGH);
      }
      else if (lifeN == 0){
        digitalWrute(BLUE, LOW);
        digitalWrite(RED, HIGH);
        break; //맞나?
      }

      showScore(level);
      //했던 배열로 재실행
      for (int i = 0; i < answer.size(); i++){ 
        Serial.println(answer[i]); //정답 내용 확인용
        ledOn(answer[i],300);
      }
      inputJoy(level);
      collectCheck(&ans, &resp);
    }
  }
  level++;
  showScore(level); 
}

void showScore(int num){ //num = 레벨
  int k = num;
  boolean segmentBit;
  for (int bitP = 0; bitP < 8; bitP++){ //세그먼트
    segmentBit = bitRead(numberCode[k], bitP); //해당 레벨 세그먼트 패턴 가져와서 넣음
    digitalWrite(pins[bitP], segmentBit); //해당하는 세그먼트 led 켜기
  }
}
