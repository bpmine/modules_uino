#include "jard.h"

#include <Wire.h>
#include <RTClib.h>

#include "pins.h"
#include "memoire.h"
#include "autom.h"
#include "pump.h"

DS1307 rtc;
Pump pump1;
Pump pump2;

Jard::Jard():tmrBlink(TEMPS_BLINK_MS,false)
{  
  m_flgBlk=true;
  m_flgBattOk=false;
  m_flgSunOk=false;
}

void Jard::init()
{ 
  Wire.begin();
  rtc.begin();

  load();

  enablePompe(1,true);
  enablePompe(2,true);

  tmrBlink.start();
}

void Jard::save(void)
{
  T_MEMOIRE mem; 
  
  memoire_load(&mem);  

  mem.bPump1Enable=pump1.isEnabled()?1:0;
  mem.bPump1Auto=pump1.isAuto()?1:0;

  mem.bPump2Enable=pump2.isEnabled()?1:0;
  mem.bPump2Auto=pump2.isAuto()?1:0;

  memoire_save(&mem);
}

void Jard::load(void)
{
  T_MEMOIRE mem; 
  memoire_load(&mem);

  pump1.setEnable(mem.bPump1Enable==1?true:false);
  pump2.setEnable(mem.bPump2Enable==1?true:false);

  pump1.setAuto(mem.bPump1Auto==1?true:false);
  pump2.setAuto(mem.bPump2Auto==1?true:false);  
}


void Jard::setDate(int day,int month,int year)
{
  DateTime now = rtc.now();
  DateTime newDte(year,month,day,now.hour(),now.minute(),now.second());
  rtc.adjust(newDte);  
}

void Jard::setHour(int hour,int minute)
{
  DateTime now = rtc.now();
  DateTime newDte(now.year(),now.month(),now.day(),hour,minute,0);
  rtc.adjust(newDte);  
}

void Jard::getDateStr(char *o_strDate,int i_maxSize)
{
  if (i_maxSize<20)
    return;
    
  char tmp[35];
  DateTime now = rtc.now();
  sprintf(o_strDate, "%02d/%02d/%04d %02d:%02d:%02d",now.day() , now.month(), now.year(), now.hour(), now.minute(), now.second());  
}

void Jard::forcePompe(int i_iNumPompe,bool i_flgON)
{
  if (i_iNumPompe==1)
  {
    if (i_flgON==true)
      mbs.set(MB_PMP1_FORCED);
    else
      mbs.reset(MB_PMP1_FORCED);    
  }
  else if (i_iNumPompe==2)
  {
    if (i_flgON==true)
      mbs.set(MB_PMP2_FORCED);
    else
      mbs.reset(MB_PMP2_FORCED);
  }   
}

void Jard::enablePompe(int i_iNumPompe,bool i_flgEnabled)
{
  if (i_iNumPompe==1)
  {
    if (i_flgEnabled==true)
      mbs.set(MB_PMP1_ENABLE);
    else
      mbs.reset(MB_PMP1_ENABLE);    
  }
  else if (i_iNumPompe==2)
  {
    if (i_flgEnabled==true)
      mbs.set(MB_PMP2_ENABLE);
    else
      mbs.reset(MB_PMP2_ENABLE);    
  }
}

void Jard::setAuto(int i_iNumPompe,bool i_flgAuto)
{
  if (i_iNumPompe==1)
  {
    if (i_flgAuto==true)
      mbs.set(MB_PMP1_AUTO);
    else
      mbs.reset(MB_PMP1_AUTO);    
  }
  else if (i_iNumPompe==2)
  {
    if (i_flgAuto==true)
      mbs.set(MB_PMP2_AUTO);
    else
      mbs.reset(MB_PMP2_AUTO);    
  }
}

void Jard::setBatLevel(int i_level_dxv)
{
  m_batt_level_dxv=i_level_dxv;
}

void Jard::setSunLevel(int i_level_dxv)
{
  m_sun_level_dxv=i_level_dxv; 
}

void Jard::setTemp(int i_temp_dg)
{
  m_temp_deg=i_temp_dg; 
}

void Jard::setHum(int i_hum_dg)
{
  m_hum_pc=i_hum_dg;
}

unsigned short Jard::getBattLevel(void)
{
  return m_batt_level_dxv;  
}

unsigned short Jard::getSunLevel(void)
{
  return m_sun_level_dxv;
}

int Jard::getTemp(void)
{
  return m_temp_deg;
}

unsigned char Jard::getHum(void)
{
  return m_hum_pc; 
}

void Jard::loop(void)
{
  bool outLedPmp1=false;
  bool outLedPmp2=false;
  bool outCmdPmp1=false;
  bool outCmdPmp2=false;
  bool outCpu=false;
  bool outSun=false;
  bool outBatt=false;
  
  if (tmrBlink.tick()==true)
    m_flgBlk=!m_flgBlk;
  
  if (m_batt_level_dxv>=SEUIL_LOW_BATT_H)
    m_flgBattOk=true;
  else if (m_batt_level_dxv<=SEUIL_LOW_BATT_L)
    m_flgBattOk=false;
    
  if (m_sun_level_dxv>=SEUIL_LOW_SUN_H)
    m_flgSunOk=true;
  else if (m_sun_level_dxv<=SEUIL_LOW_SUN_L)
    m_flgSunOk=false;

  bool on=!mbs_inputs.get(IB_BTN_ON);
  if (on==true)
  {
    if (mbs_inputs.getFalling(IB_BTN_PMP1))
      pump1.startTimer(TEMPS_TIMER_MS);
    
    if (mbs_inputs.getFalling(IB_BTN_PMP2))
      pump2.startTimer(TEMPS_TIMER_MS);
  
    pump1.setForced(mbs.get(MB_PMP1_FORCED));
    pump1.setEnable(mbs.get(MB_PMP1_ENABLE));
    pump1.setAuto(mbs.get(MB_PMP1_AUTO));
    pump1.setBattOK(m_flgBattOk);
    
    pump2.setForced(mbs.get(MB_PMP2_FORCED));
    pump2.setEnable(mbs.get(MB_PMP2_ENABLE));
    pump2.setAuto(mbs.get(MB_PMP2_AUTO));
    pump2.setBattOK(m_flgBattOk);
    
    pump1.loop();
    pump2.loop();

    outBatt=m_flgBattOk?true:m_flgBlk;
    outSun=m_flgSunOk;
    
    outLedPmp1=pump1.getError()?m_flgBlk:pump1.getOut();
    outCmdPmp1=pump1.getOut();
    
    outLedPmp2=pump2.getError()?m_flgBlk:pump2.getOut();
    outCmdPmp2=pump2.getOut();
  }
  else
  {
    pump1.stopTimer();
    pump1.setEnable(false);
    pump1.setAuto(false);
    pump1.setBattOK(false);
    
    pump2.stopTimer();
    pump2.setEnable(false);
    pump2.setAuto(false);
    pump2.setBattOK(false);
  }
  
    
  mbs_outputs.start_latch();
  mbs_outputs.fromBool(OB_LED_CPU,on);
  mbs_outputs.fromBool(OB_LED_BATT,outBatt);
  mbs_outputs.fromBool(OB_LED_SUN,outSun);  
  mbs_outputs.fromBool(OB_CMD_PMP1,outCmdPmp1);
  mbs_outputs.fromBool(OB_LED_PMP1,outLedPmp1);
  
  mbs_outputs.fromBool(OB_CMD_PMP2,outCmdPmp2);
  mbs_outputs.fromBool(OB_LED_PMP2,outLedPmp2);
  
  mbs_outputs.end_latch();
;}
