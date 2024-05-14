#include "app.h"
#include "masterarduino.hpp"

MasterArduino Master;

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



