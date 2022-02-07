
/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-client-server-wi-fi/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

#include <Arduino.h>
#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include<Wire.h>
#include<pgmspace.h>
#include <SparkFun_I2C_Mux_Arduino_Library.h>
QWIICMUX myMux;

#define L_0 15
#define L_1 4
#define L_2 18
#define L_3 19
#define L_4 21
#define L_5 22
#define L_6 23
#define FX_SPEED 50
static int checker = 0;
byte PWM_Gamma64[64]=
{
0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,
0x08,0x09,0x0b,0x0d,0x0f,0x11,0x13,0x16,
0x1a,0x1c,0x1d,0x1f,0x22,0x25,0x28,0x2e,
0x34,0x38,0x3c,0x40,0x44,0x48,0x4b,0x4f,
0x55,0x5a,0x5f,0x64,0x69,0x6d,0x72,0x77,
0x7d,0x80,0x88,0x8d,0x94,0x9a,0xa0,0xa7,
0xac,0xb0,0xb9,0xbf,0xc6,0xcb,0xcf,0xd6,
0xe1,0xe9,0xed,0xf1,0xf6,0xfa,0xfe,0xff
};

// Set your access point network credentials
const char* ssid = "Lattice";
const char* password = "123456789";
DynamicJsonDocument doc(2048);
const char* PARAM_MESSAGE = "message";
/*#include <SPI.h>
#define BME_SCK 18
#define BME_MISO 19
#define BME_MOSI 23
#define BME_CS 5*/

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);



void onRequest(AsyncWebServerRequest *request){
  //Handle Unknown Request
  request->send(404);
}

void onBody(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){
  //Handle body
}

void onUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final){
  Serial.print("OH yeah");
}

void onEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len){
  //Handle WebSocket event
}

void WriteLedDriverByte(uint8_t Reg_Add,uint8_t Reg_Dat){
  Wire.beginTransmission(0xA8/2);//(MUX_Address/2);
  Wire.write(Reg_Add); // sends regaddress
  Wire.write(Reg_Dat); // sends regaddress
  Wire.endTransmission(); // stop transmitting
}

void clearLayer(){
    for(int pwmreg = 1; pwmreg <= 0x13; pwmreg++){
        
          WriteLedDriverByte(pwmreg,0);
        WriteLedDriverByte(0x16, 00);
        delay(FX_SPEED);
    }
 
    for(int pwmreg = 1; pwmreg <= 0x8; pwmreg++){

          WriteLedDriverByte(pwmreg,0);

        WriteLedDriverByte(0x16, 00);
        delay(FX_SPEED);
    }
 
}

void setallon(){
  for(int x = 0; x < 2; x++){
    myMux.setPort(x);
  for(int pwmreg = 1; pwmreg <= 0x13; pwmreg++){
        
          WriteLedDriverByte(pwmreg,255);
        WriteLedDriverByte(0x16, 00);
    }
 
    for(int pwmreg = 1; pwmreg <= 0x8; pwmreg++){

          WriteLedDriverByte(pwmreg,255);

        WriteLedDriverByte(0x16, 00);
    }
  }
}

void setalloff(){
  for(int x = 0; x < 2; x++){
    myMux.setPort(x);
    for(int pwmreg = 1; pwmreg <= 0x13; pwmreg++){
          
            WriteLedDriverByte(pwmreg,0);
          WriteLedDriverByte(0x16, 00);
      }
   
      for(int pwmreg = 1; pwmreg <= 0x8; pwmreg++){
  
            WriteLedDriverByte(pwmreg,0);
  
          WriteLedDriverByte(0x16, 00);
      }
  }
}

void setup(){
  Serial.begin(115200);
  Serial.println();
  Serial.println("Setting up");
  Wire.begin();
  Wire.setClock(400000);//I2C 400kHz
  
  pinMode(L_0, OUTPUT); //set both layers off
  pinMode(L_1, OUTPUT);
  digitalWrite(L_0, LOW);
  digitalWrite(L_0, LOW);
  
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
  // pinMode(4,OUTPUT);//SDB
  // digitalWrite(4,HIGH);//SDB_HIGH
  //delay(100); //keep 0.5s
  // Serial port for debugging purposes
 
  
  // Setting the ESP as an access point
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);
  //WiFi.mode(WIFI_STA);
  //WiFi.begin(ssid, password);
  /*
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.printf("WiFi Failed!\n");
    return;
  }*/
  // attach AsyncWebSocket

  // attach AsyncEventSource
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

    server.on(
    "/posttest",
    HTTP_POST,
    [](AsyncWebServerRequest * request){},
    NULL,
    [](AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total) {
      static char message[200];\
      int count;
      for (size_t i = 0; i < len; i++) {
        Serial.write(data[i]);
        message[i] = data[i];
        count = i;
      }
      message[count+1] = '\0';
      Serial.println();
      Serial.println(message);
      DeserializationError error = deserializeJson(doc, message);

      // Test if parsing succeeds.
      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
      }
      
      int red = doc["red"];
      int blue = doc["blue"];
      int green = doc["green"];
      int choice = doc["animation"];
      const char* word = doc["message"];
      Serial.printf("color = %d,%d,%d, animation = %d, message = %s",red,green,blue,choice,word);
      if(choice == 0){
        //turn all leds off 
        digitalWrite(L_1, HIGH);
        digitalWrite(L_0, HIGH);
        Serial.println("Should turn off leds");
        setalloff();
      }
      else if(choice == 1){
        //turn all leds on
        digitalWrite(L_1, HIGH);
        digitalWrite(L_0, HIGH);
        Serial.println("Should turn on leds");
        setallon();
      }
      request->send(200);
  });
  
  bool status;

  // Catch-All Handlers
  // Any request that can not find a Handler that canHandle it
  // ends in the callbacks below.
  server.onNotFound(onRequest);
  server.onFileUpload(onUpload);
  server.onRequestBody(onBody);
  
  // Start server
  server.begin();
}

void selectLayer(int layernum){
  switch(layernum){
    case 0:
        digitalWrite(L_0, HIGH);
        digitalWrite(L_1, LOW);
        digitalWrite(L_2, LOW);
        digitalWrite(L_3, LOW);
        digitalWrite(L_4, LOW);
        digitalWrite(L_5, LOW);
        digitalWrite(L_6, LOW);
        break;
    case 1:
        digitalWrite(L_0, LOW);
        digitalWrite(L_1, HIGH);
        digitalWrite(L_2, LOW);
        digitalWrite(L_3, LOW);
        digitalWrite(L_4, LOW);
        digitalWrite(L_5, LOW);
        digitalWrite(L_6, LOW);
        break;
    case 2:
        digitalWrite(L_0, LOW);
        digitalWrite(L_1, LOW);
        digitalWrite(L_2, HIGH);
        digitalWrite(L_3, LOW);
        digitalWrite(L_4, LOW);
        digitalWrite(L_5, LOW);
        digitalWrite(L_6, LOW);
        break;
    case 3:
        digitalWrite(L_0, LOW);
        digitalWrite(L_1, LOW);
        digitalWrite(L_2, LOW);
        digitalWrite(L_3, HIGH);
        digitalWrite(L_4, LOW);
        digitalWrite(L_5, LOW);
        digitalWrite(L_6, LOW);
        break;
    case 4:
        digitalWrite(L_0, LOW);
        digitalWrite(L_1, LOW);
        digitalWrite(L_2, LOW);
        digitalWrite(L_3, LOW);
        digitalWrite(L_4, HIGH);
        digitalWrite(L_5, LOW);
        digitalWrite(L_6, LOW);
        break;
    case 5:
        digitalWrite(L_0, LOW);
        digitalWrite(L_1, LOW);
        digitalWrite(L_2, LOW);
        digitalWrite(L_3, LOW);
        digitalWrite(L_4, LOW);
        digitalWrite(L_5, HIGH);
        digitalWrite(L_6, LOW);
        break;
    case 6:
        digitalWrite(L_0, LOW);
        digitalWrite(L_1, LOW);
        digitalWrite(L_2, LOW);
        digitalWrite(L_3, LOW);
        digitalWrite(L_4, LOW);
        digitalWrite(L_5, LOW);
        digitalWrite(L_6, HIGH);
        break;
  }
}

void setLayerOn(int chipNum) {
  if (chipNum == 0) {
    for(int pwmreg = 1; pwmreg <= 0x13; pwmreg++){
        if(checker == 0){
          checker = 1;
          WriteLedDriverByte(pwmreg,255);
        }
        else{
          checker = 0;
          WriteLedDriverByte(pwmreg,0);
        }
        WriteLedDriverByte(0x16, 00);
        delay(FX_SPEED);
    }
  } 
  else {
    for(int pwmreg = 1; pwmreg <= 0x8; pwmreg++){
        if(checker == 0){
          checker = 1;
          WriteLedDriverByte(pwmreg,255);
        }
        else{
          checker = 0;
          WriteLedDriverByte(pwmreg,0);
        }
        WriteLedDriverByte(0x16, 00);
        delay(FX_SPEED);
    }
  }
}

void loop(){
}
