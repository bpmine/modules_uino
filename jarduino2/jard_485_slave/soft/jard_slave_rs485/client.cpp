/**
 * @file client.cpp
 * @brief Created on: 26 novembre 2022
*/
#include "client.h"
#include "globals.h"
#include <arduino.h>

int _pinTxEn;


void client_init(unsigned char addr,int pinTxEn)
{  
  pinMode(pinTxEn,OUTPUT);
  digitalWrite(pinTxEn,LOW);
  _pinTxEn=pinTxEn;  
}


#define SOF (0x01)
#define EOF (0x02)

//uint8_t g_addr='A';

uint8_t pos=0;
uint8_t addr=0;
uint8_t fct=0;
uint8_t cmd=0;
uint8_t cs=0;
uint8_t cs_calc=0;


char tohexchar(uint8_t b)
{
  b=b&0xF;
  if ( (b>=0) && (b<=9) )
  {
    return '0'+b;
  }
  else if ( (b>=0x0A) && (b<=0x0F) )
  {
    return 'A'+(b-10);
  }
  else
  {
    return '0';
  }  
}

void _client_putchar(uint8_t b, uint8_t * cs)
{
  Serial.write(b);
  if (cs!=NULL)
    *cs=*cs+b;
}

void _client_puthex(uint8_t b, uint8_t * cs)
{
  _client_putchar(tohexchar(b),cs);
  _client_putchar(tohexchar(b>>4),cs);
}

void _client_exec_cmd(void)
{
  if (cmd==1)
    g_cmd_ev=true;
  else
    g_cmd_ev=false;
  
  uint8_t cs=0;  

  digitalWrite(_pinTxEn,HIGH);
  _client_putchar(SOF,NULL);
  _client_putchar(g_bAddr,&cs);
  
  if ( (g_bFct=='1') && (g_bFct=='1') )
  {
    _client_putchar('1',&cs);
    _client_puthex(g_cmd_ev,&cs);
  }
  else if ( (g_bFct=='2') && (g_bFct=='2') )
  {
    uint8_t stat=0;
    if (g_cpt_low==true)
      stat|=0x01;
    if (g_cpt_high==true)
      stat|=0x02;
      
    _client_putchar('2',&cs);
    _client_puthex(stat,&cs);    
  }
  
  _client_puthex(cs,NULL);
  _client_putchar(EOF,NULL); 
  while ((UCSR0A & _BV (TXC0)) == 0) {} 
  digitalWrite(_pinTxEn,LOW);
}


int fromhexchar(char c)
{  
  if ( (c>='0') && (c<='9') )
  {
    return c-'0';
  }
  else if ( (c>='A') && (c<='F') )
  {
    return (c-'A')+10;
  }
  else
  {
    return -1;
  }
}

void addLow(uint8_t *val,char c)
{  
  int res=fromhexchar(c);
  if (res!=-1)
  {
    *val=*val+res;
  }
}

void setHigh(uint8_t *val,char c)
{
  *val=0;
  int res=fromhexchar(c);
  if (res!=-1)
  {
    *val=*val+(res*16);
  }
}

void _client_recv(uint8_t b)
{
  if ( (pos==0) && (b!=SOF))
    return;

  if (b==SOF)
    pos=0;

  switch (pos)
  {
    case 0:
    {
      cs_calc=0;
      break;
    }
    case 1:
    {
      addr=b;
      cs_calc+=b;
      break;
    }    
    case 2:
    {
      fct=b;
      cs_calc+=b;
      break;
    }
    case 3:
    {
      setHigh(&cmd,b);
      cs_calc+=b;
      break;
    }
    case 4:
    {
      addLow(&cmd,b);
      cs_calc+=b;
      break;
    }
    case 5:
    {
      setHigh(&cs,b);
      break;
    }
    case 6:
    {
      addLow(&cs,b);
      break;
    }
    case 7:
    {
      if ( (b==EOF) && (cs==cs_calc) && (addr==g_bAddr) )
      {
        _client_exec_cmd();
        reset_comm_alive_timer();
      }
      else
      {
        /*Serial.print("Refusee: b=");
        Serial.print(b);
        Serial.print(" cs=");
        Serial.print(cs);
        Serial.print(" cs_calc=");
        Serial.print(cs_calc);
        Serial.print(" @ recue=");
        Serial.print(addr);
        Serial.print(" @=");
        Serial.print(g_bAddr);
        Serial.print(" cmd=");
        Serial.println(cmd);*/
      }
      
      pos=0;
      return;
    }
  }

  pos++;
}

void client_onSerialEvent(void)
{ 
  while (Serial.available()>0)
  {
    int b=Serial.read();
    if ( (b>0) && (b<255) )
      _client_recv((uint8_t)b);
  }
}
