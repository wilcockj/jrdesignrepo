#include <Arduino.h>
#include<Wire.h>
//#include<pgmspace.h>
#define MUX_Address 0xA8//0x70

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

void setledon()
{
  int randnum = random(255);
  for(int pwmreg = 1; pwmreg <= 0x3; pwmreg++){
      randnum = random(255);
      WriteByte(MUX_Address,pwmreg,randnum);  
  }
  delay(5);
  WriteByte(MUX_Address,0x16,00);
  delay(1000);
  for(int pwmreg = 1; pwmreg <= 0x12; pwmreg++){
    WriteByte(MUX_Address,pwmreg,0x0); // pwm off
  }
  delay(5);
  WriteByte(MUX_Address,0x16,00);
  delay(1000);
}

void setup(){
  Serial.begin(115200);
  Serial.println();
  Serial.println("Setting up");
  Wire.begin();
  Wire.setClock(400000);//I2C 400kHz
  WriteByte(MUX_Address,0x17,0x00);//reset
  WriteByte(MUX_Address,0x00,0x01);//enable
  WriteByte(MUX_Address,0x13,0x3f);//enable leds
  WriteByte(MUX_Address,0x14,0x3f);//enable leds 2
  WriteByte(MUX_Address,0x15,0x3f);//enable led 3
}


void loop(){
  for (int i = 0; i < 2; i++) {
      //Serial.print("Selecting mux %d\n",i);
      //tcaselect(i);
      //Serial.print("Setting leds on for board %d\n",i);
      setledon();
      delay(50);
    }
    delay(500);
}
