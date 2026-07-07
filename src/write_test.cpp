/* 이 스케치는 I2C 통신을 이용한 1602 LCD 를 사용하기 위해 
 * 주소를 스캐닝 하여 시리얼 모니터에 결과를 보여준다. 
 */ 
#include <Arduino.h>
#include <Wire.h> // I2C 통신을 위해 Wire 라이브러리를 포함한다. 

void setup() { 
  Wire.begin(); 
  Serial.begin(9600); // 9600 속도로 시리얼 통신 시작 
  while(!Serial); // 시리얼모니터를 기다린다. 
  Serial.println("\nI2C Scanner"); 
} 

void loop() { 
  byte error, address; 
  int nDevices; 
  Serial.println("Scanning..."); 
  nDevices = 0; 
  
  for (address = 1; address < 127; address++) { 
    // The i2c_scanner 는 장치가 주소를 승인했는지를 
    // 알기 위해 Write.endTransmission 반환 값을 사용한다.
    Wire.beginTransmission(address); 
    error = Wire.endTransmission(); 
    
    if (error == 0) { 
      Serial.print("I2C device found at address 0x"); 
      if(address < 16) 
        Serial.print("0"); 
      Serial.print(address, HEX); 
      Serial.println(" !"); 
      nDevices++; 
    } 
    else if(error == 4) { 
      Serial.print("Unknown error at address 0x"); 
      if (address < 16) 
        Serial.print("0"); 
      Serial.println(address, HEX); 
    } 
  } 
  if (nDevices == 0) 
    Serial.println("No I2C devices found\n"); 
  else 
    Serial.println("done\n"); 
    
  delay(5000); // 다음 스캔을 위해 5 초 기다린다. 
}