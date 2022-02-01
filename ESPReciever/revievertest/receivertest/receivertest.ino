#include <TCA9548.h>
#include <Arduino.h>
#include<Wire.h>

//#include<pgmspace.h>
#define MUX_Address 0x70//0x70
TCA9548 MP(0x70);

// Initialize I2C buses using TCA9548A I2C Multiplexer
void tcaselect(uint8_t i2c_bus) {
    if (i2c_bus > 7) return;
    Wire.beginTransmission(MUX_Address);
    Wire.write(1 << i2c_bus);
    Wire.endTransmission(); 
}

void WriteByte(uint8_t Dev_Add,uint8_t Reg_Add,uint8_t Reg_Dat)
{
  //Serial.print("Writing %d",Reg_Dat);
  Serial.print("Writing ");
  Serial.print(Reg_Dat,HEX);
  //Serial.print("to address %d\n",Reg_Add);
  Serial.print(" to address ");
  Serial.print(Reg_Add,HEX);
  Serial.println();
  Wire.beginTransmission(Dev_Add/2);//(MUX_Address/2);
  Wire.write(Reg_Add); // sends regaddress
  Wire.write(Reg_Dat); // sends regaddress
  Wire.endTransmission(); // stop transmitting
}

void WriteLedDriverByte(uint8_t Reg_Add,uint8_t Reg_Dat){
   //Serial.print("Writing %d",Reg_Dat);
  Serial.print("Writing ");
  Serial.print(Reg_Dat,HEX);
  //Serial.print("to address %d\n",Reg_Add);
  Serial.print(" to address ");
  Serial.print(Reg_Add,HEX);
  Serial.println();
  Wire.beginTransmission(0xA8/2);//(MUX_Address/2);
  Wire.write(Reg_Add); // sends regaddress
  Wire.write(Reg_Dat); // sends regaddress
  Wire.endTransmission(); // stop transmitting
}

void setledon()
{
  int randnum = random(125);
  for(int pwmreg = 1; pwmreg <= 0x12; pwmreg++){
      randnum = random(125);
      WriteLedDriverByte(pwmreg,randnum);  
  }
  delay(5);
  WriteLedDriverByte(0x16,00);
  delay(50);
  for(int pwmreg = 1; pwmreg <= 0x12; pwmreg++){
    WriteLedDriverByte(pwmreg,0x0); // pwm off
  }
  delay(5);
  WriteLedDriverByte(0x16,00);
  delay(50);
}

void setup(){
  Serial.begin(115200);
  Serial.println();
  Serial.println("Setting up");
  Wire.begin();
  Wire.setClock(100000);//I2C 400kHz
  
  if (MP.begin() == false)
  {
    Serial.println("COULD NOT CONNECT");
  }
  
  for (int i = 0; i < 8; i++) {
      Serial.printf("Initializing board %d\n",i);
      MP.selectChannel(i);
      WriteLedDriverByte(0x17,0x00);//reset
      WriteLedDriverByte(0x00,0x01);//enable
      WriteLedDriverByte(0x13,0x3f);//enable leds
      WriteLedDriverByte(0x14,0x3f);//enable leds 2
      WriteLedDriverByte(0x15,0x3f);//enable led 3
    }
  
}


void loop(){
  for (int i = 0; i < 8; i++) {
      Serial.printf("Selecting mux %d\n",i);
      MP.selectChannel(i);
      Serial.printf("Setting leds on for board %d\n",i);
      setledon();
      delay(500);
    }
    //delay(500);
}
