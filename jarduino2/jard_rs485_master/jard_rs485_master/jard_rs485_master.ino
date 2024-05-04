#include "masterarduino.hpp"
#include "pins.h"
#include "timer.h"

Timer tmrCycle(2000,false);

void setup() 
{
  pinMode(PIN_TX_EN,OUTPUT);
  digitalWrite(PIN_TX_EN,LOW);
  pinMode(PIN_TEST_BTN,INPUT_PULLUP);
  
  pinMode(PIN_PWR_ON,OUTPUT);
  digitalWrite(PIN_PWR_ON,LOW);

  pinMode(PIN_PWR_LEDS,OUTPUT);
  digitalWrite(PIN_PWR_LEDS,HIGH);

  pinMode(PIN_PWR_WIFI,OUTPUT);
  digitalWrite(PIN_PWR_WIFI,HIGH);
  
  pinMode(LED_BUILTIN,OUTPUT);
  digitalWrite(LED_BUILTIN,LOW);
  
  pinMode(PIN_DATA_LEDS,OUTPUT);
  digitalWrite(PIN_DATA_LEDS,LOW);
 
  //pinMode(PIN_SD_CS,OUTPUT);
  //digitalWrite(PIN_SD_CS,LOW);
  /*pinMode(11,INPUT);
  pinMode(12,INPUT);
  pinMode(13,INPUT);*/

  Serial.begin(9600);  
  Serial.println("Boot");

  Master.begin(&Serial1, PIN_TX_EN);
  Master.setEnable(true);
  Master.enable_slaves(0x01);

  tmrCycle.start();
  digitalWrite(PIN_PWR_ON,HIGH);
} 

void loop() 
{
  if (tmrCycle.tick()==true)
  {
    Serial.println("Cycle");
    Master.start_cycle();    
  }
  
  if (Master.loop()==true)
  {
    Serial.println("Sync");
  }
}
