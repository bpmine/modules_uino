#include "manager.h"

#include "api.h"
#include "states.hpp"
#include "wificomm.h"
#include "btn.hpp"
#include "pins.h"
#include "slaves.hpp"

#include <arduino.h>

#define ONE_MINUTE    (1 * 60 * 1000UL)
#define ONE_SECOND    (1000UL)

#define TIMEOUT_START_EV_MS                 (ONE_SECOND)
#define TIMEOUT_LOW_MS                      (ONE_MINUTE)
#define TIMEOUT_FILLING_AFTER_LOW_MS        (ONE_MINUTE)
#define TIMEOUT_STOP_EV_AFTER_STOP_PUMP_MS  (ONE_SECOND)
#define TIMEOUT_STAY_IN_DISPLAY_MS          (ONE_MINUTE)
#define TIMEOUT_WAIT_STEADY_BEFORE_START    (4 * 1000UL)

#define MAX_TIME_PER_OYA_S                  (2*60)

Btn _btn;
int mode_aff=MODE_AFF_IDLE;
Timer tmr1S(1000,false);
unsigned long _seconds=0;

class StateGestion:public State
{
   public:
      static StateMachine _machine;

      static State* stIdle;
      static State* stWifiComm;
      static State* stDisplay;
      static State* stStartFill;
      static State* stStartEV;
      static State* stStartPump;
      static State* stFillLow;
      static State* stFillWait;
      static State* stChangeEV;
      static State* stStopPump;
      static State* stStopEV;

      StateGestion() :State(&StateGestion::_machine) {}
};

StateMachine StateGestion::_machine;


class StateIdle:public StateGestion
{
    void onRun() override
    {
      mode_aff=MODE_AFF_IDLE;

      if (Comm.isActive()==true)
        _machine.setState(stWifiComm);
      else if (_btn.isRising())
        _machine.setState(stDisplay);
    }
};


class StateWifiComm:public StateGestion
{
    void onEnter() override
    {
      Serial.println("Enter wifi");
      api_master(true);
    }

    void onRun() override
    {
      mode_aff=MODE_AFF_REMOTE;

      if (Comm.isActive()==false)
        _machine.setState(stIdle);
      else
      {
        unsigned short cmds=Comm.getCommands();
        api_set_commands(cmds);
      }
    }

    void onLeave() override
    {
      Serial.println("Leave");
      api_master(false);
    }
};

class StateDisplay:public StateGestion
{
    void onEnter() override
    {
      Serial.println("Enter display");
      api_master(true);
      _machine.startTimeOut(TIMEOUT_STAY_IN_DISPLAY_MS);
    }
    void onRun() override
    {
      mode_aff=MODE_AFF_DISPLAY;

      if (_btn.isRising())
      {
        api_master(false);
        _machine.setState(stIdle);
      }
      else if (_btn.isLongPressed())
        _machine.setState(stStartFill);
    }
    void onLeave() override
    {
      Serial.println("Leave display");
    }
    void onTimeout() override
    {
      api_master(false);
      _machine.setState(stIdle);
    }
};

#define MAX_OYAS  (15)
int _ad_oyas_to_fill[MAX_OYAS];
int _time_oyas_to_fill[MAX_OYAS];
int _nb_to_fill=0;
int _inx_to_fill=0;
bool _up_1s=false;

void _init_to_fill(void)
{
  for (int i=0;i<MAX_OYAS;i++)
  {
    _time_oyas_to_fill[i]=0;
    _ad_oyas_to_fill[i]=0;
    _nb_to_fill=0;
    _inx_to_fill=0;
  }
  _up_1s=false;
}

int _get_next_addr_to_fill(void)
{
  int oldInx=_inx_to_fill;
  int nxt=oldInx+1;

  while (1)
  {
    if (nxt>_nb_to_fill)
      nxt=0;

    if (nxt==oldInx)
    {
      return -1;
    }

    Oya *o=api_get_oya(_ad_oyas_to_fill[nxt]);
    if (o!=nullptr)
    {
      if ((o->comm_ok==true) && (o->high==false) && (_time_oyas_to_fill[nxt]<MAX_TIME_PER_OYA_S))
      {
        return nxt;
      }
    }

    nxt++;
  }
}

void _add_to_fill(int addr)
{
  bool found=false;
  for (int i=0;i<MAX_OYAS;i++)
  {
    if (_ad_oyas_to_fill[i]==addr)
    {
      found=true;
      break;
    }
  }

  if (found==false)
  {
    _ad_oyas_to_fill[_nb_to_fill]=addr;
    _time_oyas_to_fill[_nb_to_fill]=0;
    _nb_to_fill++;
  }
}

void _swap(int i,int j)
{
  int tmp=_ad_oyas_to_fill[i];
  _ad_oyas_to_fill[i]=_ad_oyas_to_fill[j];
  _ad_oyas_to_fill[j]=tmp;
}

void _reorder_to_fill(void)
{
  for (int i=0;i<_nb_to_fill;i++)
  {
    for (int j=0;j<_nb_to_fill;j++)
    {
      Oya *a=api_get_oya(_ad_oyas_to_fill[i]);
      Oya *b=api_get_oya(_ad_oyas_to_fill[j]);

      if ( (a!=nullptr) && (b!=nullptr) )
      {
        if ( (b->low==true) && (a->low==false) )
          _swap(i,j);
        else if ( (b->high==true) && (a->high==false) )
          _swap(i,j);
      }
    }
  }
}

class StateStartFill:public StateGestion
{
    void onEnter() override
    {
      Serial.println("Enter Start fill");
      _machine.startTimeOut(TIMEOUT_WAIT_STEADY_BEFORE_START);
      _init_to_fill();
    }

    void onRun() override
    {
      mode_aff=MODE_AFF_AUTO;

      if (_btn.isRising())
        _machine.setState(stDisplay);
    }

    void onTimeout() override
    {
      int pos;
      Oya *oya=api_find_first_oya(pos);
      while (oya!=nullptr)
      {
        _add_to_fill(oya->addr);
        oya=api_find_next_oya(pos);
      }

      _reorder_to_fill();
      _inx_to_fill=0;

      Serial.print("Order: ");
      for (int i=0;i<_nb_to_fill;i++)
      {
        int addr=_ad_oyas_to_fill[i];
        Serial.print("@");
        Serial.print(addr);
        Serial.print(" ");
      }
      Serial.println();

      if (_nb_to_fill==0)
        _machine.setState(stDisplay);
      else
        _machine.setState(stStartEV);
    }
};

class StateStartEV:public StateGestion
{
    void onEnter() override
    {
      int addr=_ad_oyas_to_fill[_inx_to_fill];
      api_set_oya(addr, true);

      _machine.startTimeOut(TIMEOUT_START_EV_MS);
      Serial.print("Enter Start EV: @");
      Serial.println(addr);
    }
    void onTimeout()
    {
      _machine.setState(stStartPump);
    }
};

class StateStartPump:public StateGestion
{
    void onEnter() override
    {
      api_set_pompe(true);
      _machine.setState(stFillLow);
      Serial.println("Enter Start Pump");
    }
};

class StateFillLow:public StateGestion
{
    void onEnter() override
    {
      _machine.startTimeOut(TIMEOUT_LOW_MS);
      Serial.println("Enter Fill until LOW");
      _up_1s=false;

    }
    void onRun() override
    {
      mode_aff=MODE_AFF_AUTO;

      if (_up_1s==true)
      {
        _time_oyas_to_fill[_inx_to_fill]++;
        _up_1s=false;
      }

      if (_btn.isShortPressed())
        _machine.setState(stStopPump);

      Oya * oya=api_get_oya(_ad_oyas_to_fill[_inx_to_fill]);
      if (oya!=nullptr)
      {
        if (oya->low==true)
          _machine.setState(stFillWait);
        else if (oya->high==true)
          _machine.setState(stChangeEV);
      }

      if (_time_oyas_to_fill[_inx_to_fill]>MAX_TIME_PER_OYA_S)
      {
        _machine.setState(stChangeEV);
        Serial.println("Max time reached");
      }
    }
    void onTimeout()
    {
      Serial.print("TIMEOUT LOW SLAVE @");
      Serial.println(_ad_oyas_to_fill[_inx_to_fill]);
      _machine.setState(stChangeEV);
    }
};

class StateFillWait:public StateGestion
{
    void onEnter() override
    {
      _machine.startTimeOut(TIMEOUT_FILLING_AFTER_LOW_MS);
      Serial.println("Fill TIMED");
    }
    void onRun() override
    {
      mode_aff=MODE_AFF_AUTO;

      if (_up_1s==true)
      {
        _time_oyas_to_fill[_inx_to_fill]++;
        _up_1s=false;
      }

      if (_btn.isShortPressed())
        _machine.setState(stStopPump);

      Oya * oya=api_get_oya(_ad_oyas_to_fill[_inx_to_fill]);
      if (oya!=nullptr)
      {
        if (oya->high==true)
          _machine.setState(stChangeEV);
      }

      if (_time_oyas_to_fill[_inx_to_fill]>MAX_TIME_PER_OYA_S)
      {
        Serial.println("Max time reached");
        _machine.setState(stChangeEV);
      }
    }
    void onTimeout()
    {
      _machine.setState(stChangeEV);
    }
};

class StateChangeEv:public StateGestion
{
    void onEnter() override
    {
      int nxt=_get_next_addr_to_fill();
      if (nxt==-1)
      {
        Serial.println("Enter change EV: Goto stop Pump");
        _machine.setState(stStopPump);
      }
      else
      {
        api_set_oya(_ad_oyas_to_fill[_inx_to_fill], false);
        api_set_oya(_ad_oyas_to_fill[nxt], true);
        Serial.print("Enter change EV: @");
        Serial.print(_ad_oyas_to_fill[_inx_to_fill]);
        Serial.print(" -> ");
        Serial.println(_ad_oyas_to_fill[nxt]);

        _inx_to_fill=nxt;
        _machine.setState(stFillLow);
      }
    }
};

class StateStopPump:public StateGestion
{
    void onEnter() override
    {
      Serial.println("Enter Stop pump");
      api_set_pompe(false);
      _machine.startTimeOut(TIMEOUT_STOP_EV_AFTER_STOP_PUMP_MS);
    }
    void onTimeout()
    {
      _machine.setState(stStopEV);
    }
};

class StateStopEV:public StateGestion
{
    void onEnter() override
    {
      api_set_commands(0);
      Serial.println("Enter Stop EV");
      _machine.setState(stDisplay);
    }
};

State * StateGestion::stIdle = new StateIdle();
State * StateGestion::stWifiComm = new StateWifiComm();
State * StateGestion::stDisplay = new StateDisplay();
State * StateGestion::stStartFill = new StateStartFill();
State * StateGestion::stStartEV = new StateStartEV();
State * StateGestion::stStartPump = new StateStartPump();
State * StateGestion::stFillLow = new StateFillLow();
State * StateGestion::stFillWait = new StateFillWait();
State * StateGestion::stChangeEV = new StateChangeEv();
State * StateGestion::stStopPump = new StateStopPump();
State * StateGestion::stStopEV = new StateStopEV();


void manager_init(void)
{
  StateGestion::_machine.setState(StateGestion::stIdle);
  _btn.begin(PIN_TEST_BTN,INPUT_PULLUP,true);
  tmr1S.start();
}

void manager_run(void)
{
  _btn.loop();
  if (tmr1S.tick()==true)
  {
    _seconds++;
    _up_1s=true;
  }

  StateGestion::_machine.run();
}


