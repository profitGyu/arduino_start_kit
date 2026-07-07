#include <Arduino.h>
#include <Servo.h>

// [기존 핀 정의]
int led = 13;
int red_led = 9;
int green_led = 10;
int blue_led = 11;
int button = 8;
int touch = 7;

// [신규 핀 정의 - 이미지 보드 기준]
int robot = 3;       // D3: 서보모터
int trigPin = 4;     // D4: 초음파 센서 발신(Trig)
int echoPin = 2;     // 초음파 수신(Echo)
int buzzer = 6;      // 부저 핀 (D5 또는 6번 핀)

Servo myServo;
String temp_string = ""; // 시리얼 문자열 저장 변수
int cnt = 0;

// --- 🎵 Ring Ding Dong 주파수 정의 (4~5옥타브) ---
#define NOTE_F4  349
#define NOTE_G4  392
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_C5  523
#define NOTE_D5  587
#define NOTE_F5  698

int melody[] = {
  // Ring Ding Dong, Ring Ding Dong
  NOTE_D5, NOTE_D5, NOTE_D5,   
  NOTE_D5, NOTE_D5, NOTE_D5,   
  
  // Ring diggi ding diggi ding ding ding
  NOTE_D5, NOTE_C5, NOTE_D5, NOTE_C5, 
  NOTE_D5, NOTE_C5, NOTE_D5, NOTE_D5, NOTE_D5,
  
  // Ring Ding Dong, Ring Ding Dong
  NOTE_F5, NOTE_F5, NOTE_F5,   
  NOTE_F5, NOTE_F5, NOTE_F5,   
  
  // Ring diggi ding diggi ding ding ding
  NOTE_F5, NOTE_D5, NOTE_F5, NOTE_D5, 
  NOTE_F5, NOTE_D5, NOTE_F5, NOTE_F5, NOTE_F5
};

// 박자 지정 (빠른 비트에 맞춰 쪼갠 박자)
int noteDurations[] = {
  4, 4, 2,      // Ring Ding Dong
  4, 4, 2,      // Ring Ding Dong
  8, 8, 8, 8,   // Ring diggi
  8, 8, 4, 4, 2, // ding diggi ding ding ding
  
  4, 4, 2,      // Ring Ding Dong (음 한 단계 UP)
  4, 4, 2,      // Ring Ding Dong
  8, 8, 8, 8,   // Ring diggi
  8, 8, 4, 4, 2  // ding diggi ding ding ding
};

int totalNotes = sizeof(melody) / sizeof(int);
int currentNoteIndex = 0;       
unsigned long noteStartTime = 0; 
unsigned long noteDuration = 0;  
bool isPlayingNote = false;      
// -------------------------------------------------------------------

void setup() {
  Serial.begin(9600);

  // 핀 모드 설정
  pinMode(touch, INPUT);
  pinMode(button, INPUT);  
  pinMode(led, OUTPUT);
  pinMode(red_led, OUTPUT);
  pinMode(green_led, OUTPUT);
  pinMode(blue_led, OUTPUT);
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzer, OUTPUT);

  // 서보모터 초기화
  myServo.attach(robot); 
  myServo.write(0);      
}

void loop() {
  // 1. 센서 값 읽기 및 시리얼 처리
  bool touchState = digitalRead(touch);
  bool buttonState = digitalRead(button);

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  if (Serial.available() > 0) {
    temp_string = Serial.readStringUntil('\n');
    temp_string.trim(); 
    
    Serial.print("temp_string: ");
    Serial.println(temp_string);
  }

  // LED 및 RGB LED 제어
  if (touchState || temp_string == "LAMP_ON") {
    digitalWrite(led, HIGH);
  } else if (!touchState || temp_string == "LAMP_OFF") {
    digitalWrite(led, LOW);
  }

  if(temp_string == "red_on"){
    digitalWrite(green_led, LOW);    digitalWrite(blue_led, LOW);    digitalWrite(red_led, HIGH);
  }else if(temp_string == "green_on"){
    digitalWrite(red_led, LOW);    digitalWrite(blue_led, LOW);    digitalWrite(green_led, HIGH);    
  }else if(temp_string == "blue_on"){
    digitalWrite(red_led, LOW);    digitalWrite(green_led, LOW);    digitalWrite(blue_led, HIGH);
  }else if(temp_string == "rg_on"){
    digitalWrite(blue_led, LOW);   digitalWrite(red_led, HIGH);    digitalWrite(green_led, HIGH);    
  }else if(temp_string == "rb_on"){
    digitalWrite(blue_led, HIGH);  digitalWrite(red_led, HIGH);    digitalWrite(green_led, LOW);    
  }else if(temp_string == "red_off" || temp_string == "green_off" || temp_string == "blue_off"){
    digitalWrite(red_led, LOW);    digitalWrite(green_led, LOW);    digitalWrite(blue_led, LOW);
  }

  // 2. 버튼 누르는 동안 링딩동 연주
  if (buttonState) {
    myServo.write(180); 

    unsigned long currentMillis = millis();

    if (!isPlayingNote) {
      noteDuration = 550 / noteDurations[currentNoteIndex]; 
      tone(buzzer, melody[currentNoteIndex]);
      
      noteStartTime = currentMillis;
      isPlayingNote = true;
    }

    if (isPlayingNote && (currentMillis - noteStartTime >= noteDuration * 1.1)) {
      noTone(buzzer);
      isPlayingNote = false;
      
      currentNoteIndex++;
      if (currentNoteIndex >= totalNotes) {
        currentNoteIndex = 0; // 무한 중독 루프
      }
    }
  } else {
    myServo.write(0);     
    noTone(buzzer);       
    isPlayingNote = false; 
    currentNoteIndex = 0;  // 떼면 수능금지곡 강제 종료
  }

  Serial.println(cnt++);
  delay(10); 
}