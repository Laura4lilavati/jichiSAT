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
//------- ---------------------- ------

WiFiClient myWifiClient;
UltrasonicHCSR04 ultrasonic(Trigger, Echo);
WifiComunication myWifiComunication;
MqttProtocol myMqttClient("esp8266_01", &myWifiComunication, myWifiClient);

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
/*
void handleNewMessages(int numNewMessages) {
  for (int i = 0; i < numNewMessages; i++) {
    if (bot.messages[i].type ==  F("callback_query")) {
      String text = bot.messages[i].text;
      Serial.print("Call back button pressed with text: ");
      Serial.println(text);
      if (text == F("ON")) {
        String chat_id = String(bot.messages[i].chat_id);
        String text = bot.messages[i].text;
        volatile long d;
        long t;
        digitalWrite(Trigger, HIGH);
        delayMicroseconds(10);
        digitalWrite(Trigger, LOW);
        t = pulseIn(Echo, HIGH);
        d = t/59;
        ultrasonic.readSensorData();
        d = ultrasonic.sensorData.distance;
        Serial.print("Distancia: ");
        Serial.print(d);
        Serial.print("cm");
        Serial.println();
        delay(100);
        String y= String(d);
        bot.sendMessage(chat_id, "EL **NIVEL DEL AGUA** ESTA A : "+y+" cm", "Markdown");
        if(d>20){
          bot.sendMessage(chat_id, "ESTA DENTRO EL RANGO DE LA **NORMALIDAD**", "Markdown");
        }else if(d<=19, d>=15){
            bot.sendMessage(chat_id, "ESTA DENTRO EL RANGO DE NIVEL **ANORMAL** CON TENDENCIA A RIESGO", "Markdown");
          }else if(d<=14, d>=10){
            bot.sendMessage(chat_id, "ESTA DENTRO EL RANGO DE NIVEL DE **RIESGO**, SE SOLICITA EVACUAR ZONAS DENTRO EL AREA 1", "Markdown");
          }else if(d<=9, d>=2){
            bot.sendMessage(chat_id, "ESTA DENTRO EL RANGO DE **DESBORDE**, SE SOLICITA EVACUAR ZONAS DENTRO DEL AREA 1 Y 2 ", "Markdown");
          }
        String keyboardButton = F("[[{ \"text\" : \"VER REPORTE JICHISAT\", \"callback_data\" : \"ON\" }]]");
        bot.sendMessageWithInlineKeyboard(chat_id, "JICHISAT ALERTA TEMPRANA - INUNDACIONES Y DESBORDES DE RIOS, ESTE PROTOTIPO SOLO SIRVE PARA VERIFICAR LA RECOPILACION DE DATOS EN TIEMPO REAL", "", keyboardButton);
      }
    }
    else {
      String chat_id = String(bot.messages[i].chat_id);
      String text = bot.messages[i].text;
      if (text == F("/start")) {
       String keyboardButton = F("[[{ \"text\" : \"VER REPORTE JICHISAT\", \"callback_data\" : \"ON\" }]]");
        bot.sendMessageWithInlineKeyboard(chat_id, "JICHISAT ES UNA ALERTA TEMPRANA ANTE INUNDACIONES Y DESBORDES DE RIOS, ESTE PROTOTIPO SOLO SIRVE PARA VERIFICAR LA RECOPILACION DE DATOS EN TIEMPO REAL", "", keyboardButton);
      }
     }
   }
  }
*/
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
