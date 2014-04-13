#include <IRLib.h>
const int analogInPin0 = A0;
const int analogInPin1 = A1;
const int analogInPin2 = A2;
IRsend irsend;

int sensorValue0 = 0;
int sensorValue1 = 0;
int sensorValue2 = 0;
int outputValue0 = 0;
int outputValue1 = 0;
int outputValue2 = 0;

void setup() {
  Serial.begin(9600); 
}

void loop() {            
  sensorValue0 = analogRead(analogInPin0);            
  sensorValue1 = analogRead(analogInPin1);            
  sensorValue2 = analogRead(analogInPin2);            
  outputValue0 = map(sensorValue0, 0, 1023, 0, 255);  
  outputValue1 = map(sensorValue1, 0, 1023, 0, 255);  
  outputValue2 = map(sensorValue2, 0, 1023, 0, 255);  
  
  Serial.print(sensorValue0);
  Serial.print(" ");      
  Serial.print(sensorValue1);      
  Serial.print(" ");      
  Serial.println(sensorValue2);
  //irsend.send(SONY,sensorValue0, 32);
  delay(100);                     
}