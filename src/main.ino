#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include <config.h> //credentials

#include "ISensor.h"
#include "IComunication.h"
#include "UltrasonicHCSR04.hpp"

const int Trigger = D0;
const int Echo = D1;
//------- ---------------------- ------

WiFiClientSecure client;
UniversalTelegramBot bot(TELEGRAM_BOT_TOKEN, client);

UltrasonicHCSR04 ultrasonic(Trigger, Echo);

int delayBetweenChecks = 1000;
unsigned long lastTimeChecked;
unsigned long lightTimerExpires;
boolean lightTimerActive = false;

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  client.setInsecure();
  //bot.longPoll = 60;
  /*pinMode(Trigger, OUTPUT);
  pinMode(Echo, INPUT);
  digitalWrite(Trigger, LOW);*/
}

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
        /*long t;
        digitalWrite(Trigger, HIGH);
        delayMicroseconds(10);
        digitalWrite(Trigger, LOW);
        t = pulseIn(Echo, HIGH);
        d = t/59;*/
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
void loop() {
  //volatile long t;
  volatile long d;

  if (millis() > lastTimeChecked + delayBetweenChecks)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    if (numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
    }
    lastTimeChecked = millis();
    if (lightTimerActive && millis() > lightTimerExpires) {
      lightTimerActive = false;
    }

    /*digitalWrite(Trigger, HIGH);
    delayMicroseconds(10);
    digitalWrite(Trigger, LOW);
    t = pulseIn(Echo, HIGH);
    d = t/59;*/
    ultrasonic.readSensorData();
    d = ultrasonic.sensorData.distance;
    if (d < 15){
      Serial.print("Distancia: ");
      Serial.print(d);
      Serial.print("cm");
      Serial.println();
      String y= String(d);
      bot.sendMessage("642475044", "EL **NIVEL DEL AGUA** ESTA A : "+y+" cm", "Markdown");
      bot.sendMessage("642475044", "ESTA DENTRO EL RANGO DE **DESBORDE**, SE SOLICITA EVACUAR ZONAS DENTRO DEL AREA 1 Y 2 ", "Markdown");
    }
    //delay(100);
  }

}
//642475044 uska id
