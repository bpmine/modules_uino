#include "app.h"

#include "slaves.hpp"
#include "master.h"
#include "pins.h"
#include "timer.h"
#include "states.h"

#define NUM_LEDS    (8)

#include <Wire.h>
#include <RTClib.h>
#include <FastLED.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>


//#define DATE_INIT

bool g_trace_on=false;


static DS1307 _rtc;
static Pump _pump('A');
static unsigned long ulT0;
static CRGB _leds[NUM_LEDS];  ///< Tableau des LEDs
static bool _flg500ms=false;

static Oya _oyaB('B');
static Oya _oyaC('C');
static Oya _oyaD('D');
static Oya _oyaE('E');
static Oya _oyaF('F');

static Oya * _list_oyas[]=
{
  &_oyaB,
  &_oyaC,
  &_oyaD,
  &_oyaE,
  &_oyaF
};

static OyasList _oyasList(_list_oyas,5);    
static Master _master;

static Timer tmrBlink(500,false);
static Timer tmrLongTest(2000);

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
      pOya->updCycleStats();
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
    _pump.updCycleStats();
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
  FastLED.setBrightness(80);
  
  Wire.begin();
  _rtc.begin();
  
  _master.begin(&Serial1,PIN_TX_EN);
  ulT0=millis();

  #ifdef DATE_INIT
    //app_set_date(2023,7,1);
    //app_set_time(11,9,0);    
    Serial.println("/!\\ SET INIT TIME !");
    _rtc.adjust(DateTime(__DATE__, __TIME__));
    delay(5000);
  #endif

  DateTime now = _rtc.now();
  char strDate[20];
  sprintf(strDate, "%02d/%02d/%04d %02d:%02d:%02d",now.day() , now.month(), now.year(), now.hour(), now.minute(), now.second());
  Serial.println(strDate);  

  _flg500ms=false;
  tmrBlink.start();  
  
  //g_app_state=BOOT;
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
  if (_master.isRunning()==false)
  {
    _app_clrAllLeds();
    _leds[0]=CRGB(255,0,0);
  }
  else
  {
    _leds[0]=CRGB(0,255,0);
    
    if (_pump.comm_ok==false)
      _leds[1]=CRGB(0,0,0);
    else if (_pump.on==false)
      _leds[1]=CRGB(255,0,0);
    else if (_pump.flow==0)
      _leds[1]=_flg500ms?CRGB(0,0,0):CRGB(255,0,0);
    else if ( (_pump.flow>0) && (_pump.flow<10) )
      _leds[1]=_flg500ms?CRGB(0,0,0):CRGB(0,0,255);
    else
      _leds[1]=_flg500ms?CRGB(0,0,0):CRGB(0,255,0);

    int i=2;
    int pos=0;
    Oya *pOya=_oyasList.itNext(pos);
    while (pOya!=NULL)
    {
      if (i<NUM_LEDS)
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
          
        _leds[i]=col;
        i++;
      }
      
      pOya=_oyasList.itNext(pos);
    }    
  }  
}

void sleepDeep10s()
{
    wdt_enable(WDTO_8S);
    WDTCSR |= (1<<WDCE) | (1<<WDE);
    WDTCSR |= _BV(WDIE);
  
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable();
    
    sleep_mode();
    
    sleep_disable();
    power_all_enable();  
    wdt_disable();    
    delay(2000);
}


typedef enum { BOOT, TEST, RTC, TODO, START_EV, FILLING, STOP_PMP, NEXT, SLEEP  } E_APP_STATE;

class AppStates : public States
{ 
  private:
    int posOya;
    Oya *pOya;
    unsigned long cycle0;
    
  protected:
    void OnEnterState(int oldState) override
    { 
      switch (state)
      {
        case TEST:
        {
          Serial.println("BOUTON TEST");
          
          digitalWrite(PIN_PWR_ON,HIGH);
          digitalWrite(PIN_PWR_LEDS,HIGH);
          _master.setEnable(true);          
          tmt.setDuration_ms(40000);
          tmt.start();
          tmrLongTest.start();
          break;
        }
        case TODO:
        {
            Serial.println("Todo ?");            
            digitalWrite(PIN_PWR_ON,HIGH);
            digitalWrite(PIN_PWR_LEDS,HIGH);
            _master.setEnable(true);          
            break;
        }
        case START_EV:
        {         
          if (pOya!=NULL)
          {
            Serial.print("@");
            Serial.print(pOya->addr);
            Serial.println(": Start EV.");
            pOya->cmd=true;            
          }
          
          cycle0=_master.cycles();
          tmt.setDuration_ms(1000);
          tmt.start();
          break;
        }        
        case FILLING:
        {
          Serial.println("Filling...");
          break;
        }
        case NEXT:
        {
          //Serial.println("Next.");
          break;          
        }
        case STOP_PMP:
        {
          Serial.println("Stop PMP.");
          cycle0=_master.cycles();
          tmt.setDuration_ms(1000);
          tmt.start();
          break;
        }
      }
    }
    
    void OnLeaveState(int newState) override
    { 
    }
    
    virtual void OnRunState(void) override
    {    
      switch (state)
      {
        case BOOT:
        {
          if (digitalRead(PIN_TEST_BTN)==LOW)
            changeState(TEST);   
          else
            changeState(RTC);
            
          break;
        }    
    
        case TEST:
        {      
          if (digitalRead(PIN_TEST_BTN)==HIGH)
            tmrLongTest.start();
    
          /// Appui long sur test
          //if (tmrLongTest.tick()==true)
          //  g_app_state=TODO;
          
          digitalWrite(LED_BUILTIN,HIGH);
          _app_update_leds();
          
          break;
        }
        
        case RTC:
        {
          Serial.println("Check RTC...");
          
          DateTime now = _rtc.now();
          
          if ( (now.hour()==23) && (now.minute()>=0) && (now.minute()<=34) )      
            changeState(TODO);
          else
            changeState(SLEEP);
            
          break;
        }
    
        case TODO:
        {
          _app_update_leds();
    
          pOya=_oyasList.getOya(posOya);
          if (pOya!=NULL)
          {
            if ( (pOya->comm_ok==true) && ((pOya->high==true) || (pOya->low==true)) )
            {
              changeState(START_EV);
              break;
            }              
          }
          changeState(NEXT);
                    
          break;
        }    
    
        case START_EV:
        {
          _app_update_leds();
    
          if (pOya!=NULL)
          {
            pOya->cmd=true;
            if (_master.cycles()!=cycle0)
              changeState(FILLING);
          }
          else
            changeState(NEXT);
    
          break;
        }
        
        case FILLING:
        {
          _app_update_leds();
          
          if (pOya!=NULL)
          {
            if ( (pOya->comm_ok==true) && ((pOya->high==true) || (pOya->low==true)) )
            {
              pOya->cmd=true;
              if (pOya->on==true)
                _pump.cmd=true;
              else
                _pump.cmd=false;
            }
            else
              changeState(STOP_PMP);
          }
          else
            changeState(NEXT);
            
          break;
        }

        case STOP_PMP:
        {
          _pump.cmd=false;
          if (_master.cycles()!=cycle0)
            changeState(NEXT);
            
          break;
        }

        case NEXT:
        {
          _pump.cmd=false;
          int pos=0;
          Oya *p=_oyasList.itNext(pos);
          while (p!=NULL)
          {
            p->cmd=false;
            p=_oyasList.itNext(pos);
          }

          pOya=_oyasList.itNext(posOya);
          if (pOya==NULL)
            posOya=0;

          changeState(RTC);
            
          break;          
        }
    
        case SLEEP:
        default:
        {
          //Serial.println("Sleep...");
          _app_clrAllLeds();
          FastLED.show();
          //_master.setEnable(false);
          //digitalWrite(LED_BUILTIN,LOW);          
          //digitalWrite(PIN_PWR_ON,LOW);
          //digitalWrite(PIN_PWR_LEDS,LOW);
          //sleepDeep10s();
          break;
        }
      }   
    }
    
    virtual void OnTimeout(void) override
    {  
      switch (state)
      {
        case TEST:
        {
            //changeState(SLEEP);
            break;
        }
        case START_EV:
        case STOP_PMP:
        {
          changeState(NEXT);
          break;
        }
      }
    }
    
  
  public:
    AppStates():States(BOOT)
    {
      posOya=0;
      cycle0=0;
    }
};


AppStates _states;

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

  _states.loop();

  FastLED.show();
    
  if (tmrBlink.tick()==true)
  {
    _flg500ms=!_flg500ms;
  }
}
