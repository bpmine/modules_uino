/**
 * @file client.cpp
 * @brief Created on: 26 novembre 2022
*/

#include "client.h"
#include "globals.h"
#include <arduino.h>

extern "C"{
#include <mdbus.h>
}

int g_pinEnTx=0;

void user_mdbus_send(void *back,unsigned char* pbuff, int sz)
{
  if (back!=NULL)
  {
    HardwareSerial *pSerial=(HardwareSerial*)back;
    
    digitalWrite(g_pinEnTx,HIGH);
    pSerial->write(pbuff,sz);
    while ((UCSR0A & _BV (TXC0)) == 0) {}
    digitalWrite(g_pinEnTx,LOW);
  }
}

int user_mdbus_read_coils(unsigned short addr, unsigned short count, unsigned char* o_pBuffer)
{
  for (int i=0;i<count;i++)
  {
    unsigned char ucVal;
    switch (addr+i)
    {
      case 0:ucVal=(g_cmd_ev==true)?1:0;break;
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

      case 0:g_cmd_ev=(val==1)?true:false;resetWdg();break;
      
      default:return MDBUS_ERR;
    }
  }

  return MDBUS_OK;  
}

int user_mdbus_read_inputs(unsigned short addr, unsigned short count, unsigned char* o_pBuffer)
{
  for (int i=0;i<count;i++)
  {
    unsigned char ucVal;
    switch (addr+i)
    {
      case 0:ucVal=(g_cpt_low==true)?1:0;break;
      case 1:ucVal=(g_cpt_high==true)?1:0;break;

      default:return MDBUS_ERR;
    }
          
    mdbus_fill_coil_data(o_pBuffer, i, ucVal);
  }
  
  return MDBUS_OK; 
}

int user_mdbus_read_input_registers(unsigned short addr, unsigned short count, unsigned char* o_pBuffer)
{
  return MDBUS_ERR;
}

int user_mdbus_read_holding_registers(unsigned short addr, unsigned short count, unsigned char* o_pBuffer)
{
  return MDBUS_ERR;
}

int user_mdbus_write_holding_registers(unsigned short addr, unsigned short count, unsigned char* i_pBuffer)
{
  for (int i = 0; i < count; i++)
  {
    unsigned short usVal = mdbus_get_register_data(i_pBuffer, i);
    switch (addr+i)
    {
      case 0:
      {
        if (set_slave_addr((unsigned char)(usVal&0xFF))==true)
          return MDBUS_OK;
      }
    }
  }
  
  return MDBUS_ERR;
}

T_MDBUS_CTX ctx;
unsigned char md_buffer[150];

void client_init(void *pSerial,unsigned char addr,int pinTxEn)
{
  HardwareSerial *pSer=(HardwareSerial *)pSerial;
  if ( pSer == NULL )
    return;

  g_pinEnTx=pinTxEn;
  pinMode(g_pinEnTx,OUTPUT);
  digitalWrite(g_pinEnTx,LOW);

  pSer->begin(115200);

  mdbus_init(&ctx,md_buffer, sizeof(md_buffer),addr);
  ctx.back=pSerial;
}

void client_onSerialEvent(void *pSerial)
{
  HardwareSerial *pSer=(HardwareSerial *)pSerial;
  if (pSer == NULL)
    return;

  while (pSer->available() > 0) 
  {
    int incomingByte = pSer->read();
    if ( (incomingByte>=0) && (incomingByte<=255) )
      mdbus_receive(&ctx, (unsigned char)incomingByte);
  } 
}
