#ifndef STATES_HEADER_INCLUDED
#define STATES_HEADER_INCLUDED

#include "timer.h"

class States
{
  protected:
    int state;
    Timer tmt;

    virtual void OnEnterState(int oldState) = 0;
    virtual void OnLeaveState(int newState) = 0;
    virtual void OnRunState(void) = 0;
    virtual void OnTimeout(void) = 0;

  public:
    States(int initialState) {state=initialState;}
  
    void changeState(int newState);
    
    void getState(void) {return state;}

    void loop(void); 
};

#endif
