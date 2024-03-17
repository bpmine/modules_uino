/**
 * @file timer.cpp
 * @brief Implementation - Gestion d'un timer en ms
*/

#include "timer.h"
#include <arduino.h>

/**
 * brief Constructeur
 * @param[in] duration_ms Duree du timer en ms
 * @param[in] single true si le timer doit s'erreter des qu'il est au bout. Sinon, il recommence.
*/
Timer::Timer(unsigned long duration_ms,bool single)
{
  this->tick0_ms=millis();
  this->duration_ms=duration_ms;  
  this->running=false;
  this->single=single;
}

/**
 * @brief Retourne le temps restant avan echeance du timer
 * @return Temps restant en ms
*/
unsigned long Timer::getRemaining_ms(void)
{
    unsigned long t=millis();
    unsigned long delta_ms=0;

    if (t>=tick0_ms)
      delta_ms=t-tick0_ms;
    else
      delta_ms=0xFFFFFFFF-tick0_ms+t;

    if ( (delta_ms>=duration_ms) || (running==false) )
      return 0;
    else
      return duration_ms-delta_ms;
}

/**
 * @brief Retourne la duree pre-programmee du timer
 * @return Duree du timer en ms
*/
unsigned long Timer::getDuration_ms(void)
{
  return duration_ms;
}

/**
 * @brief Fonction tick a appeller periodiquement (aussi rapidement que possible)
 * @return true si le timer vient d'atteindre sa duree (Il sera de nouveau false au prochain appel)
*/
bool Timer::tick(void)
{
  unsigned long remain_ms=getRemaining_ms();

  if (running==true)
  {
    if (remain_ms==0)
    {
      if (single==true)
      {
        running=false;
      }

      this->tick0_ms=millis();
        
      return true;
    }
  }
    
  return false;    
}

/**
 * @brief Regle la duree du timer (il faut re-faire start apres)
 * @param[in] duration_ms Duree en ms
*/
void Timer::setDuration_ms(unsigned long duration_ms)
{
  this->duration_ms=duration_ms;
}

/**
 * @brief Indique si le timer est toujours en cours (pas arrêté)
 * @return true si le timer est toujours en cours
*/
bool Timer::isRunning(void)
{
  return running;
}

/**
 * @brief Stoppe le timer
*/
void Timer::stop(void)
{
  this->running=false;
}

/**
 * @brief Demarre le timer
*/
void Timer::start(void)
{
  this->running=true;
  this->tick0_ms=millis();
}
