#include "masterarduino.hpp"
#include "pins.h"
#include "timer.h"

Timer tmrCycle(2000,false);
extern MasterArduino Master;

void setup() 
{
  pinMode(PIN_TX_EN,OUTPUT);
  digitalWrite(PIN_TX_EN,LOW);
  pinMode(PIN_TEST_BTN,INPUT_PULLUP);
  
  pinMode(PIN_PWR_ON,OUTPUT);
  digitalWrite(PIN_PWR_ON,LOW);

  pinMode(PIN_PWR_LEDS,OUTPUT);
  digitalWrite(PIN_PWR_LEDS,LOW);

  pinMode(PIN_PWR_WIFI,OUTPUT);
  digitalWrite(PIN_PWR_WIFI,LOW);

  pinMode(PIN_PWR_LEDS_INV,OUTPUT);
  digitalWrite(PIN_PWR_LEDS_INV,HIGH);

  pinMode(PIN_PWR_WIFI_INV,OUTPUT);
  digitalWrite(PIN_PWR_WIFI_INV,HIGH);

  pinMode(LED_BUILTIN,OUTPUT);
  digitalWrite(LED_BUILTIN,LOW);
  
  pinMode(PIN_DATA_LEDS,OUTPUT);
  digitalWrite(PIN_DATA_LEDS,LOW);
 
  Serial.begin(9600);  
  Serial.println("Boot");

  Master.begin(&Serial1, PIN_TX_EN);
  Master.config_slaves(0xFFFF);

  tmrCycle.start();

  //Master.setEnable(true);
  //digitalWrite(PIN_PWR_ON,HIGH);

  //Master.set_commands(0x01);
} 

void loop() 
{
  if (tmrCycle.tick()==true)
  {
    Master.start_cycle();    
  }
  
  if (Master.loop()==true)
  {
  }
}
