/*
 * pump.h
 */
#ifndef PUMP_INCLUDED
#define PUMP_INCLUDED

#include "timer.h"

class Pump
{
  private:
    bool m_enabled;
    bool m_forced;
    bool m_auto;
    bool m_battOk;

    bool m_cmd;
    bool m_out;

    Timer m_timer;
    
  public:   
    Pump();

    void loop(void);

    void setEnable(bool flgEnabled);
    void setForced(bool flgForced);
    void setAuto(bool flgAuto);
    void setBattOK(bool flgBattOk);

    void startTimer(long delay_ms);
    void stopTimer(void);

    bool isEnabled(void);
    bool isAuto(void);

    bool getError(void);
    bool getOut(void);
};
#endif
