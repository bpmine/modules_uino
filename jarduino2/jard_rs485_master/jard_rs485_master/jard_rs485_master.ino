#include "master.hpp"

#define PIN_TX_EN         (6)

Master master;

void serialEvent(void)
{
  master.recv();
}

void setup() 
{
  Serial.begin(9600);
  master.begin(&Serial1,PIN_TX_EN);
  Serial.println("Boot");
} 

void loop() 
{
  master.loop();
}
