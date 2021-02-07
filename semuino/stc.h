/**
 * @file stc.h
 * 
 * @brief EN TETE STC - HORLOGE LOGICIELLE
*/
#ifndef STC_HEADER_INCLUDED
#define STC_HEADER_INCLUDED

class Stc
{
  private:
    long m_ticks_ms;
    int m_hours;
    int m_mins;
    int m_secs;

    long m_t;

    void updateHMS();
  
  public:
    Stc();

    int getHours();
    int getMinutes();
    int getSeconds();

    void setTime(int h,int m,int s);
    
    void tick();
};

#endif
