
#include "client.h"
#include "globals.h"
#include "analog.hpp" 
#include "flow.hpp" 
#include "eep_slave.hpp" 
#include "slavearduino.hpp"
#include "timer.h"

#include <DHT.h>

//#define DEBUG_TRACE

//#define INIT_AND_SET_ADDR
#ifdef INIT_AND_SET_ADDR
  #define INIT_ADDR 'Z'
#endif

#define PIN_CPT_LVL_LOW   (2)
#define PIN_CPT_LVL_HIGH  (3)
#define PIN_CPT_FLOW      (4)
#define PIN_TX_EN         (6)
#define PIN_CMD_EV        (7)
#define PIN_LED1          (8)
#define PIN_DHT22         (10)

#define PIN_MES_V         (A1)
#define PIN_ADDR_A1       (A4)
#define PIN_ADDR_A2       (A2)
#define PIN_ADDR_A3       (A5)
#define PIN_ADDR_A4       (A3)

bool g_on=false;
bool g_cpt_low=false;
bool g_cpt_high=false;
unsigned char g_mes_cv;

int g_flow_mLpMin=-1;
char g_temp_dg=0;
char g_hum_pc=0;
unsigned short g_total_s=0;
unsigned short g_errors=0;

Analog anMesV;
DHT dht(PIN_DHT22, DHT22);
Timer tmrSec(1000,true);

unsigned char getSlaveAddress(void)
{
  unsigned char addr=0;
  if (digitalRead(PIN_ADDR_A1)==LOW)
   addr|=0x01;
  if (digitalRead(PIN_ADDR_A2)==LOW)
   addr|=0x02;
  if (digitalRead(PIN_ADDR_A3)==LOW)
   addr|=0x04;
  if (digitalRead(PIN_ADDR_A4)==LOW)
   addr|=0x08;

  return addr;
}

void setup() 
{
  pinMode(PIN_CMD_EV,OUTPUT);
  digitalWrite(PIN_CMD_EV,LOW);
  pinMode(PIN_LED1,OUTPUT);
  digitalWrite(PIN_LED1,LOW);
  //pinMode(PIN_DHT22,OUTPUT);

  pinMode(PIN_CPT_LVL_LOW,INPUT);
  pinMode(PIN_CPT_LVL_HIGH,INPUT);
  pinMode(PIN_CPT_FLOW,INPUT);

  pinMode(PIN_MES_V,INPUT);

  pinMode(PIN_ADDR_A1,INPUT_PULLUP);
  pinMode(PIN_ADDR_A2,INPUT_PULLUP);
  pinMode(PIN_ADDR_A3,INPUT_PULLUP);
  pinMode(PIN_ADDR_A4,INPUT_PULLUP);

  pinMode(LED_BUILTIN,OUTPUT);
  digitalWrite(LED_BUILTIN,LOW);

  pinMode(PIN_TX_EN,OUTPUT);
  digitalWrite(PIN_TX_EN,LOW);

  bool force_init=false;
  #ifdef FORCE_INIT
    force_init=true;
  #endif

  g_cpt_low=false;
  g_cpt_high=false;
  g_on=false;
  g_flow_mLpMin=-1;
  
  g_total_s=0;
  g_errors=0;

  dht.begin();
  Flow.begin(PIN_CPT_FLOW);

  #ifdef INIT_AND_SET_ADDR
    unsigned char tmp;
    Eep::readID(&tmp); /// Pour etre sûr EEP bien init
    Eep::writeID(INIT_ADDR);
  #endif

  unsigned char addr=getSlaveAddress();
  if (addr==15)
  {
    /// Si le selecteur d'adresse est sur 'F', on lit l'adresse en EEPROM
    Eep::readID(&addr);
  }

  Slave.begin(PIN_TX_EN,addr);
  g_total_s=Eep::readCounter();

  #ifdef DEBUG_TRACE
    Serial.print("Boot @");
    Serial.println(addr);
    for (int i=0;i<500;i++)
    {
      delay(1);
      yield();
    }
    
    Serial.print("Total: ");
    Serial.print(g_total_s);
    Serial.println(" s");
    for (int i=0;i<500;i++)
    {
      delay(1);
      yield();
    }
  #endif

  #ifdef INIT_AND_SET_ADDR
    Serial.print("FORCE ADDRESS INIT: ");
    Serial.println(INIT_ADDR);
    for (int i=0;i<500;i++)
    {
      delay(1);
      yield();
    }
  #endif 

  tmrSec.start();
}

void serialEvent()
{
  Slave.onSerialEvent();
}

void loop() 
{
  #ifdef INIT_AND_SET_ADDR
    digitalWrite(PIN_LED1,HIGH);
    digitalWrite(LED_BUILTIN,HIGH);
    
    return;
  #endif
  
  g_flow_mLpMin=Flow.getFlow();
  g_cpt_low=digitalRead(PIN_CPT_LVL_LOW)==HIGH?false:true;
  g_cpt_high=digitalRead(PIN_CPT_LVL_HIGH)==HIGH?false:true;
  anMesV.latch((unsigned short)analogRead(PIN_MES_V));

  if (Slave.loop()==true)
  {
	   float tmp = dht.readHumidity();
	   if ((isnan(tmp)) || (tmp<0) || (tmp>100) )
	     g_hum_pc=-1;
	   else
	     g_hum_pc=(char)trunc(tmp);

	   tmp = dht.readTemperature();
	   if ( (isnan(tmp)) || (tmp<-50) || (tmp>100) )
	     g_temp_dg=-127;
	   else
	     g_temp_dg=(char)trunc(tmp);

	   unsigned long v=(unsigned long)anMesV.get();    
     v=v*120/669;
     if (v>255)
      v=255;
     g_mes_cv=(unsigned char)v;
  }

  if (!Slave.isAlive())
  {
	  g_on=false;
  }
  else
  {
	  g_on=Slave.isCmd();
  }

  Flow.tick();
  
  if (tmrSec.tick()==true)
  {
    if (g_on==true)
    {
      g_total_s++;
      Eep::writeCounter(g_total_s);
    }
  }
  
  digitalWrite(PIN_CMD_EV,g_on==true?HIGH:LOW);
  digitalWrite(LED_BUILTIN,Slave.isAlive()?HIGH:LOW);
}

void reset_time(void)
{
  g_total_s=0;
  Eep::writeCounter(g_total_s);
}
