#include <EEPROM.h>

#include "client.h"
#include "globals.h"

#define PIN_CMD_EV        (2)
#define PIN_CPT_LV1       (3)
#define PIN_CPT_LV2       (4)
#define PIN_BUS_ALIVE     (6)

#define DELTA_ALIVE_MS    (2000UL)

extern bool g_cmd_ev=false;
extern bool g_cpt_low=false;
extern bool g_cpt_high=false;

unsigned char g_bAddr=0;
unsigned long g_tick0_ms;

void resetWdg(void)
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

  pinMode(PIN_BUS_ALIVE,OUTPUT);
  digitalWrite(PIN_BUS_ALIVE,LOW);

  pinMode(PIN_CPT_LV1,INPUT_PULLUP);
  pinMode(PIN_CPT_LV2,INPUT_PULLUP);  

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

  g_tick0_ms=millis()-DELTA_ALIVE_MS-10;

  client_init(&Serial,g_bAddr);  
}

void serialEvent()
{
  client_onSerialEvent(&Serial);
}

void loop() 
{
  g_cpt_low=digitalRead(PIN_CPT_LV1)==HIGH?false:true;
  g_cpt_high=digitalRead(PIN_CPT_LV2)==HIGH?false:true;
  delay(10);
  digitalWrite(PIN_CMD_EV,g_cmd_ev==true?HIGH:LOW);
  
  if (isBusAlive()==false)
  {
    digitalWrite(PIN_BUS_ALIVE,LOW);
    g_cmd_ev=false;    
  }
  else
  {
    digitalWrite(PIN_BUS_ALIVE,HIGH);
  }
}
