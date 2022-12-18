#pragma once
#include "Arduino.h"
#include "IComunication.h"
#define TINY_GSM_MODEM_SIM800
#ifdef ESP32
#define MODEM_TX 27
#define MODEM_RX 26
#define SerialAT Serial1
//#else
//#include "SoftwareSerial.h"
//#define MODEM_TX 2
//#define MODEM_RX 3
//SoftwareSerial SerialAT(MODEM_TX, MODEM_RX);
#endif
class gprsSIM800 : public IComunication
{
public:
//  gprsSIM800(arguments);

private:
  /* data */
};
