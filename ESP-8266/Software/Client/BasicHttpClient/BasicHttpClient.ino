/**
   BasicHTTPClient.ino

    Created on: 24.05.2015

*/

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#include <WiFiClient.h>

ESP8266WiFiMulti WiFiMulti;

void setup() {

  Serial.begin(9600);
  // Serial.setDebugOutput(true);

  Serial.println();
  Serial.println();
  Serial.println();

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("sew", "1sgdttsa");
/*
  String ssid = String("sew");
  String password = String("1sgdttsa");
#define BUF_LEN 100
  
    Serial.print("Connecting to ");
    Serial.print(ssid);


  unsigned char sBuf[BUF_LEN] = {0x00};
  unsigned char pBuf[BUF_LEN] = {0x00};
  if(ssid.length() < BUF_LEN)
  {
    ssid.getBytes(sBuf, ssid.length());
  } else
  {
 
      Serial.println("ssid is too long for the buffer.");
  }
  if(password.length() < BUF_LEN)
  {
    password.getBytes(pBuf, password.length());
  } else
  {
 
      Serial.println("password is too long for the buffer.");
  }   
  
  WiFi.mode(WIFI_STA);

  // These casts are a bit nasty...
  
  WiFiMulti.addAP((char*)sBuf, (char*)pBuf);

  char wCount = 0;
  do
  {
    delay(2000);

    {
      Serial.print(" .");
      wCount++;
      if(!wCount%50)
      {
        Serial.println();
        wCount = 0;  
      }
    }
  } while (WiFiMulti.run() != WL_CONNECTED);


  {
    Serial.println(" connected.");
  }  
*/
}

void loop() {
  // wait for WiFi connection
  if ((WiFiMulti.run() == WL_CONNECTED)) {

    WiFiClient client;

    HTTPClient http;

    Serial.print("[HTTP] begin...\n");
    if (http.begin(client, "http://jigsaw.w3.org/HTTP/connection.html")) {  // HTTP


      Serial.print("[HTTP] GET...\n");
      // start connection and send HTTP header
      int httpCode = http.GET();

      // httpCode will be negative on error
      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTP] GET... code: %d\n", httpCode);

        // file found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = http.getString();
          Serial.println(payload);
        }
      } else {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }

      http.end();
    } else {
      Serial.printf("[HTTP} Unable to connect\n");
    }
  }

  delay(10000);
}
