#define MAX 16
const int VRX = A0; //조이스틱
const int VRY = A1;
const int SW = A2; //스위치

const int LED_U = 10; //위오아왼
const int LED_R = 11;
const int LED_D = 12;
const int LED_L = 13;

int cnt, level;
int answer[MAX], response[MAX];

const int pins[] = {2,3,4,5,6,7,8,9}; //세그먼트
const int numberCode[11] = {B01111111, //dp
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
}

void loop() {
  for (int i = 0; i < 3; i++){ //led 4
    for (int j = 0; j < 9; j++){
      on(j, 100);
    }
  }
  randQuestion(level);
  inputJoy(level);
  check(answer, response);
  delay(500);
}

void on(int n, int m){
  digitalWrite(n, HIGH);
  delay(50);
  digitalWrite(n, LOW);
  delay(50);
}

void randQuestion(int n){
  for (int i = 0; i < n; i++){
    int seed = random(0,4); 
    answer.push_back(seed);
  }
  for (int i =0; i < answer.size(); i++){ 
    Serial.println(answer[i]); //정답 내용 확인용
    on(answer[i],300); //level에 따라 속도 빨라지게,,?300 -10*level
  }
}

void inputJoy(int n){ //조이스틱으로 입력
  int x = analogRead(VRX);
  int y = analogRead(VRY);
  int z = digitalRead(SW);
  Serial.print(x); //잠시 확인용
  Serial.print("   ");
  Serial.print(y);
  Serial.print("   ");
  Serial.println(z);

  while(cnt < n){
    if (y == 1023){
      on(LED_U, 300);
      response[cnt] = 0;
    }
    else if (x == 1023){
      on(LED_R, 300);
      response[cnt] = 1;
    }
    else if (y == 0){
      on(LED_D, 300);
      response[cnt] = 2;
    }
    else if (x == 0){
      on(LED_L, 300);
      response[cnt] = 3;
    }
    cnt++;
  }
}

void check(&ans, &resp){
  bool equalAns = true;
  for (int i = 0; i < ans.size(); i++){
    if (ans[i] != resp[i]) {
      score--;
      showScore(score);
      //했던 배열로 재실행~
    }
  }
  else {
    score++;
    level++;
    showScore(level);
  }
}

void showScore(int num){
  int k = num++;
  boolean value;
  for (int bitP = 0; bitP < 8; bitP++){
    value = bitRead(numberCode[k], bitP);
    digitalWrite(pins[bitP], value); //해당하는 점수 세그먼트 출력
  }
}
