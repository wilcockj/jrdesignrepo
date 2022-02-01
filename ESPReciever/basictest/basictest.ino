
/*
  Use the Qwiic Mux to access multiple I2C devices on seperate busses.
  By: Nathan Seidle @ SparkFun Electronics
  Date: May 17th, 2020
  License: This code is public domain but you buy me a beer if you use this
  and we meet someday (Beerware license).

  Some I2C devices respond to only one I2C address. This can be a problem
  when you want to hook multiple of a device to the I2C bus. An I2C Mux
  solves this issue by allowing you to change the 'channel' or port that
  the master is talking to.

  This example shows how to connect to different ports.
  The TCA9548A is a mux. This means when you enableMuxPort(2) then the SDA and SCL lines of the master (Arduino)
  are connected to port 2. Whatever I2C traffic you do, such as distanceSensor.startRanging() will be communicated to whatever
  sensor you have on port 2.

  Hardware Connections:
  Attach the Qwiic Mux Shield to your RedBoard or Uno.
  Plug a device into port 0 or 1
  Serial.print it out at 115200 baud to serial monitor.

  SparkFun labored with love to create this code. Feel like supporting open
  source? Buy a board from SparkFun!
  https://www.sparkfun.com/products/14685
*/

#include <Wire.h>

#include <SparkFun_I2C_Mux_Arduino_Library.h> //Click here to get the library: http://librarymanager/All#SparkFun_I2C_Mux
QWIICMUX myMux;

void WriteLedDriverByte(uint8_t Reg_Add,uint8_t Reg_Dat){
   //Serial.print("Writing %d",Reg_Dat);
  //Serial.print("Writing ");
  //Serial.print(Reg_Dat,HEX);
  //Serial.print("to address %d\n",Reg_Add);
  //Serial.print(" to address ");
  //Serial.print(Reg_Add,HEX);
  //Serial.println();
  Wire.beginTransmission(0xA8/2);//(MUX_Address/2);
  Wire.write(Reg_Add); // sends regaddress
  Wire.write(Reg_Dat); // sends regaddress
  Wire.endTransmission(); // stop transmitting
}

void setledon()
{
  int randnum = random(2);
  for(int pwmreg = 1; pwmreg <= 0x12; pwmreg++){
      randnum = random(2) * 255;
      WriteLedDriverByte(pwmreg,randnum);  
  }
  //delay(5);
  WriteLedDriverByte(0x16,00);
  //delay(50);
  for(int pwmreg = 1; pwmreg <= 0x12; pwmreg++){
    WriteLedDriverByte(pwmreg,0x0); // pwm off
  }
  //delay(5);
  WriteLedDriverByte(0x16,00);
  //delay(50);
}

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println("Qwiic Mux Shield Read Example");

  Wire.begin();
  Wire.setClock(400000);
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
      currentPortNumber = myMux.getPort();
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
  for (int i = 0; i < 2; i++) {
      //Serial.printf("Selecting mux %d\n",i);
      myMux.setPort(i);
      //Serial.printf("Setting leds on for board %d\n",i);
      setledon();
      delay(5);
    }
    //delay(500);
}
