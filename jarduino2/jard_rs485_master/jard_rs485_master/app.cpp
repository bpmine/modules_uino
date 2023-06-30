#include "app.h"

#include "slaves.hpp"
#include "master.h"

bool g_trace_on=false;

unsigned long ulT0;

static Pump _pump('A');

Oya _oyaB('B');
Oya _oyaC('C');
Oya _oyaD('D');
Oya _oyaE('E');

static Oya * _list_oyas[]=
{
  &_oyaB,
  &_oyaC,
  &_oyaD,
  &_oyaE
};

OyasList _oyasList(_list_oyas,4);
    
Master _master;

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


void app_init(void)
{
  _master.begin(&Serial1,PIN_TX_EN);
  ulT0=millis();

  Serial.println(_list_oyas[0]->addr);
  Serial.println(_list_oyas[1]->addr);
}

void app_loop(void)
{
  unsigned long ulT=millis();
  if (ulT-ulT0>1000)
  {
    _master.start_cycle();
    ulT0=millis();
  }

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
    if (_master.loop()==true)
      _bus_to_objects();
      
    _objects_to_bus();
  }
  
  _master.recv();
}
