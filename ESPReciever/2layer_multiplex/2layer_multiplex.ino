#include <Wire.h>
#include <SparkFun_I2C_Mux_Arduino_Library.h>
QWIICMUX myMux;

#define L_0 15
#define L_1 4
#define FX_SPEED 50

void WriteLedDriverByte(uint8_t Reg_Add,uint8_t Reg_Dat){
  Wire.beginTransmission(0xA8/2);//(MUX_Address/2);
  Wire.write(Reg_Add); // sends regaddress
  Wire.write(Reg_Dat); // sends regaddress
  Wire.endTransmission(); // stop transmitting
}

void setLayerOn(int chipNum) {
  if (chipNum == 0) {
    for(int pwmreg = 1; pwmreg <= 0x13; pwmreg++){
        WriteLedDriverByte(pwmreg, 255);
        if (pwmreg != 1){
          WriteLedDriverByte(pwmreg-1, 0);
        }
        WriteLedDriverByte(0x16, 00);
        delay(FX_SPEED);
    }
  } 
  else {
    for(int pwmreg = 1; pwmreg <= 0x8; pwmreg++){
        WriteLedDriverByte(pwmreg, 255);
        if (pwmreg != 1){
          WriteLedDriverByte(pwmreg-1, 0);
        }
        WriteLedDriverByte(0x16, 00);
        delay(FX_SPEED);
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println();
  Serial.println("Qwiic Mux Shield Read Example");

  pinMode(L_0, OUTPUT); //set both layers off
  pinMode(L_1, OUTPUT);
  digitalWrite(L_0, LOW);
  digitalWrite(L_0, LOW);

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

void loop() {
  // put your main code here, to run repeatedly:

  for (int layer = 0; layer < 2; layer++) {
    if (layer == 0) {
      digitalWrite(L_0, HIGH);
      digitalWrite(L_1, LOW);
      for (int i = 0; i < 2; i++) {
        myMux.setPort(i);
        setLayerOn(i);
      } 
    }
    else if (layer == 1) {
      digitalWrite(L_0, LOW);
      digitalWrite(L_1, HIGH);
      for (int i = 0; i < 2; i++) {
        myMux.setPort(i);
        setLayerOn(i);
      } 
    }
    //delay(100);
  }
}
