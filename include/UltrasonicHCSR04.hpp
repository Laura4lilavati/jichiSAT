#pragma once
#include "Arduino.h"
#include "ISensor.h"
#include "UltrasonicHCSR04.hpp"

struct SensorData {
  long distance;
};

class UltrasonicHCSR04 : public ISensor
{
  const int& m_trigger;
  const int& m_echo;
public:
  UltrasonicHCSR04 (const int& trigger, const int& echo) : m_trigger(trigger), m_echo(echo){
    pinMode(trigger, OUTPUT);
    pinMode(echo, INPUT);
    digitalWrite(trigger, LOW);
  }
  SensorData sensorData;
  void readSensorData () override { //inline method
    digitalWrite(m_trigger, HIGH);
    delayMicroseconds(10);
    digitalWrite(m_trigger, LOW);
    sensorData.distance = pulseIn(m_echo, HIGH)/59;
  }
};
