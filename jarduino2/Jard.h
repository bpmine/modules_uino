/*
 * Jard.h
 */
#ifndef JARD_INCLUDED
#define JARD_INCLUDED

#include "IJardCmd.h"

class Jard: public IJardCmd
{
  private:
    int m_batt_level_dxv;
    int m_sun_level_dxv;
    int m_temp_deg;
    int m_hum_pc;
  
    void setDate(int day,int month,int year);
    void setHour(int hour,int minute);

    void forcePompe(int i_iNumPompe,bool i_flgON);
    void enablePompe(int i_iNumPompe,bool i_flgEnabled);
    void setAuto(int i_iNumPompe,bool i_flgAuto);

    unsigned short getBattLevel(void);
    unsigned short getSunLevel(void);
    int getTemp(void);
    unsigned char getHum(void);    

    void save(void);
    void load(void);
  
  public:
    Jard();

    void init();

    void setBatLevel(int i_level_dxv);
    void setSunLevel(int i_level_dxv);
    void setTemp(int i_temp_dg);
    void setHum(int i_hum_dg);

    void getDateStr(char *o_strDate,int i_maxSize);
    void loop(void);

  
};
#endif
