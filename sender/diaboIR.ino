#include <IRLib.h>
#include <Wire.h>

const int analogInPin0 = A0;
const int analogInPin1 = A1;
const int analogInPin2 = A2;
int inPin=12;
IRsend irsend;

/********************ACCELEROMETER DATAS************/
// adresss of accelerometer
int adress_acc=0X1D; // MMA8653FC and MMA8652FC
// adress of registers for MMA8653FC
int ctrl_reg1 = 0x2A;
int ctrl_reg2 = 0x2B;
int ctrl_reg3 = 0x2C;
int ctrl_reg4 = 0x2D;
int ctrl_reg5 = 0x2E;
int int_source = 0x0C;
int status_ = 0x00;
int f_setup = 0x09;
int out_x_msb = 0x01;
int out_y_msb = 0x03;
int out_z_msb = 0x05;
int sysmod = 0x0B;
int xyz_data_cfg = 0x0E;

/******PROGRAM DATAS**********/
int result [3];
int axeXnow ;
int axeYnow ;
int axeZnow ;

int sensorValue0 = 0;
int sensorValue1 = 0;
int sensorValue2 = 0;
int outputValue0 = 0;
int outputValue1 = 0;
int outputValue2 = 0;
int val = 0; 

void setup() {
  Wire.begin(); // start of the i2c protocol
  Serial.begin(9600);
  pinMode(inPin, INPUT); 
  ACC_INIT();
}

void loop() {            
  sensorValue0 = analogRead(analogInPin0);            
  sensorValue1 = analogRead(analogInPin1);            
  sensorValue2 = analogRead(analogInPin2);
  outputValue0 = map(sensorValue0, 0, 1023, 0, 255);  
  outputValue1 = map(sensorValue1, 0, 1023, 0, 255);  
  outputValue2 = map(sensorValue2, 0, 1023, 0, 255);  
  
  val = digitalRead(inPin);  // read input value
  Serial.println(outputValue1);
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
  I2C_READ_ACC(0x00); // to understand why 0x00 and not 0x01, look at the data-sheet p.19 or on the comments of the sub. This is valid only becaus we use auto-increment     
// NOTE: This communication is taking a little less than 1ms to be done. (for data-rate calculation for delay)
  //Serial.print("X: ");
  Serial.print(axeXnow); 
  //Serial.print(";");
  //Serial.print("Y: ");
  //Serial.print(axeYnow); 
  //Serial.print(";");
  //Serial.print("Z: ");
  //Serial.print(axeZnow); 
  //Serial.print(";");
  Serial.println();
delay(50);
      
}

void ACC_INIT()
{
    I2C_SEND(ctrl_reg1 ,0X00); // standby to be able to configure
    delay(10);
    
 //   I2C_SEND(xyz_data_cfg ,B00000000); // 2G full range mode
 //   delay(1);
    I2C_SEND(xyz_data_cfg ,B00000001); // 4G full range mode
    delay(1);
 //   I2C_SEND(xyz_data_cfg ,B00000010); // 8G full range mode
 //   delay(1);
    
 //   I2C_SEND(ctrl_reg1 ,B00000001); // Output data rate at 800Hz, no auto wake, no auto scale adjust, no fast read mode
 //   delay(1);
 //   I2C_SEND(ctrl_reg1 ,B00100001); // Output data rate at 200Hz, no auto wake, no auto scale adjust, no fast read mode
 //   delay(1);
    I2C_SEND(ctrl_reg1 ,B01000001); // Output data rate at 50Hz, no auto wake, no auto scale adjust, no fast read mode
    delay(1);
 //   I2C_SEND(ctrl_reg1 ,B01110001); // Output data rate at 1.5Hz, no auto wake, no auto scale adjust, no fast read mode
 //   delay(1);    
}

//------------------------------------------------------------------

void I2C_SEND(unsigned char REG_ADDRESS, unsigned  char DATA)  //SEND data to MMA8652
{
  
  Wire.beginTransmission(adress_acc);
  Wire.write(REG_ADDRESS);
  Wire.write(DATA);
  Wire.endTransmission();
}

//------------------------------------------------------------------

void I2C_READ_ACC(int ctrlreg_address) //READ number data from i2c slave ctrl-reg register and return the result in a vector
{
  byte REG_ADDRESS[7];
  int accel[4];
  int i=0; 
  Wire.beginTransmission(adress_acc); //=ST + (Device Adress+W(0)) + wait for ACK
  Wire.write(ctrlreg_address);  // store the register to read in the buffer of the wire library
  Wire.endTransmission(); // actually send the data on the bus -note: returns 0 if transmission OK-
  Wire.requestFrom(adress_acc,7); // read a number of byte and store them in wire.read (note: by nature, this is called an "auto-increment register adress")

  for(i=0; i<7; i++) // 7 because on datasheet p.19 if FREAD=0, on auto-increment, the adress is shifted
  // according to the datasheet, because it's shifted, outZlsb are in adress 0x00
  // so we start reading from 0x00, forget the 0x01 which is now "status" and make the adapation by ourselves
  //this gives:
  //0 = status
  //1= X_MSB
  //2= X_LSB
  //3= Y_MSB
  //4= Y_LSB
  //5= Z_MSB
  // 6= Z_LSB
  {
  REG_ADDRESS[i]=Wire.read(); //each time you read the write.read it gives you the next byte stored. The couter is reset on requestForm
}


// MMA8653FC gives the answer on 10bits. 8bits are on _MSB, and 2 are on _LSB
// this part is used to concatenate both, and then put a sign on it (the most significant bit is giving the sign)
// the explanations are on p.14 of the 'application notes' given by freescale.
      for (i=1;i<7;i=i+2)
      {
      accel[0] = (REG_ADDRESS[i+1]|((int)REG_ADDRESS[i]<<8))>>6; // X
        if (accel[0]>0x01FF) {accel[1]=(((~accel[0])+1)-0xFC00);} // note: with signed int, this code is optional
        else {accel[1]=accel[0];} // note: with signed int, this code is optional
            switch(i){
            case 1: axeXnow=accel[1];
                              break;
            case 3: axeYnow=accel[1];
                              break;
             case 5: axeZnow=accel[1];
                              break;
                }
       }

}

//------------------------------------------------------------------

void I2C_READ_REG(int ctrlreg_address) //READ number data from i2c slave ctrl-reg register and return the result in a vector
{
  unsigned char REG_ADDRESS;
  int i=0; 
  Wire.beginTransmission(adress_acc); //=ST + (Device Adress+W(0)) + wait for ACK
  Wire.write(ctrlreg_address);  // register to read
  Wire.endTransmission();
  Wire.requestFrom(adress_acc,1); // read a number of byte and store them in write received
}