#include <DHT.h>

#include "flow.hpp" 

#define PIN_CPT_LOW       (2)
#define PIN_CPT_HIGH      (3)
#define PIN_CPT_FLOW      (4)
#define PIN_TX_EN         (6)
#define PIN_CMD_EV        (7)
#define PIN_LED           (8)
#define PIN_DHT22         (10)

#define PIN_MES_V         (A1)
#define PIN_ADDR_A1       (A4)
#define PIN_ADDR_A2       (A2)
#define PIN_ADDR_A3       (A5)
#define PIN_ADDR_A4       (A3)

#define DELTA_ALIVE_MS    (4000UL)

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

void setup() 
{
  Serial.begin(9600);
  
  pinMode(PIN_CMD_EV,OUTPUT);
  digitalWrite(PIN_CMD_EV,LOW);
  pinMode(PIN_LED,OUTPUT);
  digitalWrite(PIN_LED,LOW);
  
  pinMode(PIN_CPT_LOW,INPUT_PULLUP);
  pinMode(PIN_CPT_HIGH,INPUT_PULLUP);
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

  dht.begin();
  Flow.begin(PIN_CPT_FLOW);
}

void serialEvent()
{  
}

int g_flow_mLpMin=0;
bool g_cpt_low=false;
bool g_cpt_high=false;
int valV=0;
int g_temp=0;
int g_hum=0;

void loop() 
{
  //g_flow_mLpMin=Flow.getFlow();
  /*g_cpt_low=digitalRead(PIN_CPT_LOW)==HIGH?false:true;
  g_cpt_high=digitalRead(PIN_CPT_HIGH)==HIGH?false:true;
  valV=analogRead(PIN_MES_V);*/

  Serial.println("____________________");

   float tmp = dht.readHumidity(); 
   if (isnan(tmp))
     g_hum=-1;
   else
     g_hum=trunc(tmp);

   tmp = dht.readTemperature(); 
   if (isnan(tmp))
     g_temp=-127;
   else
     g_temp=trunc(tmp);

   Serial.print("Temperature : ");
   Serial.print(g_temp);
   Serial.println("°C");
   
   Serial.print("Humidite    : ");
   Serial.print(g_hum);
   Serial.println("%");
   

   unsigned char addr=0;
   if (digitalRead(PIN_ADDR_A1)==LOW)
    addr|=0x01;
   if (digitalRead(PIN_ADDR_A2)==LOW)
    addr|=0x02;
   if (digitalRead(PIN_ADDR_A3)==LOW)
    addr|=0x04;
   if (digitalRead(PIN_ADDR_A4)==LOW)
    addr|=0x08;
   
   Serial.print("Adresse     : ");
   Serial.println(addr);   

   delay(100);

   valV=analogRead(PIN_MES_V);
   Serial.print("Tension     : ");
   Serial.print(valV);   
   Serial.println("");

   delay(1000);
    
}
