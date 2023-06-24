#ifndef TERMINAL_HEADER_INCLUDED
#define TERMINAL_HEADER_INCLUDED

#include "app.h"
#include <arduino.h>


bool parseOnOff(char *params,bool def)
{
  if (strcmp(params,"on")==0)
    return true;
  else if (strcmp(params,"off")==0)
    return false;
  else
    return def;
}


void _execCmd(char *strCmd,char *strParams)
{
  if (strcmp(strCmd,"test")==0)
  {
    Serial.println("OK");
  }
  else if (strcmp(strCmd,"slaves")==0)
  {
    int pos=0;
    Oya *pOya=app_term_get_next_oya(pos);
    while (pOya!=NULL)
    {
      Serial.print("Oya ");
      Serial.print(pOya->addr);
      Serial.print(pOya->on?" on":" off");
      Serial.print(pOya->high?" high":"");
      Serial.print(pOya->low?" low":"");
      Serial.print(" temp: ");
      Serial.print(pOya->temp_dg);
      Serial.print("°C");
      Serial.print(" hum: ");
      Serial.print(pOya->hum_pc);
      Serial.println("%");

      pOya=app_term_get_next_oya(pos);
    }
  }
  else if (strcmp(strCmd,"master")==0)
  {
    bool on=parseOnOff(strParams,false);
    app_term_master(on);
  }
}

char _bufferTerm[200];
int _posBufferTerm=0;

void serialEvent(void)
{
  char cmd[20];
  char par1[20];
  
  if (Serial.available()>0)
  {
    String st=Serial.readString();
    if (st.length()>20)
      return;

    st.trim();

    int inx=st.indexOf(" ");
    if (inx==-1)
    {
      _execCmd(st.c_str(),NULL);
    }
    else
    {
      _execCmd(st.substring(0,inx).c_str(),st.substring(inx+1).c_str());
    }
  }
}


/*extern void app_term_master(bool on);
extern void app_term_slaves(void);
extern void app_term_log(bool on);
extern void app_term_trace(bool on);
extern void app_slave(char addr,bool on);*/


#endif
