#ifndef MASTER_HEADER_INCLUDED
#define MASTER_HEADER_INCLUDED

#include <arduino.h>

typedef struct
{
  byte ctrl;
  byte level;
  byte modeRGB1;
  byte modeRGB2;
  byte modeRGB3;  
  byte leds;
} T_OUT;

typedef struct
{
 byte h1; 
 byte h2;
 byte h3;
 byte inputs;
} T_IN;


extern void master_init(void);
extern void master_loop(void);

extern void master_get_in_values(T_IN *pIN);
extern void master_set_out_values(T_OUT *pOUT);

#endif
