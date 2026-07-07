/* 이 스케치는 빛 감지 센서를 이용해 빛의 밝기를 측정한다.
 * 빛 감지 센서에 연결된 A1 핀으로 빛의 밝기에 따라 0~1023 사이의 아날로그 입력 값이 입력되고
 * map 함수를 통해 아날로그 입력 값을 0~3 4 단계로 나뉘어 switch-case 에 이용된다.
 */

// LCD 사용을 위한 설정
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>
#include <io.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // LCD I2C 주소를 설정한다. 16 칸 2 줄 LCD 사용

const int photoresistorPin = A1; // A1 핀에 조도 센서 연결
const int sensorMin = 0;         // 실험을 통해 발견된 최소 센서 값, 수정할 수 있다.
const int sensorMax = 700;       // 실험을 통해 발견된 최대 센서 값, 수정할 수 있다.

void setup() {
  lcd.init();          // LCD 초기화
  lcd.backlight();     // 백 라이트를 켠다. (lcd.noBacklight()는 백 라이트 끈다.)
  lcd.setCursor(0, 0); // 첫 번째 줄 첫째 칸
  lcd.print("Range : "); // 메세지 출력
  pinMode(RGB09, OUTPUT);

  Serial.begin(9600);  // 9600 속도로 시리얼 통신 시작
}

void loop() {
  // 센서 값 읽어 범위 맵핑
  int sensorReading = analogRead(photoresistorPin); // A1 핀에서 조도 센서 값을 읽는다
  Serial.println(sensorReading);
  int range = map(sensorReading, sensorMin, sensorMax, 0, 3); // 센서 값을 0~3 까지 맵핑 한다.

  switch (range) {
    case 0: // 센서위에 손을 가까이 대어 어두워 지고 센서 값이 0 이 되면
      Serial.println("DARK"); // 시리얼창에 dark 인쇄하고 줄 바꿈
      lcd.setCursor(9, 0);    // 첫 번째 줄 열번째 칸
      lcd.print(range);       // LCD 창에 밝기 단계 나타내기
      lcd.setCursor(0, 1);    // 두 번째 줄 첫째 칸
      lcd.print("DARK  ");    // DARK 출력 (기존 문자 지우기용 공백 추가)
      analogWrite(RGB09, 0);
      break;

    case 1: // 센서위에 손을 가까이 대고 센서 값이 1 이 되면
      Serial.println("DIM");  // 시리얼창에 dim 인쇄하고 줄 바꿈
      lcd.setCursor(9, 0);    // 첫 번째 줄 열번째 칸
      lcd.print(range);       // LCD 창에 밝기 단계 나타내기
      lcd.setCursor(0, 1);    // 두 번째 줄 첫째 칸
      lcd.print("DIM   ");    // DIM 출력
      analogWrite(RGB09, 64);
      break;

    case 2: // 센서위에 손을 멀리하며 센서 값이 2 가 되면
      Serial.println("MEDIUM"); // 시리얼창에 medium 인쇄하고 줄 바꿈
      lcd.setCursor(9, 0);      // 첫 번째 줄 열번째 칸
      lcd.print(range);         // LCD 창에 밝기 단계 나타내기
      lcd.setCursor(0, 1);      // 두 번째 줄 첫째 칸
      lcd.print("MEDIUM");      // MEDIUM 출력
      analogWrite(RGB09, 127);
      break;

    case 3: // 센서에 근처에 손을 대지 않고 센서 값이 3 이 되면
      Serial.println("BRIGHT"); // 시리얼창에 bright 인쇄하고 줄 바꿈
      lcd.setCursor(9, 0);      // 첫 번째 줄 열번째 칸
      lcd.print(range);         // LCD 창에 밝기 단계 나타내기
      lcd.setCursor(0, 1);      // 두 번째 줄 첫째 칸
      lcd.print("BRIGHT");      // BRIGHT 출력
      analogWrite(RGB09, 255);
      break;
  }
  delay(50); // 안정적으로 값을 읽기 위해 50 밀리초 지연
}