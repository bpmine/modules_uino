#include "app.h"

#include "slaves.hpp"
#include "master.h"
#include "pins.h"
#include "timer.h"

#define NUM_LEDS    (8)

#include <Wire.h>
#include <RTClib.h>
#include <FastLED.h>

//#define DATE_INIT

bool g_trace_on=false;
E_APP_STATE g_app_state=BOOT;


static DS1307 _rtc;
static Pump _pump('A');
static unsigned long ulT0;
static CRGB _leds[NUM_LEDS];  ///< Tableau des LEDs
static bool _flg500ms=false;

static Oya _oyaB('B');
static Oya _oyaC('C');
static Oya _oyaD('D');
static Oya _oyaE('E');

static Oya * _list_oyas[]=
{
  &_oyaB,
  &_oyaC,
  &_oyaD,
  &_oyaE
};

static OyasList _oyasList(_list_oyas,4);    
static Master _master;

static Timer tmrState;
static Timer tmrBlink(500,false);

void _app_setAllLeds(CRGB col)
{
  for (int i=0;i<NUM_LEDS;i++)
  {
     _leds[i] = col;
  }
}

void _app_clrAllLeds(void)
{
  _app_setAllLeds(CRGB(0,0,0));
}

void _bus_to_objects(void)
{
  int pos=0;
  Oya *pOya=_oyasList.itNext(pos);
  while (pOya!=NULL)
  {
    RqOya *pRqOya = _master.getRequestFrom(pOya->addr);
    if (pRqOya!=NULL)
    {
      pOya->setCommOk(pRqOya->getCommOk());
      pOya->setLow(pRqOya->getLow());
      pOya->setHigh(pRqOya->getHigh());
      pOya->setOn(pRqOya->getOn());
      pOya->setEnabled(pRqOya->getEnabled());
      pOya->setTemp_dg(pRqOya->getTemp());
      pOya->setHum_pc(pRqOya->getHum());
    }
    else
    {
      pOya->setCommOk(false);
    }  

    pOya=_oyasList.itNext(pos);
  }  
    
  RqPump *pPump = _master.getRequestFrom(_pump.addr);
  if (pPump!=NULL)
  {
    _pump.setCommOk(pPump->getCommOk());
    _pump.setFlow(pPump->getFlow());
    _pump.setOn(pPump->getOn());
    _pump.setTemp_dg(pPump->getTemp());
    _pump.setHum_pc(pPump->getHum());
    _pump.setEnabled(pPump->getEnabled());
  }
  else
  {
    _pump.setCommOk(false);
  }
}

void _objects_to_bus(void)
{
  int pos=0;
  Oya *pOya=_oyasList.itNext(pos);
  while (pOya!=NULL)
  {
    RqOya *pRqOya = _master.getRequestFrom(pOya->addr);
    if (pRqOya!=NULL)
    {   
      pRqOya->cmd = (pOya->cmd==true)?1:0;
    }

    pOya = _oyasList.itNext(pos);
  }  
    
  RqPump *pPump = (RqPump *)_master.getRequestFrom(_pump.addr);
  if (pPump!=NULL)
  {
    pPump->cmd=_pump.cmd==true?1:0;
  }
}

void app_term_master(bool on)
{
  _master.setEnable(on);
}

Oya * app_term_get_next_oya(int &pos)
{
  return _oyasList.itNext(pos);  
}

Pump * app_term_get_pump(void)
{
  return &_pump;
}


void app_term_log(bool on)
{
}

void app_term_trace(bool on)
{
  g_trace_on=on;
  _master.setTrace(on);
}

bool app_slave(char addr,bool on)
{
  return false;
}

void app_set_date(int year,int month,int day)
{
  DateTime now = _rtc.now();
  DateTime newDte(year,month,day,now.hour(),now.minute(),now.second());
  _rtc.adjust(newDte);  
}

void app_set_time(int hour,int minute,int second)
{
  DateTime now = _rtc.now();
  DateTime newDte(now.year(),now.month(),now.day(),hour,minute,second);
  _rtc.adjust(newDte);    
}



void app_init(void)
{  
  _app_clrAllLeds();
  FastLED.show();
    
  FastLED.addLeds<NEOPIXEL, PIN_DATA_LEDS>(_leds, NUM_LEDS); 
  FastLED.setBrightness(255);
  
  Wire.begin();
  _rtc.begin();
  
  _master.begin(&Serial1,PIN_TX_EN);
  ulT0=millis();

  #ifdef DATE_INIT
    //app_set_date(2023,7,1);
    //app_set_time(11,9,0);    
    Serial.println("/!\\ SET INIT TIME !");
    _rtc.adjust(DateTime(__DATE__, __TIME__));
  #endif

  DateTime now = _rtc.now();
  char strDate[20];
  sprintf(strDate, "%02d/%02d/%04d %02d:%02d:%02d",now.day() , now.month(), now.year(), now.hour(), now.minute(), now.second());
  Serial.println(strDate);  

  _flg500ms=false;
  tmrBlink.start();  
  
  g_app_state=BOOT;
}

void _app_master_mgt(void)
{
  unsigned long ulT=millis();
  if (ulT-ulT0>1000)
  {
    _master.start_cycle();
    ulT0=millis();
  }

  if (_master.loop()==true)
    _bus_to_objects();
      
  _objects_to_bus();
      
  _master.recv();  
}

void _app_update_leds(void)
{
    int pos=0;
    Oya *pOya=_oyasList.itNext(pos);
    while (pOya!=NULL)
    {
      if (pos-1<NUM_LEDS)
      {
        CRGB col;
        if (pOya->comm_ok==false)
          col=CRGB(0,0,0);
        else if (pOya->high==false) 
          col=CRGB(0,255,0);
        else if (pOya->low==false)
          col=CRGB(0,0,255);
        else 
          col=CRGB(255,0,0);

        if ( (pOya->on==true) && (_flg500ms==true) )
          col=CRGB(0,0,0);
          
        _leds[pos]=col;
      }
      
      pOya->comm_ok=false;
      pOya=_oyasList.itNext(pos);
    }  
}

void app_loop(void)
{
  if (_master.isRunning()==false)
  {
    int pos=0;
    Oya *pOya=_oyasList.itNext(pos);
    while (pOya!=NULL)
    {
      pOya->comm_ok=false;
      pOya=_oyasList.itNext(pos);
    }
  }
  else
  {
    _app_master_mgt();
  }

  switch (g_app_state)
  {
    case BOOT:
    {
      if (digitalRead(PIN_TEST_BTN)==0)
      {        
        g_app_state=TEST;
        digitalWrite(PIN_PWR_ON,HIGH);
        _master.setEnable(true);
        Serial.println("BOUTON TEST");
        tmrState.setDuration_ms(5000);
        tmrState.start();
      }        
      else
      {
        Serial.println("Check RTC...");
        g_app_state=RTC;
      }
        
      break;
    }    

    case TEST:
    {      
      digitalWrite(LED_BUILTIN,HIGH);
      _app_update_leds();
      _leds[0]=CRGB(255,0,0);
      _leds[1]=CRGB(0,255,0);
      _leds[2]=CRGB(0,0,255);

      if (_flg500ms==true)
        _leds[3]=CRGB(0,0,255);
      else
        _leds[3]=CRGB(0,0,0);
      
      if (tmrState.tick()==true)
      {
        Serial.println("Sleep...");
        digitalWrite(LED_BUILTIN,LOW);
        g_app_state=SLEEP;
      }
      break;
    }
    
    case RTC:
    {
      DateTime now = _rtc.now();
      
      if ( (now.hour()==14) && (now.minute()>=0) && (now.minute()<=10) )      
      {
        Serial.println("Todo ?");
        tmrState.setDuration_ms(10000);
        g_app_state=TODO;
        digitalWrite(PIN_PWR_ON,HIGH);
        _master.setEnable(true);      
      }
      else
        g_app_state=SLEEP;
        
      break;      
    }

    case TODO:
    {
      _app_update_leds();
      
      if (tmrState.tick()==true)
      {
        Serial.println("Tmt todo...");
        g_app_state=RTC;
      }
      
      break;
    }    

    case SLEEP:
    default:
    {
      _app_clrAllLeds();
      _master.setEnable(false);
      digitalWrite(PIN_PWR_ON,LOW);
      break;
    }
  }

  FastLED.show();
    
  if (tmrBlink.tick()==true)
  {
    _flg500ms=!_flg500ms;
  }
}
