#include <Arduino.h>


int a = 0;
int mic = 3;
int thermistor = 4;

void setup() {
    pinMode(a, INPUT);
    pinMode(mic, INPUT);
    pinMode(thermistor, INPUT);
    Serial.begin(9600);
}

void loop() {
    int A0_read_data = analogRead(a);
    int mic_data = analogRead(mic);
    int thermistor_data = analogRead(thermistor);
    if(A0_read_data > 0){
        Serial.print("A0_read_data :");
        Serial.println(A0_read_data );
    }

    if(mic_data){
        Serial.print("mic_data :");
        Serial.println(mic_data );
    }
    if(thermistor_data){
        Serial.print("thermistor_data :");
        Serial.println(thermistor_data );
    }
    
    
    delay(1000);
}