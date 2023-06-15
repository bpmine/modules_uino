/**
 * @file client.cpp
 * @brief Created on: 26 novembre 2022
*/
#include "client.h"
#include "globals.h"
#include <arduino.h>

#include "prot.h"

int _pinTxEn;
uint32_t _nb_err_frame=0;
uint32_t _nb_err_cs=0;

void client_init(unsigned char addr,int pinTxEn)
{  
  pinMode(pinTxEn,OUTPUT);
  digitalWrite(pinTxEn,LOW);
  _pinTxEn=pinTxEn;  
}


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
  _client_putchar(tohexchar(b>>4),cs);
  _client_putchar(tohexchar(b),cs);  
}

void _client_puthex_u32(uint32_t ul, uint8_t * cs)
{
    uint8_t a=(ul>>24)&0xFF;
    uint8_t b=(ul>>16)&0xFF;
    uint8_t c=(ul>>8)&0xFF;
    uint8_t d=ul&0xFF;

    _client_puthex(a,cs);     
    _client_puthex(b,cs);     
    _client_puthex(c,cs);     
    _client_puthex(d,cs);     
}


void _client_exec_cmd(void)
{
  // Trame broadcast de fonction 'A' pour "Alive". Contient une commande d'arrêt d'urgence
  if (addr=='*')
  {
    if (fct=='A')
    {
      // En cas d'arret d'urgence, on desactive le noeud et on arrete la commande
      if (cmd==0xAC)
      {
        g_cmd_ev=false;
        set_enabled(false);
      }
    }
    return;
  }

  // Application de la commande si pompe fct 1 ou oya fct 2
  if ( ( (g_bFct=='1') && (fct=='1') ) || ( (g_bFct=='2') && (fct=='2') ) )
  {
    if (cmd==1)
      g_cmd_ev=true;
    else
      g_cmd_ev=false;
  }
  else if (fct=='@')
  {
    set_slave_addr(cmd);
  }
  else if (fct=='f')
  {
    set_slave_function(cmd);
  }
  else if (fct=='e')
  {
    set_enabled(cmd==1?true:false);
  }
  
  uint8_t cs=0;  

  // Tempo pour laisser le bus se retourner
  delay(10);
  
  digitalWrite(_pinTxEn,HIGH);
  _client_putchar(SOH,NULL);
  _client_putchar(g_bAddr,&cs);
  
  if ( (g_bFct==FCT_PUMP) && (fct==FCT_PUMP) )
  {
    uint8_t stat=0;
    if (g_cmd_ev==true)
      stat|=ST_CMD_ON;
    if (g_enabled==true)
      stat|=ST_ENABLED;

    int val=g_flow_mLpMin;
    
    _client_putchar(FCT_PUMP,&cs);        
    _client_puthex(stat,&cs);    
    _client_puthex(((val>>8)&0xFF),&cs);
    _client_puthex((val&0xFF),&cs);
    _client_puthex(g_temp,&cs);
    _client_puthex(g_hum,&cs);
  }
  else if ( (g_bFct==FCT_OYA) && (fct==FCT_OYA) )
  {
    uint8_t stat=0;
    if (g_cpt_low==true)
      stat|=ST_LVL_LOW;
    if (g_cpt_high==true)
      stat|=ST_LVL_HIGH;
    if (g_cmd_ev==true)
      stat|=ST_CMD_ON;
    if (g_enabled==true)
      stat|=ST_ENABLED;
      
    _client_putchar(FCT_OYA,&cs);
    _client_puthex(stat,&cs);    
    _client_puthex(g_temp,&cs);
    _client_puthex(g_hum,&cs);
  }
  else if ( fct==FCT_ADDR )
  {
    _client_putchar(FCT_ADDR,&cs);
    _client_puthex(g_bAddr,&cs);   
  }
  else if ( fct=='f' )
  {
    _client_putchar('f',&cs);
    _client_puthex(g_bFct,&cs);   
  }
  else if (fct==FCT_ENABLE)
  {
    _client_putchar(FCT_ENABLE,&cs);
    _client_puthex(g_enabled==true?1:0,&cs);   
  }
  else if (fct==FCT_READ_TIME)
  {
    _client_putchar(FCT_READ_TIME,&cs);
    _client_puthex_u32(g_pump_s,&cs);
  }
  else if (fct==FCT_RAZ_TIME)
  {
    reset_stats();
    _client_putchar(FCT_RAZ_TIME,&cs);
    _client_puthex_u32(0,&cs);
  }
  else if (fct==FCT_READ_ERR)
  {
    _client_putchar(FCT_READ_ERR,&cs);
    _client_puthex_u32(_nb_err_cs,&cs);
    _client_puthex_u32(_nb_err_frame,&cs);
  }
  else if (fct==FCT_RAZ_ERR)
  {
    _nb_err_frame=0;
    _nb_err_cs=0;
    _client_putchar(FCT_RAZ_ERR,&cs);
    _client_puthex_u32(0,&cs);
    _client_puthex_u32(0,&cs);
  }
  else if (fct==FCT_PING)
  {
    _client_putchar(FCT_PING,&cs);
    _client_puthex(cmd,&cs);
  }
  else
  {
    _client_putchar('-',&cs);
  }
  
  _client_puthex(cs,NULL);
  _client_putchar(STX,NULL); 
  while ((UCSR0A & _BV (TXC0)) == 0) {} 
  delay(2); // Au cas où
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
  if ( (pos==0) && (b!=SOH))
    return;

  if (b==SOH)
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
      if ( (b==STX) && (cs==cs_calc) && ( (addr==g_bAddr) || (addr=='*') ) )
      {
        digitalWrite(LED_BUILTIN,HIGH);
        _client_exec_cmd();
        reset_comm_alive_timer();
        digitalWrite(LED_BUILTIN,LOW);
      }
      else
      {
        if (cs!=cs_calc)
          _nb_err_cs++;
        if (b!=STX)
          _nb_err_frame++;
          
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
