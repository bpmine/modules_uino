#ifndef MASTERARDUINO_HEADER_INCLUDED
#define MASTERARDUINO_HEADER_INCLUDED

#include "master.h"

#include <arduino.h>

class MasterArduino : public Master
{
private:
  HardwareSerial* pStr;
  int txen;

  void sendBytes(unsigned char* buffer, int size) override
  {
    if (pStr == nullptr)
      return;

    pStr->write(buffer,size);
  }

  int available(void) override
  {
	return pStr->available();
  }

  int readByte(void) override
  {
	return pStr->read();
  }


public:
  MasterArduino()
  {
    txen = -1;
    pStr = nullptr;
  }

  void begin(HardwareSerial* pSerial, int txen)
  {
    this->txen = txen;
    pinMode(this->txen, OUTPUT);
    digitalWrite(this->txen, LOW);
    pSerial->begin(BUS_RS485_SPEED);
    this->pStr = pSerial;

    init();
  }
};

MasterArduino Master;

#endif
