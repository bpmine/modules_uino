#include "pins.h"
#include "autom.h"
#include "pump.h"


 #include <arduino.h>
 
Pump::Pump()
{
    m_enabled=false;
    m_forced=false;
    m_auto=false;
    m_battOk=false;

    m_cmd=false;
    m_out=false;
}

void Pump::loop(void)
{
  m_timer.tick();
  
  bool tmr=m_timer.isRunning();
  
  m_cmd=( m_enabled  && ( tmr ) );
  m_out=( m_cmd && m_battOk ) || ( m_forced );

  /*Serial.print(m_enabled);
  Serial.print(" ");
  Serial.print(m_forced);
  Serial.print(" ");
  Serial.print(m_battOk);
  Serial.print(" ");
  Serial.println(m_out);*/  
}

void Pump::startTimer(long delay_ms)
{
  m_timer.setDuration(delay_ms);
  m_timer.start();
}

void Pump::stopTimer(void)
{
  m_timer.stop();
}

void Pump::setEnable(bool flgEnabled)
{
  m_enabled=flgEnabled;
}

void Pump::setForced(bool flgForced)
{
  m_forced=flgForced;
  
}

bool Pump::isEnabled(void)
{
  return m_enabled;
}

bool Pump::isAuto(void)
{
  return m_auto;
}


void Pump::setAuto(bool flgAuto)
{
  m_auto=flgAuto;
}
  
void Pump::setBattOK(bool flgBattOk)
{
  m_battOk=flgBattOk;
}

bool Pump::getError(void)
{
  return m_cmd!=m_out?true:false;
}

bool Pump::getOut(void)
{
  return m_out;
}
