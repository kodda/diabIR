#include <IRLib.h>
const int analogInPin0 = A0;
const int analogInPin1 = A1;
const int analogInPin2 = A2;
int inPin=12;
IRsend irsend;

int sensorValue0 = 0;
int sensorValue1 = 0;
int sensorValue2 = 0;
int outputValue0 = 0;
int outputValue1 = 0;
int outputValue2 = 0;
int val = 0; 

void setup() {
  Serial.begin(9600);
  pinMode(inPin, INPUT); 
}

void loop() {            
  sensorValue0 = analogRead(analogInPin0);            
  sensorValue1 = analogRead(analogInPin1);            
  sensorValue2 = analogRead(analogInPin2);            
  outputValue0 = map(sensorValue0, 0, 1023, 0, 255);  
  outputValue1 = map(sensorValue1, 0, 1023, 0, 255);  
  outputValue2 = map(sensorValue2, 0, 1023, 0, 255);  
  
  val = digitalRead(inPin);  // read input value
  Serial.println(outputValue0);
  if (outputValue0 >100) {
    irsend.send(SONY,0xa8bca, 20);
  }
    
  if (val == HIGH) {
    irsend.send(SONY,0xa8bca, 20);
  }

  //Serial.print(" ");      
  //Serial.print(outputValue1);      
  //Serial.print(" ");      
  //Serial.println(outputValue2);
  delay(10);              
}