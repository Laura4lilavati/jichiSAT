#pragma once
#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "config.h" //ssid, password, mqtt server and other credentials
#include "IComunication.h"

class wifiComunication : public IComunication
{
  const char* mqttClientID;
  int* myData;
  WiFiClient myWifiClient;
  PubSubClient myMqttClient{myWifiClient};
public:
  void loop(){
    if (!myMqttClient.connected()) {
      if (myMqttClient.connect(mqttClientID)){
        myMqttClient.publish("myTopic", "connected");
        myMqttClient.subscribe("mySubscribeTopic");
      }else{
        Serial.print (F("failed, rc="));
        Serial.println (myMqttClient.state());
      }
    }else{
      myMqttClient.loop();
    }
  }

  void sendData (int* jsonData) override {
    myMqttClient.publish("myTopic", "hola mundo");
  }

  static void setCallback(wifiComunication* wCom){
    auto callback = [&wCom] (char* topic, byte* payload, unsigned int length) {
      int a = 77;
      wCom->myData = &a;
      Serial.print ("Message arrived [");
      Serial.print (topic);
      Serial.print ("] ");
      //instead of this bellow make a jsonData
      for (unsigned int i = 0; i < length; i++) {
        Serial.print ((char)payload[i]);
      }
      Serial.println();
      // Switch on the LED if an 1 was received as first character
      // INSTEAD change this to recieve an int for servo's position
      if ((char)payload[0] == '1') {
        digitalWrite (LED_BUILTIN, LOW);  // Turn the LED on (Note that LOW is the voltage level
        // but actually the LED is on; this is because
        // it is active low on the ESP-01)
      } else {
        digitalWrite (LED_BUILTIN, HIGH); // Turn the LED off by making the voltage HIGH
      }
    };
    wCom->myMqttClient.setCallback(callback);
  }

  int* receiveData() override {
    return myData;
  }

  wifiComunication (const char* clientID): mqttClientID(clientID){
    Serial.begin (115200);
    WiFi.mode (WIFI_STA);
    WiFi.begin (WifiSSID, WifiPASS);
    Serial.print (F("connecting to: "));
    Serial.println (WifiSSID);
    while (WiFi.status() != WL_CONNECTED) {
      delay (500);
      Serial.print (".");
    }
    Serial.println(F("CONNECTED"));
    myMqttClient.setServer (mqtt_server, 1883);
    wifiComunication::setCallback(this); //passing the actual instance
  };

private:
  /* data */
};
