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
  send_state=OFF;
  send_pos=0;
  send_acked=false;
  send_pSlave=nullptr;
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
  send_state=OFF;
  send_pos=0;
  send_acked=false;
  send_pSlave=nullptr;
}

void WifiComm::setPower(bool on)
{
  if (on)
  {
    digitalWrite(PIN_PWR_WIFI,HIGH);
    digitalWrite(PIN_PWR_WIFI_INV,LOW);
    send_state=SEND_MASTER;
    send_pos=0;
    send_acked=false;
  }
  else
  {
    send_state=OFF;
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

  static StaticJsonDocument<200> doc;
  doc["type"]="cmds";
  doc["cmds"]=cmds;
  doc["ctrl"]=active;

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
  char dteIso[25];
  int year,month,day,hour,minute,second;
  api_get_date_hour(day, month, year, hour, minute, second);
  sprintf(dteIso,"%02d/%02d/%04dT%02d:%02d:%02dZ",
      day,
      month,
      year,
      hour,
      minute,
      second
      );
  doc["date"]=dteIso;

  static char jsonOutput[200];
  serializeJson(doc, jsonOutput,200);

  pStr->print("\x01");
  pStr->write(jsonOutput);
  pStr->print("\x02");
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
            send_acked=true;
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
  switch (send_state)
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
      logger.println("Send master");
      pubMasterInfo();
      tmrSendAck.start();
      send_acked=false;
      send_state=WAIT_ACK_MASTER;
      break;
    }
    case WAIT_ACK_MASTER:
    {
      if (tmrSendAck.tick()==true)
        send_state=SEND_MASTER;
      else if (send_acked==true)
        send_state=SEND_FIRST;
      break;
    }
    case SEND_FIRST:
    {
      logger.println("Send first");
      api_latch_slaves();
      SlavesList *pSlaveList=api_get_latched_slaves();
      if (pSlaveList!=nullptr)
      {
        send_pSlave=pSlaveList->findFirstSlave(send_pos);
        if (send_pSlave==nullptr)
          send_state=IDLE;
        else
          send_state=SEND;
      }
      else
      {
        send_state=IDLE;
      }

      break;
    }
    case SEND:
    {
      logger.print("Send @");
      logger.println(send_pSlave->addr);
      if (send_pSlave->addr==1)
        pubInfo((Pump*)send_pSlave);
      else
        pubInfo((Oya *)send_pSlave);

      tmrSendAck.start();
      send_acked=false;
      send_state=WAIT_ACK;
      break;
    }
    case WAIT_ACK:
    {
      if (tmrSendAck.tick()==true)
        send_state=SEND;
      else if (send_acked==true)
        send_state=SEND_NEXT;
      break;
    }
    case SEND_NEXT:
    {
      SlavesList *pSlaveList=api_get_latched_slaves();
      if (pSlaveList!=nullptr)
      {
        send_pSlave=pSlaveList->findNextSlave(send_pos);
        if (send_pSlave==nullptr)
          send_state=IDLE;
        else
          send_state=SEND;
      }
      else
      {
        send_state=IDLE;
      }
      break;
    }
  }
}

void WifiComm::loop(void)
{
  if (pStr==nullptr)
    return;

  recv();
  sendTask();

  if (tmrRemoteActive.tick()==true)
  {
    flgRemoteActive=false;
    commands=0;
  }

  if (tmrAlive.tick()==true)
  {
    flgAlive=false;
  }
}
