#include "states.h"

void States::changeState(int newState)
{
  if (state!=newState)
  {
    OnLeaveState(newState);
    int oldState=state;
    state=newState;
    tmt.stop();
    OnEnterState(oldState);
  }
}

void States::loop(void)
{
  OnRunState();
  
  if (tmt.tick()==true)
    OnTimeout();
}
