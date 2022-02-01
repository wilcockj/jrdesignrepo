#include <Arduino.h>
#include<Wire.h>

void TCA9548A(uint8_t bus)
{
  for(int i=0x70;i<=0x77;i++){
    Wire.beginTransmission(0x70);
    Wire.write(1 << bus);
    byte test = Wire.endTransmission();
    Serial.printf("Address %x returned status: %d\n",i,test);
  }
}

void setup(){
  Serial.begin(115200);
  Serial.println();
  Serial.println("Setting up");
  Wire.begin();
  Wire.setClock(100000);//I2C 400kHz
  
  TCA9548A(0);
  
}

void loop(){
   TCA9548A(0);
}
