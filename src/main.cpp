#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
//#include "config.h" //ssid, password, mqtt server and other credentials

//#include "ISensor.h"
//#include "IComunication.h"
#include "UltrasonicHCSR04.hpp"
#include "WifiComunication.hpp"
#include "MqttProtocol.hpp"

const int Trigger = D0;
const int Echo = D1;
const char mqttClientID[] = "esp8266_01";
//------- ---------------------- ------

WiFiClient myWifiClient;
UltrasonicHCSR04 ultrasonic(Trigger, Echo);
WifiComunication myWifiComunication;
MqttProtocol myMqttClient(mqttClientID, &myWifiComunication, myWifiClient);

int delayBetweenChecks = 1000;
unsigned long lastTimeChecked;
unsigned long lightTimerExpires;
boolean lightTimerActive = false;

void callback(char* topic, byte* payload, unsigned int length) {
  //do something when mqtt messages arrive
}

void setup() {
  myWifiComunication.setup();
  myMqttClient.setup(callback);
  if (myMqttClient.reconnect()) {
    myMqttClient.sendMessage("prueba de data json 4");
  }else{
    //save to fileSystem, circular buffer littleFS
  }
  //Go to sleep for SLEEPTIME minutes
}

void loop() {
  volatile long d;
  int numNewMessages = 1;

  if (millis() > lastTimeChecked + delayBetweenChecks)  {
    if (numNewMessages) {
      Serial.println("got response");
    }
    lastTimeChecked = millis();
    if (lightTimerActive && millis() > lightTimerExpires) {
      lightTimerActive = false;
    }

    ultrasonic.readSensorData();
    d = ultrasonic.sensorData.distance;
    if (d < 15){
      myMqttClient.sendMessage("hola mundo");
      Serial.print("Distancia: ");
      Serial.print(d);
      Serial.print("cm");
      Serial.println();
    }
  }
}
