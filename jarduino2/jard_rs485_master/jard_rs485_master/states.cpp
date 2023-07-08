/**
 * @file states.cpp
 *
 * @brief Modèle de gestion d'une machine à états
 **/
#include "states.h"

/**
 * @brief Changement d'état
 * @param[in] newState Nouvel état
 **/
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

/**
 * @brief Boucle à appeler périodiquement
 **/
void States::loop(void)
{
  OnRunState();
  
  if (tmt.tick()==true)
    OnTimeout();
}
