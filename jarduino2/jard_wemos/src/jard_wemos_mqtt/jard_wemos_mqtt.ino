#include <ESP8266WiFi.h>
#include <EspMQTTClient.h>

#include "timer.h"

//#define NODE_MAIN
//#define NODE_PAUL
//#define NODE_REDUIT
//#define NODE_BARBEC
#define NODE_TEST

#define PIN_IN_N1       14
#define PIN_IN_N2       12
#define PIN_IN_N3       13
#define PIN_OUT_CMD     15
#define PIN_LED         2
#define PIN_ANALOG_POW  A0

//#define MQTT_TRACE_ON
  
#define TOPIC_PREFIX  "/wifiio"
#define TOPIC_CMD     "cmd"
#define TOPIC_SYNC    "sync"
#define TOPIC_DATA    "data"
#define TOPIC_LOG     "log"
  
/*#define DELAY_TASK_REPORT_COMM  (3600*1000UL)
#define DELAY_TASK_REPORT_DATA  (5*60*1000UL)
#define DELAY_TASK_MQTT_OK      (800UL)*/

bool g_comm_ok=false;
int g_pwr_value=0;
bool g_lvl_1=false;
bool g_lvl_2=false;
bool g_lvl_3=false;
int g_rssi=0;
bool g_cmd_pump=false;

Timer tmrComm(3000,false);
Timer tmrCycle(100,false);
Timer tmrBeforeSleep(60000UL);

#ifdef NODE_TEST
  char NAME[]="test";
#endif

char ssid[] = "Domotique";
char password[] = "94582604";

EspMQTTClient mqttClient(
  ssid,
  password,
  "192.168.3.200",
  "toto",
  "toto",
  NAME
);

void sendLog(char *strMsg)
{
  char strTopicLog[50];
  sprintf(strTopicLog,"%s/%s/%s",TOPIC_PREFIX,TOPIC_LOG,NAME);
  Serial.print("Log into ");
  Serial.println(strTopicLog);
  mqttClient.publish(strTopicLog, strMsg);
}

void addBool(char *strJson,char *strKey,bool val)
{
  strcat(strJson,"\"cmd\":");
  if (val==true)
    strcat(strJson,"true");
  else
    strcat(strJson,"false");  
}

void sendData(void)
{
  char strTopicLog[50];
  sprintf(strTopicLog,"%s/%s/%s",TOPIC_PREFIX,TOPIC_DATA,NAME);
  Serial.print("Data to ");
  Serial.println(strTopicLog);

  char strData[255]="";
  strcat(strData,"{");
  addBool(strData,"cmd",g_cmd_pump);
  strcat(strData,",");
  addBool(strData,"n1",g_lvl_1);
  strcat(strData,",");
  addBool(strData,"n2",g_lvl_2);
  strcat(strData,",");
  addBool(strData,"n3",g_lvl_3);
  strcat(strData,"}");
  
  mqttClient.publish(strTopicLog, strData);
}

void onReceiveCmd(const String &payload)
{
  Serial.print("Cmd: ");
  Serial.println(payload);
  
  if (payload=="on")  
    g_cmd_pump=true;
  else if (payload=="off")
    g_cmd_pump=false;

  g_comm_ok=true;
  tmrComm.start();    

  sendData();

  tmrBeforeSleep.start();
}
  
void onConnectionEstablished()
{
  char strTopicCmd[50];
  sprintf(strTopicCmd,"%s/%s",TOPIC_PREFIX,TOPIC_CMD);
  mqttClient.subscribe(strTopicCmd,onReceiveCmd);  
  
  sendLog("MQTT Connected");
  sendData();
}


void setup_mqtt(void)
{
  static char strName[30];
  sprintf(strName,"wifiio_%s",NAME);
  mqttClient.setMqttClientName(strName);
  
  mqttClient.enableMQTTPersistence();

  #ifdef MQTT_TRACE_ON
    mqttClient.enableDebuggingMessages();
  #endif
}


void setup() 
{
  pinMode(PIN_OUT_CMD,OUTPUT);
  digitalWrite(PIN_OUT_CMD,LOW);
  pinMode(PIN_LED,OUTPUT);
  digitalWrite(PIN_LED,LOW);

  pinMode(PIN_IN_N1,INPUT_PULLUP);
  pinMode(PIN_IN_N2,INPUT_PULLUP);
  pinMode(PIN_IN_N3,INPUT_PULLUP);

  pinMode(PIN_ANALOG_POW,INPUT);
  
  Serial.begin(115200);
  delay(50);
  Serial.println('Boot');
  Serial.setDebugOutput(true);

  setup_mqtt();
  
  g_comm_ok=false;  
  
  tmrComm.start();
  tmrCycle.start();
  tmrBeforeSleep.start();
}
 
void loop() 
{  
   if (tmrComm.tick()==true)
      g_comm_ok=false;            

   if (tmrCycle.tick())
   {
     g_pwr_value=analogRead(PIN_ANALOG_POW);     

     if (digitalRead(PIN_IN_N1)==LOW)
        g_lvl_1=true;
     else
        g_lvl_1=false;
  
     if (digitalRead(PIN_IN_N2)==LOW)
        g_lvl_2=true;
     else
        g_lvl_2=false;
  
     if (digitalRead(PIN_IN_N3)==LOW)
        g_lvl_3=true;
     else
        g_lvl_3=false;
    
     g_rssi=WiFi.RSSI();   

     if (g_comm_ok==false)
      g_cmd_pump=false;
  
     if ( g_cmd_pump==true )
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

   mqttClient.loop();

   if (tmrBeforeSleep.tick()==true)
   {
     ESP.deepSleep( sleepTimeS * 1000000, WAKE_RF_DISABLED );
   }
}
