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

extern "C"{
#include <mdbus.h>
}

void user_mdbus_send(void *back,unsigned char* pbuff, int sz)
{
  if (back!=NULL)
  {
    HardwareSerial *pSerial=(HardwareSerial*)back;
    pSerial->write(pbuff,sz);
  }
}

int user_mdbus_read_coils(unsigned short addr, unsigned short count, unsigned char* o_pBuffer)
{
  for (int i=0;i<count;i++)
  {
    unsigned char ucVal;
    switch (addr+i)
    {
      case 0:ucVal=mbs.get(MB_PMP1_FORCED);break;
      case 1:ucVal=mbs.get(MB_PMP1_ENABLE);break;
      case 2:ucVal=mbs.get(MB_PMP1_AUTO);break;

      case 10:ucVal=mbs.get(MB_PMP2_FORCED);break;
      case 11:ucVal=mbs.get(MB_PMP2_ENABLE);break;
      case 12:ucVal=mbs.get(MB_PMP2_AUTO);break;

      default:return MDBUS_ERR;
    }
          
    mdbus_fill_coil_data(o_pBuffer, i, ucVal);
  }
  
  return MDBUS_OK; 
}

int user_mdbus_write_coils(unsigned short addr, unsigned short count, unsigned char* i_pBuffer)
{
  for (int i = 0; i < count; i++)
  {
    unsigned char val = mdbus_get_coil_data(i_pBuffer, i);
    switch (addr+i)
    {
      case 0:mbs.fromBool(MB_PMP1_FORCED,val);break;
      case 1:mbs.fromBool(MB_PMP1_ENABLE,val);break;
      case 2:mbs.fromBool(MB_PMP1_AUTO,val);break;

      case 10:mbs.fromBool(MB_PMP2_FORCED,val);break;
      case 11:mbs.fromBool(MB_PMP2_ENABLE,val);break;
      case 12:mbs.fromBool(MB_PMP2_AUTO,val);break;
      default:return MDBUS_ERR;
    }
  }

  return MDBUS_OK;  
}

int user_mdbus_read_inputs(unsigned short addr, unsigned short count, unsigned char* o_pBuffer)
{
    return MDBUS_ERR;
}

int user_mdbus_read_input_registers(unsigned short addr, unsigned short count, unsigned char* o_pBuffer)
{
    for (int i = 0; i < count; i++)
    {
      unsigned short usVal;
      switch (addr+i)
      {
        case 0:usVal=cmds.m_pJardCmd->getBattLevel();break;
        case 1:usVal=cmds.m_pJardCmd->getSunLevel();break;
        case 2:usVal=cmds.m_pJardCmd->getTemp();break;
        case 3:usVal=cmds.m_pJardCmd->getHum();break;
        
        default:return MDBUS_ERR;
      }
      mdbus_fill_register_data(o_pBuffer, i, usVal);
    }

    return MDBUS_OK;
}

int user_mdbus_read_holding_registers(unsigned short addr, unsigned short count, unsigned char* o_pBuffer)
{
    for (int i = 0; i < count; i++)
    {
      unsigned short usVal;
      switch (addr+i)
      {
        case 0:usVal=1978;break;
        case 1:usVal=6;break;
        case 2:usVal=26;break;
        case 3:usVal=9;break;
        case 4:usVal=15;break;
        case 5:usVal=0;break;
        
        default:return MDBUS_ERR;

      }
      mdbus_fill_register_data(o_pBuffer, i, usVal);
    }

    return MDBUS_OK;
}

int user_mdbus_write_holding_registers(unsigned short addr, unsigned short count, unsigned char* i_pBuffer)
{
    for (int i = 0; i < count; i++)
    {
        unsigned short usVal = mdbus_get_register_data(i_pBuffer, i);
        switch (addr+i)
        {
          case 0:usVal=1978;break;
          case 1:usVal=6;break;
          case 2:usVal=26;break;
          case 3:usVal=9;break;
          case 4:usVal=15;break;
          case 5:usVal=0;break;
          
          default:return MDBUS_ERR;
        }
    }

    return MDBUS_OK;
}

T_MDBUS_CTX ctx;
unsigned char md_buffer[150];
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

  m_pSerial->begin(9600);
  //m_pSerial->println("Boot...");

  mdbus_init(&ctx,md_buffer, sizeof(md_buffer),4);  
  ctx.back=m_pSerial;
}

void Cmds::onSerialEvent()
{
  if (m_pSerial == NULL)
    return;

  while (m_pSerial->available() > 0) 
  {
    int incomingByte = m_pSerial->read();
    if ( (incomingByte>=0) && (incomingByte<=255) )
      mdbus_receive(&ctx, (unsigned char)incomingByte);
  } 
}
