/*
 * Jard.h
 */
#ifndef JARD_INCLUDED
#define JARD_INCLUDED

#include "IJardCmd.h"
#include "timer.h"

#define TEMPS_TIMER_MS    (60*60*1000UL)
#define TEMPS_BLINK_MS    (800UL)

#define SEUIL_LOW_BATT_L  (90)
#define SEUIL_LOW_BATT_H  (100)
#define SEUIL_LOW_SUN_L   (138)
#define SEUIL_LOW_SUN_H   (142)

class Jard: public IJardCmd
{
  private:
    Timer tmrComm=Timer(5000,false);
    Timer tmrBlink;
    bool m_flgBlk;
  
    int m_batt_level_dxv;
    int m_sun_level_dxv;
    int m_temp_deg;
    int m_hum_pc;

    bool m_flgBattOk;
    bool m_flgSunOk;

    unsigned char m_ucOldMin;
    unsigned char m_ucMinsSun;

    void aliveComm(void);
    void setDate(int day,int month,int year);
    void setHour(int hour,int minute);    
    void getDate(unsigned short *o_pusYear,unsigned char *o_pucMonth,unsigned char *o_pucDay,unsigned char *o_pucHour,unsigned char *o_pucMin,unsigned char *o_pucSec);
    void getHour(unsigned char *o_pucHour,unsigned char *o_pucMin,unsigned char *o_pucSec);
    void setSheduler(int num,unsigned char ucStartHour,unsigned char ucStartMin,unsigned char ucDuration_min,unsigned char ucDaysOfWeek);
    void getSheduler(int num,unsigned char *o_ucStartHour,unsigned char *o_ucStartMin,unsigned char *o_ucDuration_min,unsigned char *o_ucDaysOfWeek);

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
