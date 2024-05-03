
#include "client.h"
#include "globals.h"
#include "analog.hpp" 
#include "flow.hpp" 
#include "eep_slave.hpp" 
#include "slavearduino.hpp"

#include <DHT.h>

//#define FORCE_INIT
#define DEBUG_TRACE

#define INIT_AND_SET_ADDR
#ifdef INIT_AND_SET_ADDR
  #define INIT_ADDR (1)
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

//#define DELTA_ALIVE_MS    (4000UL)

bool g_on=false;
bool g_cpt_low=false;
bool g_cpt_high=false;
unsigned short g_mes_cv;

int g_flow_mLpMin=-1;
char g_temp_dg=0;
char g_hum_pc=0;
unsigned short g_total_s=0;
unsigned short g_errors=0;

Analog anMesV;
DHT dht(PIN_DHT22, DHT22);


/*bool set_slave_addr(unsigned char bNewAddr)
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
}*/

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

  pinMode(LED_BUILTIN,OUTPUT);
  digitalWrite(LED_BUILTIN,LOW);

  pinMode(PIN_TX_EN,OUTPUT);
  digitalWrite(PIN_TX_EN,LOW);

  bool force_init=false;
  #ifdef FORCE_INIT
    force_init=true;
  #endif

  /*if ( (Eep::readID(&g_bAddr,&g_bFct)==false) || (force_init==true) )
  {
    g_bAddr=INIT_ADDR;
    g_bFct=INIT_FCT;
   *g_enabled=true;
    g_pump_s=0;

    Eep::writeID(g_bAddr,g_bFct);
    Eep::writeEnabled(g_enabled);
    Eep::writeCounter(g_pump_s);
  }
  else
  {
    g_enabled=Eep::readEnabled();
    g_pump_s=Eep::readCounter();
  }*/

  g_cpt_low=false;
  g_cpt_high=false;
  g_on=false;
  g_flow_mLpMin=-1;
  
  g_total_s=0;
  g_errors=0;


  /*#ifdef DEBUG_TRACE
    Serial.begin(9600);
    Serial.print("BOOT:");
    char str[5]="";
    sprintf(str,"%c",g_bAddr);
    Serial.print(str);
    Serial.print(" ");
    sprintf(str,"%c",g_bFct);
    Serial.print(str);
    Serial.print(" ");
    Serial.println(g_enabled==true?"EN":"X");
  #endif*/
  
  #ifdef FORCE_INIT
    Serial.println("FORCE INIT!");
  #endif

  dht.begin();
  Flow.begin(PIN_CPT_FLOW);

  unsigned char addr=getSlaveAddress();
  if (addr==15)
  {
	/// Lire EEPROM
  }

  Slave.begin(PIN_TX_EN,addr);

}

void serialEvent()
{
  Slave.onSerialEvent();
}

void loop() 
{
  #ifdef FORCE_INIT
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

	   g_mes_cv=anMesV.get();
  }

  if (!Slave.isAlive())
  {
	g_on=false;
  }
  else
  {
	g_on=Slave.isCmd();
  }


  digitalWrite(PIN_CMD_EV,g_on==true?HIGH:LOW);
  digitalWrite(LED_BUILTIN,Slave.isAlive()?HIGH:LOW);

  Flow.tick();
}

/*void reset_stats(void)
{
  g_pump_s=0;
  Eep::writeCounter(g_pump_s);
}*/

