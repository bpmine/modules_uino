#include <DHT.h>

#define VERY_SIMPLE_TEST

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
  Flow.begin();
  delay(100);
}

void serialEvent()
{  
}

int g_flow_LpH=0;
bool g_cpt_low=false;
bool g_cpt_high=false;
int g_temp=0;
int g_hum=0;

int cnt=0;
void loop() 
{    
  #ifdef VERY_SIMPLE_TEST
    g_cpt_low=digitalRead(PIN_CPT_LOW)==HIGH?false:true;
    g_cpt_high=digitalRead(PIN_CPT_HIGH)==HIGH?false:true;


   bool on=false;
   if ( (g_cpt_high==true) && (g_cpt_low==true) )
   {
    digitalWrite(LED_BUILTIN,HIGH);
    on=true;
   }
   else if ( (g_cpt_high==true) && (g_cpt_low==false) )
   {
    digitalWrite(LED_BUILTIN,!digitalRead(LED_BUILTIN));
    on=true;
   }
   else if ( (g_cpt_high==false) && (g_cpt_low==false) )
   {
    digitalWrite(LED_BUILTIN,LOW);
    on=false;
   }
   else
   {
    on=false;
    digitalWrite(LED_BUILTIN,LOW);
   }
   
    digitalWrite(PIN_CMD_EV,on? HIGH : LOW );

    cnt++;
    if (cnt>15)
    {
      Serial.println("_______________");
      Serial.print("Low         : ");
      Serial.println(g_cpt_low);   
      Serial.print("High        : ");
      Serial.println(g_cpt_high);  
      Serial.print("ON          : ");       
      Serial.println(on);
      cnt=0;
    }    

    delay(200);
    
    return;
  #endif
  
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

  Flow.start();
  for (int i=0;i<2500;i++)
  {
    Flow.tick();
    delay(1);
  } 

  Serial.println("____________________");
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

  for (int i=0;i<100;i++)
  {
    Flow.tick();
    delay(1);
  }
  
   unsigned long v=analogRead(PIN_MES_V);
   v=v*1200/669;
   Serial.print("Tension     : ");
   Serial.print((float)((float)v/100.0));
   Serial.println("V");

   g_flow_LpH=Flow.getFlow();
   Serial.print("Flow        : ");
   Serial.print(g_flow_LpH);   
   Serial.println("L/H");   
   
   g_cpt_low=digitalRead(PIN_CPT_LOW)==HIGH?false:true;
   Serial.print("Low         : ");
   Serial.println(g_cpt_low);   
   g_cpt_high=digitalRead(PIN_CPT_HIGH)==HIGH?false:true;
   Serial.print("High        : ");
   Serial.println(g_cpt_high);   

   digitalWrite(PIN_CMD_EV,!digitalRead(PIN_CMD_EV));

}
