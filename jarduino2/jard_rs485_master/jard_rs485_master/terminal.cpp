/**
 * @file terminal.cpp
 * @brief Gestion du terminal
 *
 * @todo Ajouter les comandes setdate, settime
 **/
#ifndef TERMINAL_HEADER_INCLUDED
#define TERMINAL_HEADER_INCLUDED

#include "app.h"
#include <arduino.h>
#include "master.h"

char _bufferTerm[200];
int _posBufferTerm=0;

static void _help(void)
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
  Serial.println("  - stats");
  Serial.println("  - razerrs");
}

static bool _parseOnOff(const char *params,bool def)
{
  if (strcmp(params,"on")==0)
    return true;
  else if (strcmp(params,"off")==0)
    return false;
  else
    return def;
}

static void _OnOffAns(const char *strTitle,bool on)
{
  Serial.print(strTitle);
  Serial.print(": ");
  if (on==true)
    Serial.println("ON");
  else
    Serial.println("OFF");
}

static void _term_exec_oya(const char *strParams)
{
  int addr;
  char onoff[20];
  Serial.println(strParams);
  if ( (strlen(strParams)>19) || (sscanf(strParams,"%X %s",&addr,onoff)!=2) || (addr<2) || (addr>14))
  {
    Serial.println("Format incorrect!");
    return;      
  }
  
  int pos=0;
  Oya * p=app_term_find_first_oya(pos);
  while (p!=NULL)
  {
    if (p->addr==addr)
    {
      char tmp[10];
      bool on=_parseOnOff(onoff,false);
      app_set_oya(addr, on);
      sprintf(tmp,"Oya %01X",addr);
      _OnOffAns(tmp,on);      
      return;
    }
    p=app_term_find_next_oya(pos);
  }
  
  Serial.println("Oya non trouvé!");
}

static void _term_exec_pump(const char *strParams)
{
    Pump *p=app_term_get_pump();
    if (p==nullptr)
    {
      Serial.println("Erreur interne: pump est NULL");
      return;
    }

    if (strParams==NULL)
    {      
      Serial.println("__________");
      Serial.print("Pompe ");
      Serial.print((int)p->addr);
      Serial.println(":");

      if (p->comm_ok==true)
      {
        Serial.print("  - Etat : ");
        Serial.println(p->on==true?"ON":"OFF");
        Serial.print("  - Flow : ");
        Serial.println(p->flow);
        Serial.print("  - temp : ");
        Serial.print(p->temp_dg);
        Serial.println("°C");
        Serial.print("  - hum  : ");
        Serial.print(p->hum_pc);
        Serial.println("%");
        Serial.print("  - total s    : ");
        Serial.print(p->total_slave_on_s);
        Serial.println("s");
        Serial.print("  - total errs : ");
        Serial.println(p->total_slave_errs);
        Serial.print("  - tick       : ");
        Serial.println(p->last_slave_tick_ms);
      }
      else
      {
        Serial.println("NO COMM");
      }
    }
    else
    {
      bool on=_parseOnOff(strParams,false);
      app_set_pompe(on);
      _OnOffAns("Pompe",on);
    }
}

static void _term_exec_oyas(void)
{
    int pos=0;
    Serial.println("__________");
    Oya *pOya=app_term_find_first_oya(pos);
    while (pOya!=NULL)
    {
      char tmp[12];
      sprintf(tmp,"Oya @%01X : ",pOya->addr);
      Serial.print(tmp);

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
        Serial.println("");
      }
      else
      {
        Serial.println("NO COMM");
      }

      pOya=app_term_find_next_oya(pos);
    }
    Serial.println();
}

static void _term_disp_slave_stats(Slave *p)
{
  char tmp[20];

  if (p->addr!=1)
    sprintf(tmp,"Oya %01X   : ",p->addr);
  else
    sprintf(tmp,"Pompe %01X : ",p->addr);

  Serial.print(tmp);

  sprintf(tmp," off=%ld ",p->cycles_since_off);Serial.print(tmp);
  sprintf(tmp," on=%ld ",p->cycles_since_on);Serial.print(tmp);
  sprintf(tmp," nok=%ld ",p->cycles_since_nok);Serial.print(tmp);
  sprintf(tmp," ok=%ld ",p->cycles_since_ok);Serial.print(tmp);
  sprintf(tmp," errs=%ld ",p->cycles_errors);Serial.print(tmp);
  Serial.println("");
}

static void _term_exec_stats(void)
{
  Serial.println("___________________");
  Serial.println("Statistiques:");
  int pos=0;
  Oya *pOya=app_term_find_first_oya(pos);
  while (pOya!=NULL)
  {
    _term_disp_slave_stats(pOya);
    pOya=app_term_find_next_oya(pos);
  }

  Pump *pPump=app_term_get_pump();
  if (pPump!=NULL)
  _term_disp_slave_stats(pPump);
}

static void _term_exec_razerrs(void)
{
  int pos=0;
  Oya *pOya=app_term_find_first_oya(pos);
  while (pOya!=NULL)
  {
    pOya->razErrors();
    pOya=app_term_find_next_oya(pos);
  }
  Pump *pPump=app_term_get_pump();
  if (pPump!=NULL)
    pPump->razErrors();
  
  Serial.println("RAZ errors...");
}

static void _execCmd(const char *strCmd,const char *strParams)
{
  if (strcmp(strCmd,"test")==0)
  {
    Serial.println("TEST OK");
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
    _term_exec_oya(strParams);
  }
  else if (strcmp(strCmd,"master")==0)
  {
    bool on=_parseOnOff(strParams,false);
    app_term_master(on);
    _OnOffAns("Maitre 485",on);
  }
  else if (strcmp(strCmd,"trace")==0)
  {
    bool on=_parseOnOff(strParams,false);
    app_term_trace(on);
    _OnOffAns("Traces",on);
  }
  else if (strcmp(strCmd,"stats")==0)
  {
    _term_exec_stats();
  }
  else if (strcmp(strCmd,"razerrs")==0)
  {
    _term_exec_razerrs();
  }
}

void serialEvent(void)
{
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


#endif
