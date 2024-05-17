#include "app.h"
#include "masterarduino.hpp"

#include <RTClib.h>

 MasterArduino Master;

static DS1307 _rtc;

void app_term_trace(bool en)
{
  Master.setTrace(en);
}

void app_term_master(bool on)
{
  Master.setEnable(on);
}

Pump *app_term_get_pump(void)
{
  SlavesList l=Master.getSlavesList();
  return l.getPump();
}

Oya *app_term_find_first_oya(int &pos)
{
  SlavesList l=Master.getSlavesList();
  Oya *p=l.findFirstOya(pos);
  return p;
}

Oya *app_term_find_next_oya(int &pos)
{
  SlavesList l=Master.getSlavesList();
  Oya *p=l.findNextOya(pos);
  return p;
}

void app_set_pompe(bool on)
{
  Master.set_pump(on);
}

void app_set_oya(char addr,bool on)
{
  Master.set_oya(addr, on);
}

void app_get_date_hour(int &day,int &month,int &year,int &hour,int &minute,int &second,int &dow)
{
  DateTime now = _rtc.now();

  day=now.day();
  month=now.month();
  year=now.year();
  hour=now.hour();
  minute=now.minute();
  second=now.second();
  dow=now.dayOfWeek();
}

void app_get_date_hour(int &day,int &month,int &year,int &hour,int &minute,int &second)
{
  int dow;
  return app_get_date_hour(day, month, year, hour, minute, second,dow);
}

void app_set_date(int day,int month,int year)
{
  DateTime now = _rtc.now();
  _rtc.adjust(DateTime(year,month,day,now.hour(),now.minute(),now.second()));
}

void app_set_hour(int hour,int minute,int second)
{
  DateTime now = _rtc.now();
  _rtc.adjust(DateTime(now.year(),now.month(),now.day(),hour,minute,second));
}

void app_set_slaves_config(unsigned short config)
{
  Master.set_config_slaves(config);
}

unsigned short app_get_slaves_config(void)
{
  return Master.get_config_slaves();
}

