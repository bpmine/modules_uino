/**
 * @file states.cpp
 *
 * @brief Mod�le de gestion d'une machine � �tats
 **/
#include "states.h"

/**
 * @brief Changement d'�tat
 * @param[in] newState Nouvel �tat
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
 * @brief Boucle � appeler p�riodiquement
 **/
void States::loop(void)
{
  OnRunState();
  
  if (tmt.tick()==true)
    OnTimeout();
}
