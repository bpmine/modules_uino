#include <DHT.h>

#include "client.h"
#include "globals.h"
#include "analog.hpp" 
#include "flow.hpp" 
#include "eep_slave.hpp" 

//#define FORCE_INIT

//#define INIT_PMP
#define INIT_OYA

#ifdef INIT_PMP
  #define INIT_ADDR 'A'
  #define INIT_FCT  '1'
#endif

#ifndef INIT_PMP
  #ifdef INIT_OYA
    #define INIT_ADDR 'Z'
    #define INIT_FCT  '2'    
  #endif
#endif

#define PIN_CPT_LV1       (2)
#define PIN_CPT_LV2       (3)
#define PIN_CPT_FLOW      (4)
#define PIN_TX_EN         (6)
#define PIN_CMD_EV        (7)
#define PIN_LED1          (8)
#define PIN_LED2          (9)
#define PIN_DHT22         (10)

#define PIN_MES_I         (A0)
#define PIN_MES_V         (A1)

#define DELTA_ALIVE_MS    (4000UL)

bool g_cmd_ev=false;
bool g_cpt_low=false;
bool g_cpt_high=false;
unsigned short g_mes_v;
unsigned short g_mes_i;
bool g_enabled=false;
unsigned short g_defaults=false;

unsigned char g_bAddr=0;
unsigned char g_bFct=0;
unsigned long g_comm_t0_ms;
unsigned long g_second_t0_ms;

int g_flow_mLpMin=-1;
uint8_t g_temp=0;
uint8_t g_hum=0;
unsigned long g_pump_s=0;

Analog anMesI;
Analog anMesV;

DHT dht(PIN_DHT22, DHT22);



unsigned long getTickCount_ms()
{
  return millis();
}

unsigned long getElapsedTime_ms(unsigned long t0_ms)
{
  unsigned long t=millis();
  if (t>t0_ms)
  {
    return t-t0_ms;
  }
  else
  {
    return 0xFFFFFFFF-t0_ms+t;
  }
}

void reset_comm_alive_timer(void)
{
  g_comm_t0_ms=getTickCount_ms();
}

bool isBusAlive(void)
{
  unsigned long t=millis();
  unsigned long delta_ms=getElapsedTime_ms(g_comm_t0_ms);

  if (delta_ms>DELTA_ALIVE_MS)
  {
    //g_tick0_ms=millis()+DELTA_ALIVE_MS+10UL;
    return false;
  }
  else
    return true;
}

bool set_slave_addr(unsigned char bNewAddr)
{
  uint8_t a;
  uint8_t b;
  
  g_bAddr=bNewAddr;

  if (Eep::readID(&a,&b)==false)
  {
    Eep::writeID(bNewAddr,g_bFct);
  }
  else
  {
    Eep::writeAddr(g_bAddr);
  }  

  return true;
}

bool set_slave_function(unsigned char bNewFunction)
{
  uint8_t a;
  uint8_t b;

  g_bFct=bNewFunction;
  
  if (Eep::readID(&a,&b)==false)
  {
    Eep::writeID(g_bAddr,g_bFct);
  }
  else
  {
    Eep::writeAddr(g_bAddr);
  }  

  return true;
}

void setup() 
{
  pinMode(PIN_CMD_EV,OUTPUT);
  digitalWrite(PIN_CMD_EV,LOW);
  pinMode(PIN_LED1,OUTPUT);
  digitalWrite(PIN_LED1,LOW);
  pinMode(PIN_LED2,OUTPUT);
  digitalWrite(PIN_LED2,LOW);
  //pinMode(PIN_DHT22,OUTPUT);

  pinMode(PIN_CPT_LV1,INPUT);
  pinMode(PIN_CPT_LV2,INPUT);
  pinMode(PIN_CPT_FLOW,INPUT);

  pinMode(PIN_MES_I,INPUT);
  pinMode(PIN_MES_V,INPUT);

  pinMode(LED_BUILTIN,OUTPUT);
  digitalWrite(LED_BUILTIN,LOW);

  bool force_init=false;
  #ifdef FORCE_INIT
    force_init=true;
  #endif

  if ( (Eep::readID(&g_bAddr,&g_bFct)==false) || (force_init==true) )
  {
    g_bAddr=INIT_ADDR;
    g_bFct=INIT_FCT;
    g_enabled=false;
    g_pump_s=0;

    Eep::writeID(g_bAddr,g_bFct);
    Eep::writeEnabled(g_enabled);
    Eep::writeCounter(g_pump_s);    
  }
  else
  {
    g_enabled=Eep::readEnabled();
    g_pump_s=Eep::readCounter();
  }

  g_cpt_low=false;
  g_cpt_high=false;
  g_enabled=false;
  g_cmd_ev=false;
  g_defaults=0;
  g_flow_mLpMin=-1;
  
  g_comm_t0_ms=millis()-DELTA_ALIVE_MS-10;
  g_second_t0_ms=getTickCount_ms();

  Serial.begin(9600);
  Serial.print("BOOT:");
  char str[5]="";
  sprintf(str,"%c",g_bAddr);
  Serial.print(str);
  Serial.print(" ");
  sprintf(str,"%c",g_bFct);
  Serial.println(str);

  #ifdef FORCE_INIT
  Serial.println("FORCE INIT!");
  #endif

  dht.begin();
  Flow.begin(PIN_CPT_FLOW);

  client_init(g_bAddr,PIN_TX_EN);
}

void serialEvent()
{
  client_onSerialEvent();
}

void loop() 
{
  #ifdef FORCE_INIT
    digitalWrite(PIN_LED1,HIGH);
    digitalWrite(PIN_LED2,HIGH);
    digitalWrite(LED_BUILTIN,HIGH);
    return;
  #endif
  
  bool flgEv;
  bool flgBusAlive;

  g_flow_mLpMin=Flow.getFlow();
  g_cpt_low=digitalRead(PIN_CPT_LV1)==HIGH?false:true;
  g_cpt_high=digitalRead(PIN_CPT_LV2)==HIGH?false:true;
  anMesI.latch((unsigned short)analogRead(PIN_MES_I));
  anMesV.latch((unsigned short)analogRead(PIN_MES_V));
  flgBusAlive=isBusAlive();

   float tmp = dht.readHumidity(); 
   if (isnan(tmp))
     g_hum=-1;
   else
     g_hum=(uint8_t)trunc(tmp);

   tmp = dht.readTemperature(); 
   if (isnan(tmp))
     g_temp=-127;
   else
     g_temp=(uint8_t)trunc(tmp);
    
  flgEv=flgBusAlive && g_cmd_ev && g_enabled;
  if (g_bAddr=='Z')
    flgEv=false;
    
  g_mes_v=anMesV.get();
  g_mes_i=anMesI.get();
  
  //delay(1);
    
  digitalWrite(PIN_CMD_EV,flgEv==true?HIGH:LOW);
  digitalWrite(LED_BUILTIN,flgBusAlive?HIGH:LOW);

  Flow.tick();

  if (getElapsedTime_ms(g_second_t0_ms)>=1000)
  {
    g_second_t0_ms=getTickCount_ms();

    if (flgEv==true)
    {
      g_pump_s++;
      Eep::writeCounter(g_pump_s);
    }
  }
  
  //if (flgBusAlive==false)
  //  g_cmd_ev=false;  
}

void reset_stats(void)
{
  g_pump_s=0;
  Eep::writeCounter(g_pump_s);
}

void set_enabled(bool enabled)
{
  g_enabled=enabled;
  Eep::writeEnabled(g_enabled);
}
