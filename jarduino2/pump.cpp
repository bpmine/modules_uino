#include "pins.h"
#include "autom.h"
#include "pump.h"
#include "jard.h"

 #include <arduino.h>
 
Pump::Pump()
{
    m_enabled=false;
    m_forced=false;
    m_auto=false;

    m_cmd=false;
    m_out=false;
    m_mins=0;
}

bool Pump::incMins(void)
{
  if (m_out)
    m_mins++;
    
  if (m_mins>=60)
  {
    m_mins=0;
    return true;
  }
  return false;  
}

void Pump::loop(int hour,int min,bool batt_ok,bool cmd_remote)
{
  m_timer.tick();
  
  bool tmr=m_timer.isRunning();

  bool sched=m_sched.check(hour,min,0);
  
  m_cmd=( m_enabled  && ( (tmr) || ( (sched) && (m_auto) ) || ( (m_remote) && (cmd_remote) ) ) );
  m_out=( m_cmd && batt_ok ) || ( m_forced );
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

void Pump::setRemote(bool flgRemote)
{
  m_remote=flgRemote;
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

bool Pump::getError(void)
{
  return (m_cmd!=m_out) && (!m_forced) ?true:false;
}

bool Pump::getOut(void)
{
  return m_out;
}

void Pump::setSched(unsigned char startHour,unsigned char startMin,unsigned char ucDuration_min,unsigned char ucDaysOfWeek)
{
  m_sched.setSettings(startHour,startMin,ucDuration_min,ucDaysOfWeek);
}

void Pump::getSched(unsigned char *o_startHour,unsigned char *o_startMin,unsigned char *o_ucDuration_min,unsigned char *o_ucDaysOfWeek)
{
  m_sched.getSettings(o_startHour,o_startMin,o_ucDuration_min,o_ucDaysOfWeek);
}
