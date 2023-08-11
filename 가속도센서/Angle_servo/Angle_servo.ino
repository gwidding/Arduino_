//----- MPU6050 -----//
#include <MPU6050_tockn.h>
#include <Wire.h>

//----- Servo -----//
#include <Servo.h>
Servo servo;

MPU6050 mpu6050(Wire);            // 서보모터 사용 선언(객체 생성)

void setup() {
  Serial.begin(9600);             // 9600 보레이트로 통신 시작
  Wire.begin();                   // I2C 통신 초기화(시작)
  mpu6050.begin();                // mpu6050 초기화(시작)
  mpu6050.calcGyroOffsets(true);  // 오프셋 설정

  servo.attach(9);                // 서보모터 PWM 핀 설정 (9번핀 사용)
}

void loop() {
  mpu6050.update();                    // mpu6050모듈에서 값을 받아온 후 계산
  Serial.print("angleX : ");
  Serial.print(mpu6050.getAngleX());   // X축 회전 각도(Pitch)
  Serial.print("\tangleY : ");
  Serial.print(mpu6050.getAngleY());   // Y축 회전 각도(Yaw)
  Serial.print("\tangleZ : ");
  Serial.println(mpu6050.getAngleZ()); // Z축 회전 각도(Roll)

  int degree = map(mpu6050.getAngleY(), -90, 90, 0, 180); // MPU6050의 각도 -90~90을 0~180으로 변환
  int degree_limit = constrain(degree, 0, 180);           // 0~180으로 범위 제한함
  servo.write(degree_limit);                              // 서보모터 작동
}