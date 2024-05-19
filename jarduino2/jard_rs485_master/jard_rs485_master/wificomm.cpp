/**
 * @file wificomm.cpp
 * @brief IMPLEMENTATION - GESTION DE LA COMMUNICATION WIFI
 **/
#include "wificomm.h"

#include <arduino.h>
#include <ArduinoJson.h>

#include "api.h"
#include "slaves.hpp"
#include "pins.h"

#define COMM_SOF 1
#define COMM_EOF 2

WifiComm Comm;

WifiComm::WifiComm()
{
  pos=0;
  pStr=nullptr;
  flgActive=false;
  commands=0;
}

void WifiComm::begin(HardwareSerial* pSerial)
{
  pStr=pSerial;
  if (pStr!=nullptr)
  {
    pStr->begin(9600);
    digitalWrite(PIN_PWR_WIFI,HIGH);
    digitalWrite(PIN_PWR_WIFI_INV,LOW);
  }

  pos=0;
  flgActive=false;
  commands=0;
}

void WifiComm::pubMasterInfo(void)
{
  unsigned short sc=api_get_slaves_config();
  StaticJsonDocument<150> doc;

  doc["type"]="master";
  doc["config_slaves"]=api_get_slaves_config();

  JsonArray list = doc.createNestedArray("slaves");
  for (int i=0;i<15;i++)
  {
    unsigned short mask=1 << i;
    if ((sc & mask) == mask)
      list.add(i+1);
  }

  char jsonOutput[150];
  serializeJson(doc, jsonOutput);

  pStr->print("\x01");
  pStr->print(jsonOutput);
  pStr->print("\x02");
}

void WifiComm::pubPumpInfo(void)
{
  Pump *pump=api_get_pump();
  if (pump==nullptr)
  {
    pStr->print("\x01ERR\x02");
    return;
  }

  StaticJsonDocument<150> doc;
  doc["type"]="pump";
  doc["addr"]=(int)pump->addr;
  doc["on"]=pump->on;
  doc["cmd"]=pump->cmd;
  doc["flow"]=pump->flow;
  doc["temp"]=pump->temp_dg;
  doc["hum"]=pump->hum_pc;
  doc["voltage"]=pump->voltage;
  doc["comm_ok"]=pump->comm_ok;
  doc["tick"]=pump->last_slave_tick_ms;
  doc["total_on_s"]=pump->total_slave_on_s;

  char jsonOutput[150];
  serializeJson(doc, jsonOutput);

  pStr->print("\x01");
  pStr->print(jsonOutput);
  pStr->print("\x02");
}

void WifiComm::pubOyaInfo(int addr)
{
  Oya *oya=api_get_oya(addr);
  if (oya==nullptr)
  {
    pStr->print("\x01ERR\x02");
    return;
  }

  StaticJsonDocument<150> doc;
  doc["type"]="oya";
  doc["addr"]=(int)oya->addr;
  doc["on"]=oya->on;
  doc["cmd"]=oya->cmd;
  doc["high"]=oya->high;
  doc["low"]=oya->low;
  doc["temp"]=oya->temp_dg;
  doc["hum"]=oya->hum_pc;
  doc["voltage"]=oya->voltage;
  doc["comm_ok"]=oya->comm_ok;
  doc["tick"]=oya->last_slave_tick_ms;
  doc["total_on_s"]=oya->total_slave_on_s;

  char jsonOutput[150];
  serializeJson(doc, jsonOutput);

  pStr->print("\x01");
  pStr->print(jsonOutput);
  pStr->print("\x02");
}

void WifiComm::execCommands(unsigned short cmds)
{
  tmrComms.start();
  commands=cmds;
  flgActive=true;

  StaticJsonDocument<150> doc;
  doc["type"]="cmd";
  doc["cmds"]=cmds;
  doc["res"]=true;

  unsigned short ons=0;
  unsigned short comms_ok=0;
  Pump *pump=api_get_pump();
  if (pump!=nullptr)
  {
    if (pump->on==true)
      ons|=0x01;
    if (pump->comm_ok==true)
      comms_ok|=0x01;
    doc["flow"]=pump->flow;
  }

  int pos;
  unsigned short lows=0;
  unsigned short highs=0;
  Oya *oya=api_find_first_oya(pos);
  while (oya!=nullptr)
  {
    unsigned short mask=1 << (oya->addr);
    if (oya->on==true)
      ons|=mask;

    if (oya->low==true)
      lows|=mask;

    if (oya->high==true)
      highs|=mask;

    if (oya->comm_ok==true)
      comms_ok|=mask;

    oya=api_find_next_oya(pos);
  }
  doc["ons"]=ons;
  doc["lows"]=lows;
  doc["highs"]=highs;
  doc["comms_ok"]=comms_ok;
  doc["config_slaves"]=api_get_slaves_config();

  char jsonOutput[150];
  serializeJson(doc, jsonOutput);

  pStr->print("\x01");
  pStr->print(jsonOutput);
  pStr->print("\x02");

  flgActive=true;
  tmrComms.start();
}

bool WifiComm::isActive(void)
{
  return flgActive;
}

unsigned short WifiComm::getCommands(void)
{
  return commands;
}

void WifiComm::loop(void)
{
  if (pStr==nullptr)
    return;

  while (pStr->available())
  {
    int b = pStr->read();
    if ((b > 0) && (b < 255))
    {
      if (pos==0)
      {
        if (b==COMM_SOF)
        {
          pos++;
        }
      }
      else if (pos<COMM_BUFFER_MAX_SIZE)
      {
        if ( (b!=COMM_EOF) && (b!=COMM_SOF) )
        {
          buffer[pos-1]=b;
          pos++;
        }
        else if (b==COMM_SOF)
        {
          pos=0;
        }
        else
        {
          buffer[pos-1]=0;

          StaticJsonDocument<50> doc;
          DeserializationError error = deserializeJson(doc, buffer);
          pos=0;

          if (error)
          {
            pStr->print("\x01");
            pStr->print(F("ERR: Erreur de désérialisation: "));
            pStr->print(error.f_str());
            pStr->print("\x02");

            return;
          }

          if (strcmp(doc["req"],"cmds")==0)
          {
            unsigned short cmds=doc["cmds"];
            execCommands(cmds);
          }
          else if (strcmp(doc["req"],"master")==0)
          {
            pubMasterInfo();
          }
          else if (strcmp(doc["req"],"pump")==0)
          {
            pubPumpInfo();
          }
          else if (strcmp(doc["req"],"oya")==0)
          {
            int addr=doc["addr"];
            pubOyaInfo(addr);
          }
          else if (strcmp(doc["req"],"test")==0)
          {
            pStr->print("\x01");
            pStr->print("{\"type\":\"test\",\"res\":\"true\"}");
            pStr->print("\x02");
          }
        }
      }
      else
      {
        pos=0;
      }
    }
  }

  if (tmrComms.tick()==true)
  {
    flgActive=false;
    commands=0;
  }
}
