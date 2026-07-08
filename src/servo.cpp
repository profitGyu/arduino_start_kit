#include <Arduino.h>
#include <Wire.h>
#include <io.h>
#include <LiquidCrystal_I2C.h> 
#include <Servo.h>

// 함수 밖에 전역 변수로 선언해두어야 합니다.
unsigned long previousMillis = 0; 
int step = 0; // 현재 서보모터의 단계를 저장

Servo servo;

long time = millis();
LiquidCrystal_I2C lcd(0x27, 16, 2); // LCD I2C 주소를 설정한다. 16 칸 2줄 LCD 사용 

void setup() {
    servo.attach(SVM03);

    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0); // 첫 번째 줄 첫째 칸 
    lcd.print("Hello~~~"); 
    lcd.setCursor(0, 1); // 두 번째 줄 첫째 칸 
    lcd.print("DHT Sensor Start"); 
    delay(2000); 
    lcd.clear();
}




void loop() {
    unsigned long currentMillis = millis();
    bool btnState = digitalRead(13);

    if (currentMillis - previousMillis >= 1000) {
        previousMillis = currentMillis; 
        step++;

        if (step == 1) {
            servo.write(50);
            lcd.setCursor(0, 0);
            lcd.print("SERVE STEP :");
            lcd.print(step);
            lcd.print(" 단계");
        } 
        else if (step == 2) {
            servo.write(100);
            lcd.setCursor(0, 0);
            lcd.print("SERVE STEP :");
            lcd.print(step);
            lcd.print(" 단계");
        } 
        else if (step == 3) {
            servo.write(140);
            lcd.setCursor(0, 0);
            lcd.print("SERVE STEP :");
            lcd.print(step);
            lcd.print(" 단계");
        } 
        else if (step == 4) {
            servo.write(180);
            lcd.setCursor(0, 0);
            lcd.print("SERVE STEP :");
            lcd.print(step);
            lcd.print(" 단계");
            step = 0; 
        }
    }
}