#include<Wire.h>
#include<avr/pgmspace.h>
#define Addr_GND_GND 0xa8
void setup() {
// put your setup code here, to run once:
pinMode(13, OUTPUT);//ARDUINO BOARD LED control
Wire.begin();
Wire.setClock(400000);//I2C 400kHz
}
void loop() {
// put your main code here, to run repeatedly:
mainloop();
}
void IS_IIC_WriteByte(uint8_t Dev_Add,uint8_t Reg_Add,uint8_t Reg_Dat)//writing an LED register
{
Wire.beginTransmission(Dev_Add/2);
Wire.write(Reg_Add); // sends regaddress
Wire.write(Reg_Dat); // sends regaddress
Wire.endTransmission(); // stop transmitting
}
void mainloop(void)//
{
int i,j,k,l;
while(1)
{
digitalWrite(13, HIGH); // turn the ARDUINO BOARD LED on (HIGH is the voltage level)
IS_IIC_WriteByte(Addr_GND_GND,0x17,0x00);//reset IC
IS_IIC_WriteByte(Addr_GND_GND,0x00,0x01);//Enable SSD
IS_IIC_WriteByte(Addr_GND_GND,0x13,0x3f);//enable all LED channel
IS_IIC_WriteByte(Addr_GND_GND,0x14,0x3f);//enable all LED channel
IS_IIC_WriteByte(Addr_GND_GND,0x15,0x3f);//enable all LED channel
for(j=0;j<=0xFF;j++)
{
for(i=0;i<18;i=i+1)
{
IS_IIC_WriteByte(Addr_GND_GND,(0x01+i),j);//write all channel PWM with 0x10
}
IS_IIC_WriteByte(Addr_GND_GND,0x16,0x00);//update PWM and ON/OFF
delay(5);
}
digitalWrite(13, LOW); // turn the ARDUINO BOARD LED off by making the voltage LOW
IS_IIC_WriteByte(Addr_GND_GND,0x13,0x3f);//enable all LED channel
IS_IIC_WriteByte(Addr_GND_GND,0x14,0x3f);//enable all LED channel
IS_IIC_WriteByte(Addr_GND_GND,0x15,0x3f);//enable all LED channel
for(l=255;l>=0;l--)
{
for(k=0;k<18;k=k+1)
{
IS_IIC_WriteByte(Addr_GND_GND,(0x01+k),l);//write all channel PWM with 0x10
}
IS_IIC_WriteByte(Addr_GND_GND,0x16,0x00);//update PWM and ON/OFF
delay(5);
}
// IS_IIC_WriteByte(Addr_GND_GND,0x00,0x01);//release SSD to normal operation
delay(500); // wait for half a second
IS_IIC_WriteByte(Addr_GND_GND,0x17,0x00);//reset IC
}
}
