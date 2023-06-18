#include "master.hpp"

#define PIN_TX_EN         (6)

Master master;

unsigned long ulT0;

void serialEvent1(void)
{
  //master.recv();
}

void setup() 
{
  Serial.begin(9600);
  master.begin(&Serial1,PIN_TX_EN);
  Serial.println("Boot");
  ulT0=millis();
} 

void loop() 
{
  unsigned long ulT=millis();
  if (ulT-ulT0>5000)
  {
    master.start_cycle();
    ulT0=millis();
  }
  
  master.loop();
  master.recv();
}
