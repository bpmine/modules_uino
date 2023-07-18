#include <ESP8266WiFi.h>
#include <EspMQTTClient.h>

#include "timer.h"

#define NODE_MAIN
//#define NODE_PAUL
//#define NODE_REDUIT
//#define NODE_BARBEC
//#define NODE_TEST

#define TIME_BEFORE_SLEEP_S   (3600)
#define TIME_BEFORE_FIRST_S   (120)
#define TIME_REPEAT_FIRST_S   (30)
#define TIME_CYCLE_MS         (100)
#define TIMEOUT_COMM_S        (5)
#define SLEEPING_TIME_MIN     (60)

#define WIFI_ID       "Domotique"
#define WIFI_PWD      "94582604"
#define MQTT_IP       "192.168.3.200"
#define MQTT_LOGIN    "toto"
#define MQTT_PWD      "toto"


/////////////////////////////////////////////////////////////////////
#define PIN_IN_N1       14
#define PIN_IN_N2       12
#define PIN_IN_N3       13
#define PIN_OUT_CMD     15
#define PIN_LED         2
#define PIN_ANALOG_POW  A0

//#define MQTT_TRACE_ON
  
#define TOPIC_PREFIX  "/wifiio"
#define TOPIC_CMD     "cmd"
#define TOPIC_DATA    "data"
#define TOPIC_LOG     "log"

bool g_comm_ok=false;
int g_pwr_value=0;
bool g_lvl_1=false;
bool g_lvl_2=false;
bool g_lvl_3=false;
int g_rssi=0;
bool g_cmd_pump=false;
unsigned long g_tick_s=0;

Timer tmrComm((unsigned long)TIMEOUT_COMM_S*1000UL,false);
Timer tmrCycle((unsigned long)TIME_CYCLE_MS,false);
Timer tmrBeforeSleep((unsigned long)TIME_BEFORE_SLEEP_S*1000UL);
Timer tmrGotoSleep(5000UL);
Timer tmrTick1S(1000UL,false);

Timer tmrBeforeFirst((unsigned long)TIME_BEFORE_FIRST_S*1000UL);
Timer tmrRepeatFirst((unsigned long)TIME_REPEAT_FIRST_S*1000UL,false);

#ifdef NODE_TEST
  char NAME[]="test";
#endif
#ifdef NODE_MAIN
  char NAME[]="main";
#endif
#ifdef NODE_PAUL
  char NAME[]="paul";
#endif
#ifdef NODE_REDUIT
  char NAME[]="reduit";
#endif
#ifdef NODE_BARBEC
  char NAME[]="barbec";
#endif

char ssid[] = "Domotique";
char password[] = "94582604";

EspMQTTClient mqttClient(
  WIFI_ID,
  WIFI_PWD,
  MQTT_IP,
  MQTT_LOGIN,
  MQTT_PWD,
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
  strcat(strJson,"\"");
  strcat(strJson,strKey);
  strcat(strJson,"\":");
  if (val==true)
    strcat(strJson,"true");
  else
    strcat(strJson,"false");  
}

void addInt(char *strJson,char *strKey,int val)
{
  strcat(strJson,"\"");
  strcat(strJson,strKey);
  strcat(strJson,"\":");
  char tmp[15];
  sprintf(tmp,"%d",val);
  strcat(strJson,tmp);
}

void addULong(char *strJson,char *strKey,unsigned long val)
{
  strcat(strJson,"\"");
  strcat(strJson,strKey);
  strcat(strJson,"\":");
  char tmp[20];
  sprintf(tmp,"%ld",val);
  strcat(strJson,tmp);
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
  strcat(strData,",");
  addInt(strData,"rssi",g_rssi);
  strcat(strData,",");
  addInt(strData,"pwr",g_pwr_value);
  strcat(strData,",");
  addULong(strData,"tick",g_tick_s);
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
  else if (payload=="sleep")
  {
    g_cmd_pump=false;
    sendLog("Demande de faire dodo!");
    tmrGotoSleep.start();
  }
  else if (payload=="ping")
  {
    sendLog("pong");
  }

  sendData();
  
  g_comm_ok=true;
  tmrComm.start();
  tmrBeforeSleep.start();
  
  tmrBeforeFirst.stop();    ///< Plus dans le cas d'attente de la premiere reponse
  tmrRepeatFirst.stop();
}
  
void onConnectionEstablished()
{
  char strTopicCmd[50];
  sprintf(strTopicCmd,"%s/%s/%s",TOPIC_PREFIX,TOPIC_CMD,NAME);
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
  tmrBeforeFirst.start();
  tmrRepeatFirst.start();
  tmrTick1S.start();
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

     if (tmrGotoSleep.isRunning())
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

   if (tmrBeforeFirst.isRunning())
   {    
    if (tmrRepeatFirst.tick()==true)
      sendData();
   }
   
   if ( tmrBeforeFirst.tick()==true )
   {
     Serial.println("Pas de reponse...");
     sendLog("No answer means sleep...");
     tmrGotoSleep.start();
   }

   if ( tmrBeforeSleep.tick()==true )
   {
     Serial.println("Sleep...");
     sendLog("Now time to sleep...");
     tmrGotoSleep.start();
   }

   if (tmrGotoSleep.tick()==true)
   {
    ESP.deepSleep( (unsigned long)SLEEPING_TIME_MIN * 60UL * 1000000UL );
   }

   if (tmrTick1S.tick()==true)
    g_tick_s++;
}
