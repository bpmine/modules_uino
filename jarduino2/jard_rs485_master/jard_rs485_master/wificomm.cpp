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
#include "Logger.h"

#define COMM_SOF 1
#define COMM_EOF 2

WifiComm Comm;

typedef enum {
  OFF,
  IDLE,               ///< Ne fait rien
  SEND_MASTER,        ///< Envoi de l'info master
  WAIT_ACK_MASTER,    ///< Attente Ack suite info master
  SEND_FIRST,         ///< Recherche premier esclave a envoyer
  SEND,               ///< Envoi de l'info d'un esclave
  WAIT_ACK,           ///< Attente Ack suite info esclave
  SEND_NEXT           ///< Recherche esclave suivant a envoyer
} T_SEND_STATE;

WifiComm::WifiComm()
{
  pos=0;
  pStr=nullptr;
  flgRemoteActive=false;
  flgAlive=false;
  commands=0;

}

void WifiComm::begin(HardwareSerial* pSerial)
{
  pStr=pSerial;
  if (pStr!=nullptr)
  {
    pStr->begin(9600);
    digitalWrite(PIN_PWR_WIFI,LOW);
    digitalWrite(PIN_PWR_WIFI_INV,HIGH);
  }

  pos=0;
  flgAlive=false;
  flgRemoteActive=false;
  commands=0;

}

void WifiComm::setPower(bool on)
{
  if (on)
  {
    digitalWrite(PIN_PWR_WIFI,HIGH);
    digitalWrite(PIN_PWR_WIFI_INV,LOW);

  }
  else
  {

    digitalWrite(PIN_PWR_WIFI,LOW);
    digitalWrite(PIN_PWR_WIFI_INV,HIGH);
  }

  pos=0;
  flgAlive=false;
  flgRemoteActive=false;
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

void WifiComm::pubInfo(Pump *pPump)
{
  if (pPump==nullptr)
    return;

  StaticJsonDocument<150> doc;
  doc["type"]="pump";
  doc["addr"]=(int)pPump->addr;
  doc["on"]=pPump->on;
  doc["cmd"]=pPump->cmd;
  doc["flow"]=pPump->flow;
  doc["temp"]=pPump->temp_dg;
  doc["hum"]=pPump->hum_pc;
  doc["voltage"]=pPump->voltage;
  doc["comm_ok"]=pPump->comm_ok;
  doc["tick"]=pPump->last_slave_tick_ms;
  doc["total_on_s"]=pPump->total_slave_on_s;

  char jsonOutput[150];
  serializeJson(doc, jsonOutput);

  pStr->print("\x01");
  pStr->print(jsonOutput);
  pStr->print("\x02");
}

void WifiComm::pubInfo(Oya *pOya)
{
  if (pOya==nullptr)
    return;

  StaticJsonDocument<150> doc;
  doc["type"]="oya";
  doc["addr"]=(int)pOya->addr;
  doc["on"]=pOya->on;
  doc["cmd"]=pOya->cmd;
  doc["high"]=pOya->high;
  doc["low"]=pOya->low;
  doc["temp"]=pOya->temp_dg;
  doc["hum"]=pOya->hum_pc;
  doc["voltage"]=pOya->voltage;
  doc["comm_ok"]=pOya->comm_ok;
  doc["tick"]=pOya->last_slave_tick_ms;
  doc["total_on_s"]=pOya->total_slave_on_s;

  char jsonOutput[150];
  serializeJson(doc, jsonOutput);

  pStr->print("\x01");
  pStr->print(jsonOutput);
  pStr->print("\x02");
}

void WifiComm::execCommands(unsigned short cmds,bool active)
{
  if (active==true)
  {
    flgRemoteActive=true;
    commands=cmds;
  }
  else
  {
    flgRemoteActive=false;
    commands=0;
  }
}

bool WifiComm::isRemoteActive(void)
{
  return flgRemoteActive;
}

bool WifiComm::isAlive(void)
{
  return flgAlive;
}
unsigned short WifiComm::getCommands(void)
{
  return commands;
}

void WifiComm::recv(void)
{
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
            pStr->print(F("ERR: Erreur de dé-sérialisation: "));
            pStr->print(error.f_str());
            pStr->print("\x02");

            return;
          }
          if (strcmp(doc["req"],"cmds")==0)
          {
            unsigned short cmds=doc["cmds"];
            bool ctrl=doc["ctrl"];
            execCommands(cmds,ctrl);
            tmrRemoteActive.start();
            tmrAlive.start();
          }
          else if (strcmp(doc["req"],"ack")==0)
          {
            //send_acked=true;
            tmrRemoteActive.start();
            tmrAlive.start();
          }
          else if (strcmp(doc["req"],"razt")==0)
          {
            api_raz_all_time();
            tmrRemoteActive.start();
            tmrAlive.start();
          }
          else if (strcmp(doc["req"],"test")==0)
          {
            pStr->print("\x01");
            pStr->print("{\"type\":\"test\",\"res\":\"true\"}");
            pStr->print("\x02");
            tmrAlive.start();
          }
        }
      }
      else
      {
        pos=0;
      }
    }
  }
}

void WifiComm::sendTask(void)
{
  /*switch (send_state)
  {
    case OFF:
    {
      break;
    }
    case IDLE:
    {

      break;
    }
    case SEND_MASTER:
    {

      break;
    }
    case WAIT_ACK_MASTER:
    {
      break;
    }
    case SEND_FIRST:
    {

      break;
    }
    case SEND:
    {

      break;
    }
    case WAIT_ACK:
    {

      break;
    }
    case SEND_NEXT:
    {


      break;
    }
  }*/
}

void WifiComm::loop(void)
{
  if (pStr==nullptr)
    return;

  //recv();
  //sendTask();

  /*if (tmrRemoteActive.tick()==true)
  {
    flgRemoteActive=false;
    commands=0;
  }

  if (tmrAlive.tick()==true)
  {
    flgAlive=false;
  }*/
}

