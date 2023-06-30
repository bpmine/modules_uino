#ifndef TERMINAL_HEADER_INCLUDED
#define TERMINAL_HEADER_INCLUDED

#include "app.h"
#include <arduino.h>

void _help(void)
{
  Serial.println("____________________");
  Serial.println("Aide:");
  Serial.println("  - test");
  Serial.println("  - master on/off");
  Serial.println("  - pump on/off");
  Serial.println("  - pump");
  Serial.println("  - oyas");
  Serial.println("  - oya @ on/off");
  Serial.println("  - trace on/off");
  Serial.println("");
}

bool parseOnOff(char *params,bool def)
{
  if (strcmp(params,"on")==0)
    return true;
  else if (strcmp(params,"off")==0)
    return false;
  else
    return def;
}

void _OnOffAns(char *strTitle,bool on)
{
  Serial.print(strTitle);
  Serial.print(": ");
  if (on==true)
    Serial.println("ON");
  else
    Serial.println("OFF");
}

void _term_exec_oya(char *strParams)
{
  char addr;
  char onoff[20];
  Serial.println(strParams);
  if ( (strlen(strParams)>19) || (sscanf(strParams,"%c %s",&addr,onoff)!=2) )
  {
    Serial.println("Format incorrect!");
    return;      
  }
  
  int pos=0;
  Oya * p=app_term_get_next_oya(pos);
  while (p!=NULL)
  {
    if (p->addr==addr)
    {
      bool on=parseOnOff(onoff,false);
      p->cmd=on;
      _OnOffAns("Oya",on);      
      return;
    }
    p=app_term_get_next_oya(pos);
  }
  
  Serial.println("Oya non trouvé!");
}

void _term_exec_pump(char strParams)
{
    Pump *p=app_term_get_pump();
    if (p==NULL)
    {
      Serial.println("Erreur interne pump est NULL");
      return;
    }

    if (strParams==NULL)
    {      
      Serial.println("_____");
      Serial.print("Pompe ");
      Serial.print(p->addr);
      Serial.print(": ");

      if (p->comm_ok==true)
      {
        Serial.print(p->on==true?" on":" off");
        Serial.print(" Flow= ");
        Serial.print(p->flow);
        Serial.print(" temp: ");
        Serial.print(p->temp_dg);
        Serial.print("°C");
        Serial.print(" hum: ");
        Serial.print(p->hum_pc);
        Serial.println("%");
        Serial.print("enabled=");
        Serial.print(p->enabled==true?"true":"false");
        Serial.println("");
      }
      else
      {
        Serial.println("NO COMM");
      }
    }
    else
    {
      bool on=parseOnOff(strParams,false);
      p->cmd=on;
      _OnOffAns("Pompe",on);      
    }  
}

void _term_exec_oyas(void)
{
    int pos=0;
    Serial.println("_____");
    Oya *pOya=app_term_get_next_oya(pos);
    while (pOya!=NULL)
    {
      Serial.print("Oya ");
      Serial.print(pOya->addr);
      Serial.print(": ");

      if (pOya->comm_ok==true)
      {
        Serial.print(pOya->on==true?" on":" off");
        Serial.print(pOya->high==true?"":" high");
        Serial.print(pOya->low==true?"":" low");
        Serial.print(" temp: ");
        Serial.print(pOya->temp_dg);
        Serial.print("°C");
        Serial.print(" hum: ");
        Serial.print(pOya->hum_pc);
        Serial.print("% ");
        Serial.print("enabled=");
        Serial.print(pOya->enabled==true?"true":"false");
        Serial.println("");
      }
      else
      {
        Serial.println("NO COMM");
      }

      pOya=app_term_get_next_oya(pos);
    }
    Serial.println();
}

void _execCmd(char *strCmd,char *strParams)
{
  if (strcmp(strCmd,"test")==0)
  {
    Serial.println("OK");
  }
  else if (strcmp(strCmd,"help")==0)
  {
    _help();
  }
  else if (strcmp(strCmd,"pump")==0)
  {
    _term_exec_pump(strParams);
  }
  else if (strcmp(strCmd,"oyas")==0)
  {
    _term_exec_oyas();
  }
  else if (strcmp(strCmd,"oya")==0)
  {
    Serial.println(strParams);
    _term_exec_oya(strParams);
  }
  else if (strcmp(strCmd,"master")==0)
  {
    bool on=parseOnOff(strParams,false);
    app_term_master(on);
    _OnOffAns("Maître 485",on);
  }
  else if (strcmp(strCmd,"trace")==0)
  {
    bool on=parseOnOff(strParams,false);
    app_term_trace(on);
    _OnOffAns("Traces",on);
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
