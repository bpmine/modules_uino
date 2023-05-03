#include <EEPROM.h>

#include "client.h"
#include "globals.h"
#include "analog.hpp" 

#define PIN_CPT_LV1       (2)
#define PIN_CPT_LV2       (3)
#define PIN_CPT_FLOW      (4)
#define PIN_TX_EN         (6)
#define PIN_CMD_EV        (7)
#define PIN_LED1          (8)
#define PIN_LED2          (9)
#define PIN_DHT20         (10)

#define PIN_MES_I         (A0)
#define PIN_MES_V         (A1)

#define DELTA_ALIVE_MS    (2000UL)

bool g_cmd_ev=false;
bool g_cpt_low=false;
bool g_cpt_high=false;
unsigned short g_mes_v;
unsigned short g_mes_i;
bool g_enabled=false;
unsigned short g_defaults=false;

unsigned char g_bAddr=0;
unsigned long g_tick0_ms;

Analog anMesI;
Analog anMesV;

void reset_comm_alive_timer(void)
{
  g_tick0_ms=millis();
}

bool isBusAlive(void)
{
  unsigned long t=millis();
  unsigned delta_ms=0;
  
  if (t>g_tick0_ms)
  {
    delta_ms=t-g_tick0_ms;
  }
  else
  {
    delta_ms=t-0xFFFFFFFF+g_tick0_ms;
  }

  if (delta_ms>DELTA_ALIVE_MS)
  {
    g_tick0_ms=millis()-DELTA_ALIVE_MS-10;
    return false;
  }
  else
    return true;
}

bool set_slave_addr(unsigned char bNewAddr)
{
  g_bAddr=bNewAddr;
  
  EEPROM.write(0,0xAA);
  EEPROM.write(1,bNewAddr);
  EEPROM.write(2,0x55);  

  return true;
}


void setup() 
{
  pinMode(PIN_CMD_EV,OUTPUT);
  digitalWrite(PIN_CMD_EV,LOW);
  pinMode(PIN_LED1,OUTPUT);
  digitalWrite(PIN_LED1,LOW);
  pinMode(PIN_LED2,OUTPUT);
  digitalWrite(PIN_LED1,LOW);
  pinMode(PIN_DHT20,OUTPUT);

  pinMode(PIN_CPT_LV1,INPUT);
  pinMode(PIN_CPT_LV2,INPUT);
  pinMode(PIN_CPT_FLOW,INPUT);

  pinMode(PIN_MES_I,INPUT);
  pinMode(PIN_MES_V,INPUT);

  unsigned char bMagic1=EEPROM.read(0);
  g_bAddr=EEPROM.read(1);
  unsigned char bMagic2=EEPROM.read(2);

  if ( (bMagic1!=0xAA) || (bMagic2!=0x55) )
  {
    EEPROM.write(0,0xAA);
    EEPROM.write(1,0);
    EEPROM.write(2,0x55);
    g_bAddr=0;
  }
  else
  {
    if (g_bAddr==0xFF)
    {
      g_bAddr=0;
      EEPROM.write(1,0);
    }    
  }  

  g_cpt_low=false;
  g_cpt_high=false;
  g_enabled=false;
  g_cmd_ev=false;
  g_defaults=0;
  
  g_tick0_ms=millis()-DELTA_ALIVE_MS-10;
  client_init(&Serial,g_bAddr,PIN_TX_EN);  
}

void serialEvent()
{
  client_onSerialEvent(&Serial);
}

void loop() 
{
  bool flgEv;
  bool flgBusAlive;
  
  g_cpt_low=digitalRead(PIN_CPT_LV1)==HIGH?false:true;
  g_cpt_high=digitalRead(PIN_CPT_LV2)==HIGH?false:true;
  anMesI.latch((unsigned short)analogRead(PIN_MES_I));
  anMesV.latch((unsigned short)analogRead(PIN_MES_V));
  flgBusAlive=isBusAlive();

  flgEv=flgBusAlive && g_cmd_ev && g_enabled;
  g_mes_v=anMesV.get();
  g_mes_i=anMesI.get();
  
  delay(1);
    
  digitalWrite(PIN_CMD_EV,flgEv==true?HIGH:LOW);
  
  if (flgBusAlive==false)
    g_cmd_ev=false;    
}
