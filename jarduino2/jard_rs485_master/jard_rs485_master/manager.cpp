#include "manager.h"

#include "api.h"
#include "states.hpp"
#include "wificomm.h"

#include <arduino.h>


class StateGestion:public State
{
   public:
      static StateMachine _machine;

      static State* stIdle;
      static State* stWifiComm;

      StateGestion() :State(&StateGestion::_machine) {}
};

StateMachine StateGestion::_machine;


class StateIdle:public StateGestion
{
    void onRun() override
    {
      if (Comm.isActive()==true)
        _machine.setState(stWifiComm);
    }
};


class StateWifiComm:public StateGestion
{
    void onEnter() override
    {
      api_master(true);
    }

    void onRun() override
    {
      if (Comm.isActive()==false)
        _machine.setState(stIdle);
      else
      {
        unsigned short cmds=Comm.getCommands();

      }
    }

    void onLeave() override
    {
      api_master(false);
    }
};


State * StateGestion::stIdle = new StateIdle();
State * StateGestion::stWifiComm = new StateWifiComm();

void manager_init(void)
{
}

void manager_run(void)
{
  StateGestion::_machine.run();
}


