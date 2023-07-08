/**
 * @file states.h
 *
 * @brief Modèle de gestion d'une machine à états
 **/
#ifndef STATES_HEADER_INCLUDED
#define STATES_HEADER_INCLUDED

#include "timer.h"

/**
 * @class States
 * @brief Classe de gestion d'une machine à états
 **/
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
    States() {state=-1;}
    virtual ~States() {}
  
    void changeState(int newState);
    
    int getState(void) {return state;}

    void loop(void); 
};

#endif
