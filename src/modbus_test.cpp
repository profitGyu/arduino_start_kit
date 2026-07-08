#include <Arduino.h>
#include <io.h>
#include <modbusManager.h>
#include <Wire.h>

int DIGITAL_INPUT[DEGITAL_INPUT_LENGTH] = {TOUCH07, BTN08};
int DIGITAL_OUTPUT[DEGITAL_OUTPUT_LENGTH] = {BZ06, LED13, RGB09, RGB10, RGB11, DHT12, SVM03};
int ANALOG_INPUT[ANALOG_INPUT_LENGTH] = {POTENTION_METER_A0};

void setup() {
    for(int i=0; i<DEGITAL_OUTPUT_LENGTH; i++) pinMode(DIGITAL_OUTPUT[i], OUTPUT);
    for(int j=0; j<DEGITAL_INPUT_LENGTH; j++) pinMode(DIGITAL_INPUT[j], INPUT);
    // 아날로그는 핀모드를 선언하지 않아도 됨
    // for(int z=0; z<ANALOG_INPUT_LENGTH; z++) pinMode(ANALOG_INPUT[z], INPUT);

    Serial.begin(9600);
    while (!Serial);

    // 모드버스 초기화 함수 호출 (시리얼 포트와 슬레이브 ID를 넘겨줌)
    initModbus(&Serial, 1);
}

void loop() {  
    
    

    bool coil1_state = mb.Coil(0);
    // digitalWrite(RGB11, coil1_state);
    digitalWrite(LED13, coil1_state);
    
    // 2. 물리 버튼 상태를 읽어서 마스터가 볼 수 있게 Coil(2) 주소에 업데이트
    bool button_state = digitalRead(BTN08);
    mb.Coil(2, button_state); 
    // 3. AI potionion Meter <-- 0~1023 [ADC] 가변저항 0~5v
    int potionMeter = analogRead(POTENTION_METER_A0);
    // 3. 데이터 맵핑
    int tempPotenMeter = map(potionMeter, 0 ,1023, 0, 255);
    int photoResister = map(analogRead(PHOTO_RESISTOR_A1), 0, 1023, 0 ,255);
    int hallEffect = analogRead(HALL_EFFECT_A2);
    int them = analogRead(THEM_A3);
    int mic = analogRead(MIC_A4);
    int flame = analogRead(FLAME_A5);
    
    mb.Hreg(0, tempPotenMeter);
    mb.Hreg(1, photoResister);
    mb.Hreg(2, hallEffect);
    mb.Hreg(3, them);
    mb.Hreg(4, mic);
    mb.Hreg(5, flame);

    // 4. 아날로그 출력(PWM) :
    int hretempPotenMeter = mb.Hreg(0);
    int hrePhotoResister = mb.Hreg(2);
    analogWrite(RGB09, hrePhotoResister);
    analogWrite(LED13, hretempPotenMeter);
    
    // 모드버스 통신 폴링
    mb.task();

}