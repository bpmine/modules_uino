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
  unsigned short usTotalBoots;
  unsigned short ulTotal_h;
  unsigned short ulTotalP1_h;
  unsigned short ulTotalP2_h;
  unsigned short ulTotalSun_h;
  unsigned short ulTotalBtn1;
  unsigned short ulTotalBtn2;
  unsigned char bMinBatt_dxV;
  unsigned char bMaxBatt_dxV;
  unsigned char bMinSun_dxV;
  unsigned char bMaxSun_dxV;
  unsigned char bPump1Enable;
  unsigned char bPump2Enable;
  unsigned char bPump1Auto;
  unsigned char bPump2Auto;
   
  unsigned char bCS;
  unsigned char bMagicEnd;
} T_MEMOIRE;  

bool memoire_save(T_MEMOIRE *pMemoire);
bool memoire_load(T_MEMOIRE *pMemoire);

bool memoire_set_serial(unsigned char bSerial);

#endif
