#ifndef SLAVEARDUINO_HEADER_INCLUDED
#define SLAVEARDUINO_HEADER_INCLUDED

#include "slavemgr.hpp"
#include <arduino.h>

class SlaveArduino: public SlaveMgr
{
  private:
  	void send(unsigned char *buffer,int size) override
  	{
  	  digitalWrite(tx_pin,HIGH);
  	  delay(15);
  	  Serial.write(buffer,size);
  	  while ((UCSR0A & _BV (TXC0)) == 0) {}
  	  delay(5);
  	  digitalWrite(tx_pin,LOW);
  	}
  
  	unsigned long getTick(void) override
  	{
  	  return millis();
  	}

  public:
	  SlaveArduino() {}
	  ~SlaveArduino() {}

    void onSerialEvent(void) override
    {
      while (Serial.available()>0)
      {        
    	int b=Serial.read();
    	if ( (b>0) && (b<256) )        
    	  rxFrame.recv((unsigned char)b);
      }
    }

    void begin(int tx_pin,unsigned char addr) override
    {
      SlaveMgr::begin(tx_pin,addr);
      
      pinMode(tx_pin,OUTPUT);
      digitalWrite(tx_pin,LOW);
      
      Serial.begin(SERIAL_SPEED);
    }
};

SlaveArduino Slave;

#endif
