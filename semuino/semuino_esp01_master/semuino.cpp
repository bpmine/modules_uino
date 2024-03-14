#include "semuino.h"
#include "master.h"

T_OUT _out;

void semuino_init(void)
{
  _out.ctrl=2;
  _out.level=124;
  _out.modeRGB1=0;
  _out.modeRGB2=0;
  _out.modeRGB3=0;  
  _out.leds=0;

  master_set_out_values(&_out);  
}

void semuino_loop(void)
{
  master_loop(); 
}
