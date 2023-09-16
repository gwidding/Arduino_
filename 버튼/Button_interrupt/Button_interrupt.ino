const byte ledPin = 13; //LED 연결 핀
const byte interruptPin = 2; //인터럽트를 사용할 핀
volatile byte state = LOW;


void setup() {
  pinMode(ledPin, OUTPUT);           //13핀을 LED 제어를 위해 출력으로 지정
  pinMode(interruptPin, INPUT);      //2핀을 인터럽트 핀으로 쓰기 위해 INPUT
  attachInterrupt(0, blink, FALLING); //인터럽트 생성 함수
  //0:인터럽트 번호, blink:인터럽트 핸들러, change:핀의 레벨이 변하면 인터럽트 발생
}

void loop() {
  digitalWrite(ledPin, state);       //state 변수 값에 따라 LED On/Off
}

void blink(){
  state = !state;                   //값이 0이면 1, 1이면 0으로 계속 토글
}
//외부 인터럽트를 사용하면 계속 D2의 레벨을 검사할 필요 없음
//인터럽트의 디바운스는 따로 있음