/* 이 스케치는 디지털 핀 12번에 연결된 온습도 센서 DHT11 모듈을 사용하여 
 * 주변의 온도와 습도를 측정하여 
 * I2C LCD 에 결과값을 출력한다. 
 */ 

// DHT11 모듈 사용을 위한 라이브러리 
#include <Arduino.h>
#include <Adafruit_Sensor.h> 
#include <DHT.h> 
#include <DHT_U.h> 


//I2C LCD 사용을 위한 라이브러리 
#include <Wire.h> 
#include <LiquidCrystal_I2C.h> 

// 온습도 센서 설정 
#define DHTPIN 12 // DHT 센서를 12 번에 연결한다. 
#define DHTTYPE DHT11 // DHT 11 를 사용한다. 

DHT_Unified dht(DHTPIN, DHTTYPE); // dht 객체를 형성한다. 

uint32_t delayMS; 

//LCD 설정 
LiquidCrystal_I2C lcd(0x27, 16, 2); // LCD I2C 주소를 설정한다. 16 칸 2줄 LCD 사용 

void setup() { 
  Serial.begin(9600); // 9600 속도로 시리얼 통신 시작 

  // LCD 초기화 
  lcd.init(); 
  lcd.backlight(); // 백 라이트를 켠다. 
  lcd.setCursor(0, 0); // 첫 번째 줄 첫째 칸 
  lcd.print("Hello~~~"); 
  lcd.setCursor(0, 1); // 두 번째 줄 첫째 칸 
  lcd.print("DHT Sensor Start"); 
  delay(1000); 
  lcd.clear(); 
  
  // DHT 센서 시작 
  dht.begin(); 
  Serial.println("DHT11 Unified Sensor Example"); 

  // 온도센서 정보 인쇄 
  sensor_t sensor; 
  dht.temperature().getSensor(&sensor); 
  Serial.println("------------------------------------"); 
  Serial.println("Temperature"); 
  Serial.print ("Sensor: "); Serial.println(sensor.name); 
  Serial.print ("Driver Ver: "); Serial.println(sensor.version); 
  Serial.print ("Unique ID: "); Serial.println(sensor.sensor_id); 
  Serial.print ("Max Value: "); Serial.print(sensor.max_value); Serial.println(" C"); 
  Serial.print ("Min Value: "); Serial.print(sensor.min_value); Serial.println(" C"); 
  Serial.print ("Resolution:"); Serial.print(sensor.resolution); Serial.println(" C"); 
  Serial.println("------------------------------------"); 

  // 습도센서 정보 인쇄 
  dht.humidity().getSensor(&sensor); 
  Serial.println("------------------------------------"); 
  Serial.println("Humidity"); 
  Serial.print ("Sensor: "); Serial.println(sensor.name); 
  Serial.print ("Driver Ver: "); Serial.println(sensor.version); 
  Serial.print ("Unique ID: "); Serial.println(sensor.sensor_id); 
  Serial.print ("Max Value: "); Serial.print(sensor.max_value); Serial.println("%"); 
  Serial.print ("Min Value: "); Serial.print(sensor.min_value); Serial.println("%"); 
  Serial.print ("Resolution: "); Serial.print(sensor.resolution); Serial.println("%"); 
  Serial.println("------------------------------------"); 

  // 센서를 읽는 시간 
  delayMS = sensor.min_delay / 1000; 
} 

void loop() { 
  // 측정 사이 지연 
  delay(delayMS); 

  sensors_event_t event; 
  
  // 온도의 값을 읽는다 
  dht.temperature().getEvent(&event); 
  if (isnan(event.temperature)) { 
    Serial.println("Error reading temperature!"); 
  } 
  else { 
    Serial.print("Temperature: "); 
    Serial.print(event.temperature); 
    Serial.println(" C"); 

    lcd.setCursor(0, 0); // 첫 번째 줄 첫째 칸 
    lcd.print("Temp : " ); // 메세지 출력 
    lcd.print(event.temperature, 0); // 측정 온도 소숫점 없이 출력 
    lcd.print(" [C]"); // 단위 출력 
  } 

  // 습도의 값을 읽는다 
  dht.humidity().getEvent(&event); 
  if (isnan(event.relative_humidity)) { 
    Serial.println("Error reading humidity!"); 
  } 
  else { 
    Serial.print("Humidity: "); 
    Serial.print(event.relative_humidity); 
    Serial.println("%"); 

    lcd.setCursor(0, 1); // 두 번째 줄 첫째 칸 
    lcd.print("Humidity: " ); // 메세지 출력 
    lcd.print(event.relative_humidity, 0); // 측정 습도 출력 
    lcd.print(" [%]"); // 단위 출력 
  } 

  // LCD 에 온도와 습도 결과 나타내기 
  delay(1000); // 안정적으로 값을 읽기 위해 1000밀리초 지연 
}