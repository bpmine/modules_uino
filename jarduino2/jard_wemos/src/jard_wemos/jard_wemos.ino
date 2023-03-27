#include <ESP8266WiFi.h>
#include <ModbusIP_ESP8266.h>
//#include <EEPROM.h>

#include "timer.h"

#define NODE_POMPE_MAIN
//#define NODE_LVL_PAUL


#define PIN_IN_N1     14
#define PIN_IN_N2     12
#define PIN_IN_N3     13
#define PIN_OUT_CMD   15
#define PIN_LED       2

char ssid[] = "Domotique";
char password[] = "94582604";

ModbusIP mb;

#define COIL_HEART    1

#define COIL_CMD_PMP  2

#define STS_LVL1      1
#define STS_LVL2      2
#define STS_LVL3      3

#define IREG_RSSI     1

bool g_comm_ok=false;
Timer tmrComm(2000,false);

#ifdef NODE_POMPE_MAIN
  IPAddress local_IP(192, 168, 3, 201);
#endif
#ifdef NODE_LVL_PAUL
  IPAddress local_IP(192, 168, 3, 202);
#endif
  
IPAddress gateway(192, 168, 3, 1);
IPAddress subnet(255, 255, 255, 0);

void setup() 
{
  //EEPROM.begin(512);
  //EEPROM.write(0,456);
  //EEPROM.commit();
  //int rd=EEPROM.read(0);
  
  pinMode(PIN_OUT_CMD,OUTPUT);
  digitalWrite(PIN_OUT_CMD,LOW);
  pinMode(PIN_LED,OUTPUT);
  digitalWrite(PIN_LED,LOW);

  pinMode(PIN_IN_N1,INPUT_PULLUP);
  pinMode(PIN_IN_N2,INPUT_PULLUP);
  pinMode(PIN_IN_N3,INPUT_PULLUP);
  
  Serial.begin(115200);
  delay(50);
  Serial.println('Boot');
  Serial.setDebugOutput(true);

  if (!WiFi.config(local_IP, gateway, subnet)) 
  {
    Serial.println("STA Failed to configure");
  }  

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid,password);
  WiFi.setAutoReconnect(true);

  int status=WiFi.waitForConnectResult();
  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("__________________");
    Serial.println("WiFi connected");  
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());    
    Serial.println("__________________");
  }
  else
  {
      Serial.print("Error statut ");
      Serial.println(WiFi.status());    
  }

  mb.server();

  g_comm_ok=false;
  mb.addCoil(COIL_HEART,false);
  mb.addCoil(COIL_CMD_PMP,false);
  
  mb.addIsts(STS_LVL1,false);
  mb.addIsts(STS_LVL2,false);
  mb.addIsts(STS_LVL3,false);

  mb.addIreg(IREG_RSSI,WiFi.RSSI());
  
  tmrComm.start();
}
 
void loop() 
{  
   mb.task();   

   if (digitalRead(PIN_IN_N1)==LOW)
      mb.Ists(STS_LVL1,true);
   else
      mb.Ists(STS_LVL1,false);

   if (digitalRead(PIN_IN_N2)==LOW)
      mb.Ists(STS_LVL2,true);
   else
      mb.Ists(STS_LVL2,false);

      if (digitalRead(PIN_IN_N3)==LOW)
      mb.Ists(STS_LVL3,true);
   else
      mb.Ists(STS_LVL3,false);

   if (mb.Coil(COIL_HEART)==true)
   {
      g_comm_ok=true;
      tmrComm.start();
      mb.Coil(COIL_HEART,false);
   }

   mb.Ireg(IREG_RSSI,WiFi.RSSI());
   
   if (tmrComm.tick()==true)
   {
      g_comm_ok=false;            
   }

   if (g_comm_ok==false)
   {
    mb.Coil(COIL_CMD_PMP,false);
   }

   if ( mb.Coil(COIL_CMD_PMP) )
   {
    digitalWrite(PIN_OUT_CMD,HIGH);
    digitalWrite(PIN_LED,LOW);
   }
   else
   {
    digitalWrite(PIN_OUT_CMD,LOW);
    digitalWrite(PIN_LED,HIGH);
   }
}
