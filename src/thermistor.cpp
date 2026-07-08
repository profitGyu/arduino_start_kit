/* 이 스케치는 A5 에 연결된 써미스터에 분배된 전압을 저항으로 환산하고
 * 저항 값을 온도로 변화시키는 방법 중 하나인
 * Steinhart-Hart 식을 이용하여 온도 계산을 해본다.
 * A5 핀과 I2C LCD 모듈은 함께 사용할 수 없으므로 점퍼로 사용 모듈을 선택해야 한다.
 */

#include <math.h> // 로그(log) 연산을 위해 수학 라이브러리를 포함합니다.
#include <Arduino.h>

const int thermistorPin = A5; // A5번에 써미스터 연결

// 파라미터 상수는 모듈 부품 특성에 따라 가이드북에 정의된 고유 값을 사용합니다.
double ParamA = 0.001129148;
double ParamB = 0.000234125;
double ParamC = 0.0000000876741;

// Steinhart-Hart 식 온도 측정 함수 정의
double Thermistor(int RawADC) {
  double Temp;
  // 전압 분배 법칙을 이용해 10K 전역 저항과 비교하여 써미스터의 현재 저항값 계산 및 로그 적용
  Temp = log(10000.0 * ((1024.0 / RawADC - 1)));
  // Steinhart-Hart 공식을 대입하여 절대 온도 계산
  Temp = 1 / (ParamA + (ParamB + (ParamC * Temp * Temp)) * Temp);
  return Temp;
}

void setup() {
  Serial.begin(9600); // 9600 속도로 시리얼 통신 시작
}

void loop() {
  int readVal = analogRead(thermistorPin); // A5 핀에서 아날로그 전압 값 읽기
  
  double temp = Thermistor(readVal);       // Steinhart-Hart 온도 측정 함수 호출 (절대온도 반환)
  double tempC = temp - 273.15;            // 절대온도(Kelvin)를 섭씨온도(Celsius)로 환산
  double tempF = (tempC * 9.0) / 5.0 + 32.0; // 섭씨온도를 화씨온도(Fahrenheit)로 환산

  // 시리얼 모니터에 측정된 섭씨 온도 출력
  Serial.print(tempC); 
  Serial.println(" C");
  
  delay(500); // 0.5초(500ms) 지연 후 반복
}

