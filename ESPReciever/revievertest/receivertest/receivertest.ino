//#include <TCA9548.h>
//#include <Arduino.h>
#include<Wire.h>

#include <SparkFun_I2C_Mux_Arduino_Library.h> //Click here to get the library: http://librarymanager/All#SparkFun_I2C_Mux
QWIICMUX myMux;
//#include<pgmspace.h>
//#define MUX_Address 0x70//0x70

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

  if (myMux.begin() == false)
  {
    Serial.println("Mux not detected. Freezing...");
    while (1)
      ;
  }
  Serial.println("Mux detected");
  
  byte currentPortNumber = 0;
  for (int i = 0; i < 8; i++) {
      Serial.printf("Initializing board %d\n",i);
      myMux.setPort(i);
      byte currentPortNumber = myMux.getPort();
      Serial.print("CurrentPort: ");
      Serial.println(currentPortNumber);
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
      myMux.setPort(i);
      Serial.printf("Setting leds on for board %d\n",i);
      setledon();
      delay(500);
    }
    //delay(500);
}
