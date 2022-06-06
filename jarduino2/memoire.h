/*
 * memoire.h
 */
#ifndef MEMOIRE_INCLUDED
#define MEMOIRE_INCLUDED

#include <arduino.h>

typedef struct  
{
  unsigned char bMagicStart;
  unsigned char bVersion;
  unsigned char bSerial;
  unsigned char bMagicEnd;
} T_ID;

bool memoire_save_id(T_ID *pID);
bool memoire_load_id(T_ID *pID);


#define MEM_START_ADDR_STATS        (100)
#define MEM_STATS_ADDR_TOT_BOOTS    (MEM_START_ADDR_STATS)
#define MEM_STATS_ADDR_TOT_P1_H     (MEM_START_ADDR_STATS+1*2)
#define MEM_STATS_ADDR_TOT_P2_H     (MEM_START_ADDR_STATS+2*2)
#define MEM_STATS_ADDR_TOT_SUN_H    (MEM_START_ADDR_STATS+3*2)
#define MEM_STATS_ADDR_TOT_BTN1     (MEM_START_ADDR_STATS+4*2)
#define MEM_STATS_ADDR_TOT_BTN2     (MEM_START_ADDR_STATS+5*2)
#define MEM_STATS_ADDR_MAX          (MEM_START_ADDR_STATS+6*2)

bool memoire_stats_reset(int addr);
bool memoire_stats_inc(int addr);
bool memoire_stats_read(int addr,unsigned short *o_pusValue);


#define MEM_START_ADDR_SETTINGS         (MEM_STATS_ADDR_MAX+1)
#define MEM_SETTINGS_ADDR_PUMP1_EN      (MEM_START_ADDR_SETTINGS)
#define MEM_SETTINGS_ADDR_PUMP1_AUTO    (MEM_START_ADDR_SETTINGS+1)
#define MEM_SETTINGS_ADDR_PUMP1_REMOTE  (MEM_START_ADDR_SETTINGS+2)
#define MEM_SETTINGS_ADDR_PUMP2_EN      (MEM_START_ADDR_SETTINGS+10)
#define MEM_SETTINGS_ADDR_PUMP2_AUTO    (MEM_START_ADDR_SETTINGS+11)
#define MEM_SETTINGS_ADDR_PUMP2_REMOTE  (MEM_START_ADDR_SETTINGS+22)
#define MEM_SETTINGS_ADDR_MAX           (MEM_START_ADDR_SETTINGS+30)

bool memoire_set(int addr,unsigned char i_ucValue);
bool memoire_read(int addr,unsigned char *o_pucValue);


#endif
