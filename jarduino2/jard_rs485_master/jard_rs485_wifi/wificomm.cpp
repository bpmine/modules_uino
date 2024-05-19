/**
 * @file wificomm.cpp
 * @brief IMPLEMENTATION - GESTION DE LA COMMUNICATION WIFI
 **/
#include "wificomm.h"

#include <arduino.h>
#include <ArduinoJson.h>

#define COMM_SOF 1
#define COMM_EOF 2

#define COMM_STATE_IDLE         (0)
#define COMM_STATE_RECV         (1)
#define COMM_STATE_COMMANDS     (2)
#define COMM_STATE_MASTER       (3)
#define COMM_STATE_PUMP         (4)
#define COMM_STATE_FIRST_OYA    (5)
#define COMM_STATE_OYA          (6)
#define COMM_STATE_NEXT_OYA     (7)
#define COMM_STATE_LATCH        (8)

WifiComm Comm;

WifiComm::WifiComm()
{
  pos=0;
  pStr=nullptr;
  commands_to_send=0;
  curAddr=0;
  config_slaves=0;
  state=COMM_STATE_IDLE;
  flgCmdActive=false;
  flgActive=false;
}

void WifiComm::begin(HardwareSerial* pSerial)
{
  pStr=pSerial;
  if (pStr!=nullptr)
  {
    pStr->begin(9600);
  }

  pos=0;
  commands_to_send=0;
  curAddr=0;
  config_slaves=0;
  state=COMM_STATE_IDLE;
  tmrCycle.start();
  flgCmdActive=false;
  flgActive=false;
}

OyasInfo &WifiComm::getOyasInfo(void)
{
  return latchedInfos;
}

void WifiComm::cycle_management(void)
{
  switch (state)
  {
    case COMM_STATE_IDLE:
    default:
    {
      curAddr=0;
      config_slaves=0;
      
      if (tmrCycle.tick()==true)
      {
        if (flgCmdActive==true)
          state=COMM_STATE_COMMANDS;
        else
          state=COMM_STATE_MASTER;
      }
      
      break;
    }

    case COMM_STATE_RECV:
    {
      if (tmrAnswer.tick()==true)
      {
        flgActive=false;
        commands_to_send=0;
        config_slaves=0;
        curAddr=0;
        state=COMM_STATE_IDLE;
      }

      break;
    }

    case COMM_STATE_COMMANDS:
    {
      StaticJsonDocument<50> doc;
      doc["req"]="cmds";
      doc["cmds"]=commands_to_send;
        
      char jsonOutput[100];
      serializeJson(doc, jsonOutput);
      pStr->print("\x01");
      pStr->print(jsonOutput);
      pStr->print("\x02");

      state=COMM_STATE_RECV;
      
      break;
    }

    case COMM_STATE_MASTER:
    {
      StaticJsonDocument<50> doc;
      doc["req"]="master";
        
      char jsonOutput[100];
      serializeJson(doc, jsonOutput);
      pStr->print("\x01");
      pStr->print(jsonOutput);
      pStr->print("\x02");

      state=COMM_STATE_RECV;
      
      break;
    }

    case COMM_STATE_PUMP:
    {
      StaticJsonDocument<50> doc;
      doc["req"]="pump";
        
      char jsonOutput[100];
      serializeJson(doc, jsonOutput);
      pStr->print("\x01");
      pStr->print(jsonOutput);
      pStr->print("\x02");

      state=COMM_STATE_RECV;
      
      break;
    }

    case COMM_STATE_FIRST_OYA:
    {
      curAddr=2;
      state=COMM_STATE_OYA;
      break;
    }
    
    case COMM_STATE_OYA:
    {
      StaticJsonDocument<50> doc;
      doc["req"]="oya";
      doc["addr"]=curAddr+1;
        
      char jsonOutput[100];
      serializeJson(doc, jsonOutput);
      pStr->print("\x01");
      pStr->print(jsonOutput);
      pStr->print("\x02");

      state=COMM_STATE_RECV;
      
      break;
    }
   
    case COMM_STATE_NEXT_OYA:
    {
      curAddr=curAddr+1;
      if (curAddr<NB_SLAVES_MAX)
        state=COMM_STATE_OYA;
      else
        state=COMM_STATE_LATCH;
        
      break;  
    }

    case COMM_STATE_LATCH:
    {
      latchedInfos.copyFrom(curInfos);
      state=COMM_STATE_IDLE;
      break;
    }
  }  
}

void WifiComm::recv_management(void)
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

          if (strcmp((char *)buffer,"\x01ERR\x02")==0)
          {
            Serial.println("nok");
            return;
          }

          StaticJsonDocument<500> doc;
          DeserializationError error = deserializeJson(doc, buffer);
          pos=0;

          if (error)
          {
            Serial.println("nok2");
            Serial.println(error.f_str());
            return;
          }

          pos=0;

          if (strcmp(doc["type"],"cmds")==0)
          {            
            if (doc["res"]==true)
            {
              curInfos.commands=doc["cmds"];
              curInfos.ons=doc["ons"];
              curInfos.lows=doc["lows"];
              curInfos.highs=doc["highs"];
              curInfos.flow=doc["flow"];
              curInfos.config_slaves=doc["config_slaves"];
              
              tmrAnswer.start();
              flgActive=true;              
              state=COMM_STATE_MASTER;
            }
            else
            {
              Serial.println("nok3");
            }
          }
          else if (strcmp(doc["type"],"master")==0)
          {
            curInfos.config_slaves=doc["config_slaves"];
            config_slaves=curInfos.config_slaves;
            
            tmrAnswer.start();
            flgActive=true;
            state=COMM_STATE_PUMP;
          }
          else if (strcmp(doc["type"],"pump")==0)
          {
            if (doc["on"]==true)
              curInfos.ons|=0x01;
            else 
              curInfos.ons&=~0x01;
              
            if (doc["comms_ok"]==true)
              curInfos.comms_ok|=0x01;
            else 
              curInfos.comms_ok&=~0x01;

            curInfos.flow=doc["flow"];
            
            curInfos.temps[0]=doc["temp"];
            curInfos.hums[0]=doc["hum"];
            curInfos.voltages[0]=doc["voltage"];
            curInfos.ticks_ms[0]=doc["tick"];
            curInfos.totals_s[0]=doc["total_on_s"];
                        
            tmrAnswer.start();
            flgActive=true;
            state=COMM_STATE_FIRST_OYA;
          }
          else if (strcmp(doc["type"],"oya")==0)
          {
            unsigned char addr=doc["addr"];
            if ((addr>1) && (addr<15))
            {
              unsigned short mask = 1 << (addr-1);
              if (doc["on"]==true)
                curInfos.ons|=mask;
              else 
                curInfos.ons&=~mask;
                
              if (doc["comms_ok"]==true)
                curInfos.comms_ok|=mask;
              else 
                curInfos.comms_ok&=~mask;

              if (doc["low"]==true)
                curInfos.lows|=mask;
              else
                curInfos.lows&=~mask;
                
              if (doc["high"]==true)
                curInfos.highs|=mask;
              else
                curInfos.highs&=~mask;

              curInfos.temps[addr-1]=doc["temp"];
              curInfos.hums[addr-1]=doc["hum"];
              curInfos.voltages[addr-1]=doc["voltage"];
              curInfos.ticks_ms[addr-1]=doc["tick"];
              curInfos.totals_s[addr-1]=doc["total_on_s"];

              tmrAnswer.start();
              flgActive=true;
              state=COMM_STATE_NEXT_OYA;
            }              
          }
          else
          {
            tmrAnswer.stop();
            flgActive=false;
            state=COMM_STATE_IDLE;
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

void WifiComm::loop(void)
{
  if (pStr==nullptr)
    return;

  cycle_management();
  recv_management();
}
