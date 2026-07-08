#include "DHT.h"
#include <modbus.h>
#include <Arduino.h>
#include <SimpleTimer.h>
#include <modbusManager.h>

#define DHTPIN 12        // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11    // DHT 11
#define debug 0        
#define touchbutton 7    // 터치버튼
#define button 8         // 버튼
#define LED 13          
#define R_LED 9          
#define G_LED 10
#define B_LED 11

// 하나의 타이머 인스턴스로 여러 개의 타이머 스케줄을 관리합니다.
SimpleTimer  timer;

DHT dht(DHTPIN, DHTTYPE);

bool touchButtonState = false;
bool ButtonState = false;
// 디버깅용도로 전역번수를 많이 사용함
uint16_t humi = 0;
uint16_t temp = 0;
uint16_t cds = 0;
uint16_t poten = 0;
uint16_t hall = 0;

// 함수 선언 (PlatformIO/C++ 환경에서는 setup 이전에 선언하는 것이 안전합니다)
void readTempHumi();
void readCds();
void readPoten();
void readHall();
void LED_Action();
void SerialPrint();

void setup() {
    Serial.begin(9600);
    while (!Serial);

    // 모드버스 초기화 함수 호출 (시리얼 포트와 슬레이브 ID를 넘겨줌)
    initModbus(&Serial, 1);
 
    pinMode(touchbutton, INPUT);
    pinMode(button, INPUT);
    pinMode(LED, OUTPUT);
    pinMode(R_LED, OUTPUT);
    pinMode(G_LED, OUTPUT);
    pinMode(B_LED, OUTPUT);

    dht.begin();

    // SimpleTimer 문법: setInterval(간격_ms, 실행할_콜백_함수)
    timer.setInterval(2000, readTempHumi);
    timer.setInterval(500, readCds);
    timer.setInterval(500, readPoten);
    timer.setInterval(500, readHall);
}

void loop() {
    // 버튼 상태는 주기성이 아니라 항상 빠르게 체크해야 하므로 loop에서 바로 읽습니다.
    ButtonState = digitalRead(button);
    touchButtonState = digitalRead(touchbutton);
    
    mb.Coil(5, ButtonState);
    mb.Coil(6, touchButtonState);

    // 타이머 가동 (등록된 콜백 함수들이 시간에 맞춰 자동 실행됨)
    timer.run();

    LED_Action();
    mb.task();
    
    if (debug) SerialPrint();
}

// ------ 각 타이머에 의해 호출될 콜백 함수들 ------

void readTempHumi() {
    // 소수점 둘째 자리까지 살리기 위해 100.0f를 곱합니다. (예: 25.34 -> 2534)
    temp = (uint16_t)(dht.readTemperature() * 100.0f);
    humi = (uint16_t)(dht.readHumidity() * 100.0f);

    mb.Hreg(0, temp); // 0번지: 온도
    mb.Hreg(1, humi); // 1번지: 습도
}

void readCds() {
    cds = (uint16_t)(analogRead(A1));
    uint16_t cds1 = map(cds, 50, 1000, 0, 100);      
    mb.Hreg(2, cds1); // 2번지: 조도
}

void readPoten() {
    poten = (uint16_t)(analogRead(A0));
    uint16_t poten1 = map(poten, 0, 1023, 0, 100);      
    mb.Hreg(3, poten1); // 3번지: 가변저항
}

void readHall() {
    hall = (uint16_t)(analogRead(A2));
    uint16_t hall1 = map(hall, 100, 400, 0, 100);      
    mb.Hreg(4, hall1); // 4번지: 홀센서
}

// -----------------------------------------------

void SerialPrint() {
    Serial.print(F("Humidity: "));
    Serial.print(humi);
    Serial.print(F("%  Temperature: "));
    Serial.print(temp);
    Serial.println(F("°C "));
    Serial.print("CDS:");
    Serial.println(cds);
    Serial.print("가변저항:");
    Serial.println(poten);
    Serial.print("Hall:");
    Serial.println(hall);
    Serial.print("ButtonState:");
    Serial.println(mb.Coil(3));
    Serial.print("TouchButtonState:");
    Serial.println(mb.Coil(4));
    delay(1000);
    // 주의: debug가 1일 때 loop 안에서 delay(1000)을 쓰면 Modbus 통신과 다른 타이머들이 먹통이 됩니다.
    // 디버깅이 필요하다면 SerialPrint도 timer.setInterval()에 등록해서 사용하는 것을 추천합니다.
}

void LED_Action() {
    digitalWrite(13, mb.Coil(0) > 0 ? HIGH : LOW);
    digitalWrite(9,  mb.Coil(1) > 0 ? HIGH : LOW);
    digitalWrite(10, mb.Coil(2) > 0 ? HIGH : LOW);
    digitalWrite(11, mb.Coil(3) > 0 ? HIGH : LOW);
    
    if (mb.Coil(4) > 0) { 
        for(int i = 9; i <= 11; i++) { 
            digitalWrite(i, HIGH);     // 255 대신 HIGH 사용
        }
    } else { 
        for(int i = 9; i <= 11; i++) { 
            digitalWrite(i, LOW);      // 0 대신 LOW 사용
        }
    }
}