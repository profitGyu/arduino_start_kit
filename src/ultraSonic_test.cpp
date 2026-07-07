/* 이 스케치는 초음파 센서를 사용해 거리를 측정해 본다.
 * triger 핀은 아두이노의 2 번에 연결되어 있다. triger 핀은 초음파를 발생한다.
 * echo 핀은 아두이노의 4 번에 연결 되어있다. echo 핀은 반사된 초음파를 감지한다.
 * 측정된 거리는 시리얼 창과, LCD 창에 모두 나타내어본다.
 * 측정 거리가 설정 값 이하(미만)이면 빨간 LED 를, 
 * 측정 거리가 설정 값 이상이면 초록 LED 를 켜본다.
 */
#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2); // LCD I2C 주소를 설정한다. 16 칸 2 줄 LCD 사용

int redPin = 9;       // 빨간 LED 9 번에 연결
int greenPin = 10;    // 초록 LED 10 번에 연결
int threshold = 15;   // 거리 임계 값 설정 (15cm 기준)
int trigPin = 2;      // 트리거 2 번 핀
int echoPin = 4;      // 에코 4 번 핀

void setup() {
  pinMode(trigPin, OUTPUT);    // 초음파 트리거 핀모드 설정
  pinMode(echoPin, INPUT);     // 초음파 에코 핀모드 설정
  pinMode(redPin, OUTPUT);     // 빨간 LED 출력 설정
  pinMode(greenPin, OUTPUT);   // 초록 LED 출력 설정

  Serial.begin(9600);          // 9600 속도로 시리얼 통신 시작

  // LCD 설정
  lcd.init();
  lcd.backlight();             // 백 라이트를 켠다.
  lcd.clear();
}

void loop() {
  // 초음파 센서로 거리 측정
  float Duration, Distance;
  digitalWrite(trigPin, HIGH);  // 초음파 쏜다.
  delayMicroseconds(10);        // 10 마이크로초 동안 유지
  digitalWrite(trigPin, LOW);   // 초음파 끈다.
  Duration = pulseIn(echoPin, HIGH); // echo 핀이 HIGH 유지한 시간 저장

  // 왕복 시간(마이크로초)과 음속(340m/s)을 이용하여 거리를 cm로 환산
  Distance = ((float)(340 * Duration) / 10000) / 2;

  // 측정한 거리 시리얼 모니터에 출력
  Serial.print(Distance);       // 시리얼 창에 줄 바꿈 없이 거리 출력
  Serial.println("cm ");         // 단위 출력

  if (Distance < threshold) {   // 측정거리가 임계 값(15cm) 미만이면 빨간 LED 켠다.
    digitalWrite(redPin, HIGH);
    digitalWrite(greenPin, LOW);
    
    // LCD 창에 거리 나타내기
    lcd.clear();
    lcd.setCursor(0, 0);        // 첫 번째 줄 첫째 칸
    lcd.print(Distance);        // 측정된 거리 출력
    lcd.print("[cm]");          // 단위출력
  }
  else {                        // 측정값이 임계 값보다 크거나 같으면 초록 LED 를 켠다.
    digitalWrite(redPin, LOW);
    digitalWrite(greenPin, HIGH);
    
    // LCD 창에 거리 나타내기
    lcd.clear();
    lcd.setCursor(0, 0);        // 첫 번째 줄 첫째 칸
    lcd.print(Distance);        // 측정된 거리 출력
    lcd.print("[cm]");          // 단위출력
  }
  
  delay(1000);                  // 안정적으로 값을 읽기 위해 1초(1000ms) 지연
}