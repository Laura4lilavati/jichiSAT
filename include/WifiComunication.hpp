#pragma once
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "config.h" //ssid, password, mqtt server and other credentials
#include "IComunication.h"

class WifiComunication : public IComunication
{
public:
public:
  void setup() {
    Serial.begin(115200);
    WiFi.mode (WIFI_STA);
    WiFi.begin (WifiSSID, WifiPASS);
    Serial.print (F("connecting to: "));
    Serial.println (WifiSSID);
    while (WiFi.status() != WL_CONNECTED) {
      delay (500);
      Serial.print (".");
    }
    Serial.println(F("CONNECTED! \tIP address: "));
    Serial.println(WiFi.localIP());
  }

private:
  /* data */
};
