#include "Sched.h"


void Sched::init()
{
  m_ucStartHour=0xFF;
  m_ucStartMin=0xFF;
  m_ucDuration=0xFF;
  m_ucDaysOfWeek=0xFF;  
}

Sched::Sched()
{
  init();
}

void Sched::setSettings(unsigned char ucStartHour,unsigned char ucStartMin,unsigned char ucDuration,unsigned char ucDaysOfWeek)
{
  if ( ucStartHour>23 )
  {
    init();
    return;
  }
  
  if ( ucStartMin>59 )
  {
    init();
    return;
  }

  unsigned long minsSinceMidnightStart=ucStartHour*60+ucStartMin;
  unsigned long minsSinceMidnightEnd=minsSinceMidnightStart+ucDuration;
  if (minsSinceMidnightEnd>=60*24)
  {
    ucDuration=minsSinceMidnightEnd-60*24;
  }  
  
  m_ucStartHour=ucStartHour;
  m_ucStartMin=ucStartMin;
  m_ucDuration=ucDuration;
  m_ucDaysOfWeek=ucDaysOfWeek;
}

void Sched::getSettings(unsigned char *o_ucStartHour,unsigned char *o_ucStartMin,unsigned char *o_ucDuration,unsigned char *o_ucDaysOfWeek)
{
  *o_ucStartHour=m_ucStartHour;
  *o_ucStartMin=m_ucStartMin;
  *o_ucDuration=m_ucDuration;
  *o_ucDaysOfWeek=m_ucDaysOfWeek;
}

bool Sched::check(int hour,int minute,int doW)
{
  if ( (doW>6) || (doW<0) )
    return;

  if ( (hour<0) || (hour>23) )
    return;
  
  if ( (minute<0) || (minute>59) )
    return;

  bool isDay=true;
  if ( m_ucDaysOfWeek!=0xFF )
  {
    if (m_ucDaysOfWeek&(1<<doW)!=(1<<doW))
      isDay=false;
  }

  unsigned long minsSinceMidnightStart=m_ucStartHour*60+m_ucStartMin;
  unsigned long minsSinceMidnightEnd=minsSinceMidnightStart+m_ucDuration;
  unsigned long minsSinceMidnight=hour*60+minute;

  if ( (isDay) && (minsSinceMidnight>=minsSinceMidnightStart) && (minsSinceMidnight<=minsSinceMidnightEnd) )
    return true;
  else
    return false;  
}
  
