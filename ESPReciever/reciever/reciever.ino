
/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-client-server-wi-fi/

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

#include <Arduino.h>
#include <C:\Users\james\AppData\Local\Arduino15\packages\esp32\hardware\esp32\1.0.6\libraries\WiFi\src\WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <pgmspace.h>
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
// Set your access point network credentials
const char *ssid = "Lattice";
const char *password = "123456789";
static int rgb[] = {255, 255, 255};
static int animation = -1;
static int audiomode = 0;
static char usermessage[2000];

int sensorPin = 35;  // select the input pin for the potentiometer
int sensorValue = 0; // variable to store the value coming from the sensor
DynamicJsonDocument doc(2048);
const char *PARAM_MESSAGE = "message";
/*#include <SPI.h>
#define BME_SCK 18
#define BME_MISO 19
#define BME_MOSI 23
#define BME_CS 5*/

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

void onRequest(AsyncWebServerRequest *request)
{
  // Handle Unknown Request
  request->send(404);
}

void onBody(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
  // Handle body
}

void onUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final)
{
  Serial.print("OH yeah");
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
  // Handle WebSocket event
}

void WriteLedDriverByte(uint8_t Reg_Add, uint8_t Reg_Dat)
{
  Wire.beginTransmission(0xA8 / 2); //(MUX_Address/2);
  Wire.write(Reg_Add);              // sends regaddress
  Wire.write(Reg_Dat);              // sends regaddress
  Wire.endTransmission();           // stop transmitting
}

void clearLayer()
{
  for (int pwmreg = 1; pwmreg <= 0x13; pwmreg++)
  {

    WriteLedDriverByte(pwmreg, 0);
    WriteLedDriverByte(0x16, 00);
    delay(FX_SPEED);
  }

  for (int pwmreg = 1; pwmreg <= 0x8; pwmreg++)
  {

    WriteLedDriverByte(pwmreg, 0);

    WriteLedDriverByte(0x16, 00);
    delay(FX_SPEED);
  }
}

void setallon()
{
  for (int x = 0; x < 2; x++)
  {
    myMux.setPort(x);
    for (int pwmreg = 1; pwmreg <= 0x13; pwmreg++)
    {

      WriteLedDriverByte(pwmreg, 255);

      WriteLedDriverByte(0x16, 00);
    }
    for (int pwmreg = 1; pwmreg <= 0x8; pwmreg++)
    {

      WriteLedDriverByte(pwmreg, 255);

      WriteLedDriverByte(0x16, 00);
    }
  }
}

void setalloff()
{
  for (int x = 0; x < 2; x++)
  {
    myMux.setPort(x);
    for (int pwmreg = 1; pwmreg <= 0x13; pwmreg++)
    {

      WriteLedDriverByte(pwmreg, 0);
      WriteLedDriverByte(0x16, 00);
    }

    for (int pwmreg = 1; pwmreg <= 0x8; pwmreg++)
    {

      WriteLedDriverByte(pwmreg, 0);

      WriteLedDriverByte(0x16, 00);
    }
  }
}

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println("Setting up");
  Wire.begin();
  Wire.setClock(100000); // I2C 400kHz

  pinMode(L_0, OUTPUT); // set both layers off
  pinMode(L_1, OUTPUT);
  digitalWrite(L_0, LOW);
  digitalWrite(L_0, LOW);

  if (myMux.begin() == false)
  {
    Serial.println("Mux not detected. Freezing...");
  }
  Serial.println("Mux detected");

  byte currentPortNumber = 0;
  for (int i = 0; i < 8; i++)
  {
    Serial.printf("Initializing board %d\n", i);
    myMux.setPort(i);
    currentPortNumber = myMux.getPort();
    Serial.print("CurrentPort: ");
    Serial.println(currentPortNumber);
    WriteLedDriverByte(0x17, 0x00); // reset
    WriteLedDriverByte(0x00, 0x01); // enable
    WriteLedDriverByte(0x13, 0x3f); // enable leds
    WriteLedDriverByte(0x14, 0x3f); // enable leds 2
    WriteLedDriverByte(0x15, 0x3f); // enable led 3
  }
  // pinMode(4,OUTPUT);//SDB
  // digitalWrite(4,HIGH);//SDB_HIGH
  // delay(100); //keep 0.5s
  // Serial port for debugging purposes

  // Setting the ESP as an access point
  Serial.print("Setting AP (Access Point)???");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);
  // WiFi.mode(WIFI_STA);
  // WiFi.begin(ssid, password);
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
      [](AsyncWebServerRequest *request) {},
      NULL,
      [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
      {
        static char message[200];
        int count = 0;
        for (size_t i = 0; i < len; i++)
        {
          Serial.write(data[i]);
          message[i] = data[i];
          count = i;
        }
        message[count + 1] = '\0';
        Serial.println();
        Serial.println(message);
        DeserializationError error = deserializeJson(doc, message);

        // Test if parsing succeeds.
        if (error)
        {
          Serial.print(F("deserializeJson() failed: "));
          Serial.println(error.f_str());
          return;
        }

        int red = doc["red"];
        int blue = doc["blue"];
        int green = doc["green"];

        int choice = doc["animation"];
        rgb[0] = red;
        rgb[1] = blue;
        rgb[2] = green;
        animation = choice;
        const char *word = doc["message"];
        audiomode = doc["audio"];
        strcpy(usermessage, word);
        Serial.printf("color = %d,%d,%d, animation = %d, message = %s\n", red, green, blue, choice, word);
        /*if (animation == 0)
        {
          // turn all leds off
          digitalWrite(L_1, HIGH);
          digitalWrite(L_0, HIGH);
          Serial.println("Should turn off leds");
          setalloff();
        }
        else if (animation == 1)
        {
          // turn all leds on
          digitalWrite(L_1, HIGH);
          digitalWrite(L_0, HIGH);
          Serial.println("Should turn on leds");
          setallon();
        }*/
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

void selectLayer(int layernum)
{
  switch (layernum)
  {
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

void setLayerOn(int chipNum)
{
  if (chipNum == 0)
  {
    for (int pwmreg = 1; pwmreg <= 0x13; pwmreg++)
    {
      if (checker == 0)
      {
        checker = 1;
        WriteLedDriverByte(pwmreg, 255);
      }
      else
      {
        checker = 0;
        WriteLedDriverByte(pwmreg, 0);
      }
      WriteLedDriverByte(0x16, 00);
      delay(FX_SPEED);
    }
  }
  else
  {
    for (int pwmreg = 1; pwmreg <= 0x8; pwmreg++)
    {
      if (checker == 0)
      {
        checker = 1;
        WriteLedDriverByte(pwmreg, 255);
      }
      else
      {
        checker = 0;
        WriteLedDriverByte(pwmreg, 0);
      }
      WriteLedDriverByte(0x16, 00);
      delay(FX_SPEED);
    }
  }
}

void printdata()
{
  Serial.printf("color = %d,%d,%d, animation = %d, message = %s,audiomode = %d sensor reading = %d\n", rgb[0], rgb[1], rgb[1], animation, usermessage, audiomode, sensorValue);
}

void loop()
{
  delay(500);
  printdata();
  sensorValue = analogRead(sensorPin);
  while (animation == 1)
  {
    delay(500);
    Serial.println("Doing anim 1");
  }
  while (animation == 2)
  {
    delay(500);
    Serial.println("Doing anim 2");
  }
  while (audiomode)
  {
    int time = millis();
    int maxvalue = 0;
    Serial.printf("millis = %d\n", time);
    Serial.println("Inside audio mode");
    while (time - millis() < 5000)
    {
      sensorValue = analogRead(sensorPin);
      Serial.println(sensorValue);
      if (sensorValue > maxvalue)
      {
        maxvalue = sensorValue;
      }
    }
    // need to define tresholds
    /* maybe look for peaks over a certain time period
     *  say .2sec
     * switch
     *  value < 400:
     *    turn all off
     *  600 > value > 400:
     *      turn on middle, [2][2]
     *  800 > value > 600:
     *      turn on [1-3][1-3]
     *  value > 800
     *      turn all on
     */
  }
}
