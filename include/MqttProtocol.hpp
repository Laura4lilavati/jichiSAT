#pragma once
#include <Arduino.h>
#include "PubSubClient.h"
#include "config.h" //ssid, password, mqtt server and other credentials
#include "IComunication.h"
#include "IProtocol.h"

#define retryNumber 4

template<typename T>
class MqttProtocol : public IProtocol
{
public:
  const char* _mqttClientId;
  IComunication* _comunication;
  PubSubClient _mqttClient;
public:
  MqttProtocol (const char* ClientID, IComunication* comunication, T client):
                _mqttClientId(ClientID),
                _comunication(comunication),
                _mqttClient(client)
  {}

  void setup(void (*callback)(char*, byte*, unsigned int)){
    _comunication->setup();
    _mqttClient.setServer (mqtt_server, 1883);
    _mqttClient.setCallback(callback);
  }

  void loop(){
    if (!_mqttClient.connected()) {
      if (_mqttClient.connect(_mqttClientId)){
        //_mqttClient.publish("myTopic", "connected");
        _mqttClient.subscribe("mySubscribeTopic");
      }else{
        Serial.print (F("failed, rc="));
        Serial.println (_mqttClient.state());
      }
    }else{
      _mqttClient.loop();
    }
  }

  bool reconnect()
  {
    byte retryingCount = 0;
    while (!_mqttClient.connected()) {
      if (_mqttClient.connect(_mqttClientId)) {
        _mqttClient.subscribe("mySubscribeTopic");
          return true;
      }else{
        retryingCount++;
        if (retryingCount == retryNumber) {
          Serial.println("retried 4 times with no luck");
          break;
        }
      }
    }
    return false;
  }

  void sendMessage(const char* message){
    _mqttClient.publish("outTopic", message);
  }
//  virtual ~MqttProtocol ();

private:
  /* data */
};

