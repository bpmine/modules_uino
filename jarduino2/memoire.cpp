#include "memoire.h"

#include <EEPROM.h>

#define MAGIC1  (0xA5)
#define MAGIC2  (0xDA)

bool memoire_save(T_MEMOIRE *pMemoire)
{
  pMemoire->bMagicStart=MAGIC1;
  pMemoire->bMagicEnd=MAGIC2;
  
  unsigned char *p=(unsigned char *)pMemoire;
  for (int i = 0 ; i < sizeof(T_MEMOIRE) ; i++)
  {
    if ( (i!=1) && (i!=2) )
      EEPROM.update(i,p[i]);
  }    

  return true;
}

bool memoire_load(T_MEMOIRE *pMemoire)
{
  unsigned char *p=(unsigned char *)pMemoire;

  for (int i = 0 ; i < sizeof(T_MEMOIRE) ; i++)
  {
    p[i]=EEPROM.read(i);
  }  

  return true;
}

bool memoire_set_serial(unsigned char bSerial)
{
  EEPROM.update(2,bSerial);

  return true;
}
