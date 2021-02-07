/**
 * @file stc.cpp
 * 
 * @brief IMPLEMENTATION STC - HORLOGE LOGICIELLE
*/
#include "stc.h"
#include <arduino.h>
    
Stc::Stc()
{
  m_ticks_ms=6*3600*1000L;
  m_hours=0;
  m_mins=0;
  m_secs=0;

  m_t=millis();
}

void Stc::updateHMS()
{
  long tmp=m_ticks_ms/1000;
  m_secs=tmp%60;
  tmp=tmp/60;

  m_mins=tmp%60;
  tmp=tmp/60;

  m_hours=tmp%60;  
}

void Stc::setTime(int h,int m,int s)
{
  m_ticks_ms=(long)h*3600L*1000L;
  m_ticks_ms+=(long)m*60L*1000L;
  m_ticks_ms+=s*1000;
  updateHMS();
}

int Stc::getHours()
{
  return m_hours;
}

int Stc::getMinutes()
{
  return m_mins;
}

int Stc::getSeconds()
{
  return m_secs;
}

void Stc::tick()
{
  long t=millis();

  long delta=0;
  if (t>m_t)
    delta=t-m_t;
  else
    delta=0xFFFFFFFF-m_t+t;

  m_ticks_ms+=delta; 
  char str[10];
  sprintf(str,"%ld",m_ticks_ms); 
  //Serial.println(str);

  updateHMS();
   
  //m_hours=m_mins;
  //m_mins=m_secs;
  
  m_t=t;
}
