#pragma once
#include <Arduino.h>

class IProtocol{
public:
  virtual void setup(void (*callback)(char*, byte*, unsigned int)) = 0;
  virtual void loop() = 0;
  virtual bool reconnect() = 0;
  virtual void sendMessage(const char* message) = 0;
};
