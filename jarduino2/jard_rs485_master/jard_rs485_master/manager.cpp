#include "manager.h"

#include "api.h"
#include "states.hpp"
#include "wificomm.h"
#include "btn.hpp"
#include "pins.h"

#include <arduino.h>

Btn _btn;


class StateGestion:public State
{
   public:
      static StateMachine _machine;

      static State* stIdle;
      static State* stWifiComm;
      static State* stDisplay;

      StateGestion() :State(&StateGestion::_machine) {}
};

StateMachine StateGestion::_machine;


class StateIdle:public StateGestion
{
    void onRun() override
    {
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
    }
    void onRun() override
    {
      if (_btn.isRising())
        _machine.setState(stIdle);
    }
    void onLeave() override
    {
      Serial.println("Leave");
      api_master(false);
    }
};


State * StateGestion::stIdle = new StateIdle();
State * StateGestion::stWifiComm = new StateWifiComm();
State * StateGestion::stDisplay = new StateDisplay();

void manager_init(void)
{
  StateGestion::_machine.setState(StateGestion::stIdle);
  _btn.begin(PIN_TEST_BTN,INPUT_PULLUP,true);
}

void manager_run(void)
{
  _btn.loop();
  StateGestion::_machine.run();
}


