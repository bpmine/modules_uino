/**
 * @file jard_rs485_master.ino
 *
 * @brief Programme de gestion du systeme des OYAs
 **/
#include "masterarduino.hpp"
#include "pins.h"
#include "timer.h"
#include "wificomm.h"
#include "manager.h"

#include <FastLED.h>

#define COL_BLACK   CRGB(0,0,0)
#define COL_RED     CRGB(255,0,0)
#define COL_BLUE    CRGB(0,0,255)
#define COL_GREEN   CRGB(0,255,0)

Timer tmrCycle(3000,false);
extern MasterArduino Master;

Timer tmrLeds(100,false);
Timer tmrBlink(300,false);
bool flgBlink;

#define NUM_LEDS  (16)
static CRGB _leds[NUM_LEDS];  ///< Tableau des LEDs

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

  FastLED.addLeds<NEOPIXEL, PIN_DATA_LEDS>(_leds, NUM_LEDS);
  for (int j=0;j<NUM_LEDS;j++)
    _leds[j]=CRGB(0,0,0);
  FastLED.show();
 
  Serial.begin(9600);  
  Serial.println("Boot");

  Master.begin(&Serial1, PIN_TX_EN);
  Master.set_config_slaves(0x003F);

  tmrCycle.start();
  tmrLeds.start();
  tmrBlink.start();
  flgBlink=false;

  //Master.setEnable(true);
  //digitalWrite(PIN_PWR_ON,HIGH);

  //Master.set_commands(0x01);
  Comm.begin(&Serial2);
  //Comm.begin(&Serial3);

  manager_init();

  digitalWrite(PIN_PWR_LEDS_INV,LOW);
  digitalWrite(PIN_PWR_LEDS,HIGH);
  FastLED.setBrightness(50);
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

  if (tmrBlink.tick()==true)
  {
    flgBlink=!flgBlink;
  }

  if (tmrLeds.tick()==true)
  {
    _leds[0]=COL_RED;

    Pump *pump=Master.getSlavesList().getPump();
    if ( (pump==nullptr) || (pump->comm_ok==false) )
      _leds[1]=COL_BLACK;
    else if (pump->on==true)
      _leds[1]=flgBlink?COL_BLACK:COL_BLUE;
    else
      _leds[1]=COL_BLUE;

    for (int i=2;i<NUM_LEDS;i++)
      _leds[i]=COL_BLACK;
    int pos;
    Oya *oya=Master.getSlavesList().findFirstOya(pos);
    while (oya!=nullptr)
    {
      if ( (oya->addr>1) && (oya->addr<NUM_LEDS) )
      {
        if (oya->comm_ok==false)
          _leds[(unsigned char)oya->addr]=COL_BLACK;
        else
        {
          CRGB col=COL_RED;
          if ( (oya->high==true) && (oya->low==false) )
            col=COL_BLUE;
          else if ( (oya->high==false) && (oya->low==false) )
            col=COL_GREEN;


          if (oya->on==true)
            _leds[(unsigned char)oya->addr]=flgBlink ? col : COL_BLACK;
          else
            _leds[(unsigned char)oya->addr]=col;
        }
      }

      oya=Master.getSlavesList().findNextOya(pos);
    }

    FastLED.show();
  }

  Comm.loop();

  manager_run();
}
