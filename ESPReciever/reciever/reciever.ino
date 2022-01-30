
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
#define MUX_Address 0x70

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

// Initialize I2C buses using TCA9548A I2C Multiplexer
void tcaselect(uint8_t i2c_bus) {
    if (i2c_bus > 7) return;
    Wire.beginTransmission(MUX_Address);
    Wire.write(1 << i2c_bus);
    Wire.endTransmission(); 
}


void onRequest(AsyncWebServerRequest *request){
  //Handle Unknown Request
  request->send(404);
}

void onBody(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){
  //Handle body
}


String readTemp(){
  return "98";
}

void onUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final){
  Serial.print("OH yeah");
}

void onEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len){
  //Handle WebSocket event
}

void WriteByte(uint8_t Reg_Add,uint8_t Reg_Dat)
{
  Serial.printf("Writing %x to address: %x\n",Reg_Dat,Reg_Add);
  Wire.beginTransmission(MUX_Address/2);
  Wire.write(Reg_Add); // sends regaddress
  Wire.write(Reg_Dat); // sends regaddress
  Wire.endTransmission(); // stop transmitting
}


void setup(){
  Serial.begin(115200);
  Serial.println();
  Serial.println("Setting up");
  Wire.begin();
  Wire.setClock(400000);//I2C 400kHz
  WriteByte(0x17,0x00);//reset
  WriteByte(0x00,0x01);//enable
  WriteByte(0x13,0x3f);//enable leds
  WriteByte(0x14,0x3f);//enable leds 2
  WriteByte(0x15,0x3f);//enable led 3
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

  
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readTemp().c_str());
  });
  /*
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readHumi().c_str());
  });
  server.on("/pressure", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readPres().c_str());
  });*/
  server.on("/upload", HTTP_POST, [](AsyncWebServerRequest *request){
    Serial.println("uploaded\n");
    String message;
    
    if (request->hasParam("data",true)) {
            Serial.println("setting message");
            message = request->getParam("data", true)->value();
        } else {
            message = "No message sent";
        }
    Serial.printf("here\n");
    int args = request->args();
    
    for(int i=0;i<args;i++){
      Serial.printf("ARG[%s]: %s\n", request->argName(i).c_str(), request->arg(i).c_str());
      // message = request->arg(i).c_str();
      /*
      if (request->argName(i).c_str() == 'data')
      {
        //deserializeJson(doc,request->arg(i));
        message = request->arg(i).c_str();
      }*/
    }
    
    request->send(200,"text/plain", "Sent data" + message);
   
  }, onUpload);

  server.on("/post", HTTP_POST, [](AsyncWebServerRequest *request){
        String message;
        if (request->hasParam(PARAM_MESSAGE, true)) {
            message = request->getParam(PARAM_MESSAGE, true)->value();
        } else {
            message = "No message sent";
        }
        if (request->hasParam("data", true)) {
            message = request->getParam("data", true)->value();
        } else {
            message = "No message sent";
        }
        request->send(200, "text/plain", "Hello, POST: " + message);
    });

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
      
      const char* color = doc["color"];
      Serial.printf("color = %s",color);
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

void setledon()
{
  for(int pwmreg = 1; pwmreg <= 0x12; pwmreg++){
    WriteByte(pwmreg,0x1f);  
  }
  delay(5);
  WriteByte(0x16,00);
  delay(1000);
  for(int pwmreg = 1; pwmreg <= 0x12; pwmreg++){
    WriteByte(pwmreg,0x0); // pwm off
  }
  delay(5);
  WriteByte(0x16,00);
  delay(1000);
}

void loop(){
  for (int i = 0; i < 2; i++) {
      Serial.printf("Selecting mux %d\n",i);
      tcaselect(i);
      Serial.printf("Setting leds on for board %d\n",i);
      setledon();
      delay(50);
    }
    delay(500);
}
