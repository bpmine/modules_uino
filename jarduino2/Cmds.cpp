/*
   Cmds.cpp

    Created on: 26 juin 2020
        Author: Utilisateur
*/

#include "Cmds.h"
#include "IJardCmd.h"
#include <arduino.h>
#include "pins.h"
#include "autom.h"
#include "memoire.h"

Cmds cmds;

Cmds::Cmds()
{
  m_pSerial = NULL;
}

void Cmds::init(HardwareSerial *pSerial, IJardCmd *pJardCmd)
{
  m_pSerial = pSerial;
  m_pJardCmd = pJardCmd;

  if ( (m_pSerial == NULL) && (m_pJardCmd != NULL) )
    return;

  m_pSerial->begin(115200);
  m_pSerial->println("Boot...");
}

void Cmds::println(char *strMsg)
{
  if (m_pSerial == NULL)
    return;

  m_pSerial->println(strMsg);
}

void Cmds::println(const char *strMsg)
{
  println((char *)strMsg);
}

void Cmds::returnDate(void)
{
  if (m_pSerial == NULL)
    return;

  char strDate[25];
  m_pJardCmd->getDateStr(strDate, 25);
  m_pSerial->println(strDate);
}


void Cmds::execCmdWithParam(char *strCmd, char *strParams)
{
    int num;
  
    m_pSerial->println(strCmd);
    m_pSerial->println(strParams);
    
    if (strcmp(strCmd,"setdate")==0)
    {
      int year=0,month=0,day=0;
      if (sscanf(strParams,"setdate %02d/%02d/%04d",&day,&month,&year)==3)
      {
        m_pJardCmd->setDate(day,month,year);
        returnDate();
      }
      else
      {
        m_pSerial->println("[ERREUR] - Commande incorrecte. Exemple qui marche = \"setdate 01/01/2022\"\n");
      }
    }
    else if (strcmp(strCmd,"settime")==0)
    {
      int hours=0,minutes=0;
      if (sscanf(strParams,"settime %02d:%02d",&hours,&minutes)==2)
      {
        m_pJardCmd->setHour(hours,minutes);
        returnDate();
      }
      else
      {
        m_pSerial->println("[ERREUR] - Commande incorrecte. Exemple qui marche = \"settime 13:45\"\n");
      }
    }
    else if (strcmp(strCmd,"force")==0)
    {
      println((const char *)"[Omm]");
      /*if (sscanf(strParams,"%d",&num)==1)
      {
        m_pJardCmd->forcePompe(num,true);
        m_pSerial->println("[OK]");
      }
      else
      {
        m_pSerial->println("[ERREUR] - Commande incorrecte. Exemple qui marche = \"force 1\" ou \"force 2\"\n");
      }*/
    }
   /* else if (strcmp(strCmd,"unforce")==0)
    {
      if (sscanf(strParams,"%d",&num)==1)
      {
        m_pJardCmd->forcePompe(num,false);
        m_pSerial->println("[OK]");
      }
      else
      {
        m_pSerial->println("[ERREUR] - Commande incorrecte. Exemple qui marche = \"unforce 1\" ou \"unforce 2\"\n");
      }
    }*/
}

void Cmds::execCmd(char *strCmd)
{
  if (strcmp(strCmd,"date")==0)
  {
    returnDate();
  }
  else if (strcmp(strCmd,"help")==0)
  {
    m_pSerial->println("Aide:");
    m_pSerial->println("  - date");
    m_pSerial->println("  - setdate <DD/MM/YYYY>");
    m_pSerial->println("  - settime <HH:MM:SS>");
    m_pSerial->println("  - enable");
    m_pSerial->println("  - disable");
    m_pSerial->println("  - auto");
    m_pSerial->println("  - unauto");
    m_pSerial->println("  - force");
    m_pSerial->println("  - unforce");
    m_pSerial->println("  - values");
    m_pSerial->println("  - info");
    m_pSerial->println("  - settings");
    m_pSerial->println("  - help");    
  }
}


void Cmds::onSerialEvent()
{
  if (m_pSerial == NULL)
    return;

  String cmd = m_pSerial->readStringUntil('\n');
  if (cmd == NULL)
    return;

  m_pSerial->print("Recu ");
  m_pSerial->println(cmd);

  cmd.trim();
  char strCmd[15];
  char strParams[100];
  if (sscanf(cmd.c_str(), "%s %s", strCmd, strParams) == 2)
  {
    execCmdWithParam(strCmd,strParams);
  }
  else
  {
    execCmd(cmd.c_str());
  }

  /*if (cmd.startsWith("date"))
    {
    returnDate();
    }
    else if (cmd.startsWith("setdate"))
    {
    int year=0,month=0,day=0;
    if (sscanf(cmd.c_str(),"setdate %02d/%02d/%04d",&day,&month,&year)==3)
    {
      m_pJardCmd->setDate(day,month,year);
      returnDate();
    }
    else
    {
      m_pSerial->println("[ERREUR] - Commande incorrecte. Exemple qui marche = \"setdate 01/01/2022\"\n");
    }
    }
    else if (cmd.startsWith("settime"))
    {
    int hours=0,minutes=0;
    if (sscanf(cmd.c_str(),"settime %02d:%02d",&hours,&minutes)==2)
    {
      m_pJardCmd->setHour(hours,minutes);
      returnDate();
    }
    else
    {
      m_pSerial->println("[ERREUR] - Commande incorrecte. Exemple qui marche = \"settime 13:45\"\n");
    }
    }
    else if (cmd.startsWith("enable"))
    {
    int num=1;
    if (sscanf(cmd.c_str(),"enable %d",&num)==1)
    {
      m_pJardCmd->enablePompe(num,true);
      m_pSerial->println("[OK]");
    }
    else
    {
      m_pSerial->println("[ERREUR] - Commande incorrecte. Exemple qui marche = \"enable 1\" ou \"enable 2\"\n");
    }
    }
    else if (cmd.startsWith("disable"))
    {
    int num=1;
    if (sscanf(cmd.c_str(),"disable %d",&num)==1)
    {
      m_pJardCmd->enablePompe(num,false);
      m_pSerial->println("[OK]");
    }
    else
    {
      m_pSerial->println("[ERREUR] - Commande incorrecte. Exemple qui marche = \"disable 1\" ou \"disable 2\"\n");
    }
    }
    else if (cmd.startsWith("auto"))
    {
    int num=1;
    if (sscanf(cmd.c_str(),"auto %d",&num)==1)
    {
      m_pJardCmd->setAuto(num,true);
      m_pSerial->println("[OK]");
    }
    else
    {
      m_pSerial->println("[ERREUR] - Commande incorrecte. Exemple qui marche = \"auto 1\" ou \"auto 2\"\n");
    }
    }
    else if (cmd.startsWith("unauto"))
    {
    int num=1;
    if (sscanf(cmd.c_str(),"unauto %d",&num)==1)
    {
      m_pJardCmd->setAuto(num,false);
      m_pSerial->println("[OK]");
    }
    else
    {
      m_pSerial->println("[ERREUR] - Commande incorrecte. Exemple qui marche = \"unauto 1\" ou \"unauto 2\"\n");
    }
    }
    else if (cmd.startsWith("force"))
    {
    int num=1;
    if (sscanf(cmd.c_str(),"force %d",&num)==1)
    {
      m_pJardCmd->forcePompe(num,true);
      m_pSerial->println("[OK]");
    }
    else
    {
      m_pSerial->println("[ERREUR] - Commande incorrecte. Exemple qui marche = \"force 1\" ou \"force 2\"\n");
    }
    }
    else if (cmd.startsWith("unforce"))
    {
    int num=1;
    if (sscanf(cmd.c_str(),"unforce %d",&num)==1)
    {
      m_pJardCmd->forcePompe(num,false);
      m_pSerial->println("[OK]");
    }
    else
    {
      m_pSerial->println("[ERREUR] - Commande incorrecte. Exemple qui marche = \"unforce 1\" ou \"unforce 2\"\n");
    }
    }
    else if (cmd.equals("values"))
    {
    m_pSerial->println("Values:");
    m_pSerial->print("  - Batt :");
    m_pSerial->println(m_pJardCmd->getBattLevel());
    m_pSerial->print("  - Sun  :");
    m_pSerial->println(m_pJardCmd->getSunLevel());
    m_pSerial->print("  - Pmp1 :");
    m_pSerial->println(mbs_outputs.get(OB_CMD_PMP1));
    m_pSerial->print("  - Pmp2 :");
    m_pSerial->println(mbs_outputs.get(OB_CMD_PMP2));
    m_pSerial->print("  - Temp :");
    m_pSerial->println(m_pJardCmd->getTemp());
    m_pSerial->print(" °C");
    m_pSerial->print("  - Hum :");
    m_pSerial->println(m_pJardCmd->getHum());
    m_pSerial->print(" %");
    }
    else if (cmd.equals("info"))
    {
    T_ID id;
    if (memoire_load_id(&id)==true)
    {
      m_pSerial->print("Version: ");
      m_pSerial->println(id.bVersion);
      m_pSerial->print("Serial: ");
      m_pSerial->println(id.bSerial);
    }
    else
    {
      m_pSerial->print("ERREUR!");
    }
    }
    else if (cmd.equals("settings"))
    {
    unsigned char ucVal;

    m_pSerial->println("");

    m_pSerial->println("PMP1:");
    if (memoire_read(MEM_SETTINGS_ADDR_PUMP1_EN,&ucVal)==true)
    {
      m_pSerial->print("  - enabled: ");
      m_pSerial->println(ucVal);
    }
    if (memoire_read(MEM_SETTINGS_ADDR_PUMP1_AUTO,&ucVal)==true)
    {
      m_pSerial->print("  - auto: ");
      m_pSerial->println(ucVal);
    }

    m_pSerial->println("PMP2:");
    if (memoire_read(MEM_SETTINGS_ADDR_PUMP2_EN,&ucVal)==true)
    {
      m_pSerial->print("  - enabled: ");
      m_pSerial->println(ucVal);
    }
    if (memoire_read(MEM_SETTINGS_ADDR_PUMP2_AUTO,&ucVal)==true)
    {
      m_pSerial->print("  - auto: ");
      m_pSerial->println(ucVal);
    }

    m_pSerial->println("");
    }
    else if (cmd.equals("help"))
    {
    m_pSerial->println("Aide:");
    m_pSerial->println("  - date");
    m_pSerial->println("  - setdate <DD/MM/YYYY>");
    m_pSerial->println("  - settime <HH:MM:SS>");
    m_pSerial->println("  - enable");
    m_pSerial->println("  - disable");
    m_pSerial->println("  - auto");
    m_pSerial->println("  - unauto");
    m_pSerial->println("  - force");
    m_pSerial->println("  - unforce");
    m_pSerial->println("  - values");
    m_pSerial->println("  - info");
    m_pSerial->println("  - settings");
    m_pSerial->println("  - help");
    }*/
}
