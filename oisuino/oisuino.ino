/**
 * @file oisuino.ino
 * 
 * @brief Gestion de l'éclairage de la cabane de minou
*/
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <EEPROM.h>

#define ENABLE_MQTT
//#define ENABLE_OTA

#ifdef ENABLE_OTA
  #include <ArduinoOTA.h>
#endif

#define EEPROM_SIZE (1024)  

#include <DHTesp.h>
#include <uri/UriBraces.h>
#include "ldserver.h"

//#define FASTLED_ESP8266_NODEMCU_PIN_ORDER
#define FASTLED_ESP8266_D1_PIN_ORDER
//#define FASTLED_ESP8266_RAW_PIN_ORDER
#include "FastLED.h"
#include "timer.h"
#include "ldanim.h"

#include "credentials.h"

#include "move.h"

#ifdef ENABLE_MQTT
  #include <EspMQTTClient.h>

#define MQTT_TRACE_ON
#define ENABLE_MOVE_EVENT

#define TOPIC_PREFIX  "/oiso"
#define TOPIC_CMD     "cmd"
#define TOPIC_DATA    "data"
#define TOPIC_LOG     "log"
#define TOPIC_EVENT   "event"

#define DELAY_TASK_REPORT_COMM  (3600*1000UL)
#define DELAY_TASK_REPORT_DATA  (5*60*1000UL)
#define DELAY_TASK_MQTT_OK      (800UL)

EspMQTTClient mqttClient(
  STASSID,
  STAPSK,
  MQTT_IP,
  MQTT_LOGIN,
  MQTT_PASS,
  "oiso_xxx"
);

int g_id=0;
  
#endif


#define PIN_MOVE          D1
#define PIN_DATA_LED      D2 
#define PIN_DHT11         D7
#define PIN_BATT          A0

const char* ssid = STASSID;
const char* password = STAPSK;

ESP8266WebServer server(80);

Timer wdg=Timer(3600*1000U);
Timer mvtmr=Timer(30000U);

Timer tmrTemp=Timer(40*1000U);

/**
 * @brief Configuration des bandeau de LEDs
 * @{
*/
#define POWER_MAX         80
#define POWER_MIN         50
#define NUM_LEDS          26

int g_power;
CRGB leds[NUM_LEDS];
/**
 * @}
*/


DHTesp dht;
int g_hum=0;
int g_temp=0;
int g_batt=0;

Move mv=Move(false);

#define SIZE_PROG (500)
char prog[SIZE_PROG] = "S26E33LrOS1E23X200LgOWLrOWLbOWLgOWLrOWLbOWS26E33LgOS1E23X200LgOWLrOWLbOWLgOWLrOWLbOWS26E33LbOS1E23X200LgOWLrOWLbOWLgOWLrOWLbOW*";

LdServer ldsrv=LdServer(&server,leds,NUM_LEDS,prog,SIZE_PROG,"leds");

void addJsonVarInt(char *strBuffer,char *strName,int iVal)
{
  char tmp[100];
  sprintf(tmp,"\"%s\":%d",strName,iVal);
  strcat(strBuffer,tmp);
}

void addJsonVarStr(char *strBuffer,char *strName,char *strVal)
{
  char tmp[100];
  sprintf(tmp,"\"%s\":\"%s\"",strName,strVal);
  strcat(strBuffer,tmp);
}

void makeJsonSensors(char *strJson)
{
  strcpy(strJson,"");
  strcat(strJson,"{");
  addJsonVarInt(strJson,"temp",g_temp);
  strcat(strJson,",");
  addJsonVarInt(strJson,"hum",g_hum);
  strcat(strJson,",");
  addJsonVarInt(strJson,"moves",mv.getCount());
  strcat(strJson,",");
  addJsonVarInt(strJson,"batt",g_batt);
  strcat(strJson,",");  
  addJsonVarInt(strJson,"ticks",millis());  
  strcat(strJson,"}");
  
  server.send(200, "text/plain", strJson);  
}


#ifdef ENABLE_MQTT
void sendLog(char *strMsg)
{
  char strTopicLog[50];
  sprintf(strTopicLog,"%s/%s/%03d",TOPIC_PREFIX,TOPIC_LOG,g_id);
  mqttClient.publish(strTopicLog, strMsg);
}

void sendData(char *strMsg)
{
  char strTopicData[50];
  sprintf(strTopicData,"%s/%s/%03d",TOPIC_PREFIX,TOPIC_DATA,g_id);
  mqttClient.publish(strTopicData, strMsg);
}

void sendMoveEvent(void)
{
  char strTopic[50];
  sprintf(strTopic,"%s/%s/%03d",TOPIC_PREFIX,TOPIC_EVENT,g_id);
  char strMsg[80];
  sprintf(strMsg,"{\"type\":\"event\",\"name\":\"move\",\"moves\":%ld,\"ticks\":%ld}",mv.getCount(),millis());

  Serial.print(strTopic);
  Serial.print(": ");
  Serial.println(strMsg);
  
  mqttClient.publish(strTopic, strMsg);
}

void taskReportComm(void)
{
  char strReport[50];
  sprintf(strReport,"comm %d",mqttClient.getConnectionEstablishedCount());
  sendLog(strReport);
  
  mqttClient.executeDelayed(DELAY_TASK_REPORT_COMM, taskReportComm);
}

void taskReportData(void)
{
  char strJson[500];
  makeJsonSensors(strJson);
  sendData(strJson);
  
  mqttClient.executeDelayed(DELAY_TASK_REPORT_DATA, taskReportData);
}

void setup_mqtt(void)
{
  g_id=MQTT_ID;
  static char strName[15];
  sprintf(strName,"oiso_%03d",g_id);
  mqttClient.setMqttClientName(strName);
  Serial.print("Start Mqtt ");
  Serial.print(strName);
  Serial.println("...");

  mqttClient.enableMQTTPersistence();

  #ifdef MQTT_TRACE_ON
    mqttClient.enableDebuggingMessages();
  #endif

  mqttClient.executeDelayed(DELAY_TASK_REPORT_COMM, taskReportComm);
  mqttClient.executeDelayed(DELAY_TASK_REPORT_DATA, taskReportData);
  //client.executeDelayed(DELAY_TASK_MQTT_OK, taskBlinkLed);  
}

void onReceiveCmd(const String &payload)
{
  Serial.println(payload);
}

void onConnectionEstablished()
{
  Serial.println("Connection Mqtt.");

  char strTopicCmd[50];
  sprintf(strTopicCmd,"%s/%s/%03d",TOPIC_PREFIX,TOPIC_CMD,g_id);
  mqttClient.subscribe(strTopicCmd,onReceiveCmd);

  sendLog("MQTT Connected");
}

#endif

/**
 * @brief Allume toutes les LEDs avec la couleur fournie
 * @param pLeds Buffer des LEDs
 * @param a Couleur
*/
void setAll(CRGB *pLeds,CRGB a)
{
  for (int i=0;i<NUM_LEDS;i++)
  {
     pLeds[i] = a;
  }
}

/**
 * @brief Eteint toutes les LEDs
 * @param pLeds Buffer des LEDs 
*/
void clearAll(CRGB *pLeds)
{
  setAll(pLeds,CRGB::Black);
}

/**
 * @brief Controleur WS /leds/wdg/info
 * 
 * Retourne les informations du watchdog
 * 
 * {
 *    "enabled":true,
 *    "delay":456,
 *    "elapsed":15
 * }
 * 
*/
void handleWdgInfo()
{ 
  String s="{\"enabled\":\"";
  s+=wdg.isRunning()==true?"true":"false";
  s+="\", \"duration\":";
  s+=wdg.getDuration_ms()/1000U;
  s+=", \"remaining\":";
  s+=wdg.getRemaining_ms()/1000U;  
  s+="}";
  
  server.send(200, "text/plain", s);
}

/**
 * @brief Controleur WS /leds/wdg
 * 
 * Pilotage du watchdog
 * 
 *   - /leds/wdg => Rearme le watchdog. Retourne WDG OK.
 *   - /leds/wdg?enable=1 => Active le watchdog. Retourne WDG ENABLED ou WDG DISABLED
 *   - /leds/wdg?delay=456 => Configure le delai du watchdog (en s). Retourne WDG DELAY 456.
*/
void handleWdg()
{
  if (server.args()==0)
  {
    if (wdg.isRunning()==true)
      wdg.start();
      
    server.send(200, "text/plain", "WDG OK");
    
    return;
  }
  else if (server.args()==1)
  {
    if (strcmp(server.argName(0).c_str(),"enable")==0)
    {
      if (strcmp(server.arg(0).c_str(),"1")==0)
      {
        wdg.start();
        server.send(200, "text/plain", "WDG ENABLED");
        return;
      }
      else
      {
        wdg.stop();
        server.send(200, "text/plain", "WDG DISABLED");
        return;
      }
    }
    else if (strcmp(server.argName(0).c_str(),"delay")==0)
    {
      long delay_s=0;
      if (sscanf(server.arg(0).c_str(),"%ld",&delay_s)==1)
      {
        unsigned long delay_ms=delay_s*1000L;

        wdg.setDuration(delay_ms);
        wdg.start();
        
        String s="WDG DELAY ";
        s+=delay_s;
        s+=" seconds";
        server.send(200, "text/plain", s);
        return;
      }
    }
  }

  server.send(400, "text/plain", "ERROR");
}

/**
 * @brief Controlleur WS eteint toutes les LEDs /leds/clearall
*/
void handleClearAll()
{  
  ldsrv.handlerClearAll();
  server.send(200, "text/plain", "OK");
  FastLED.show();
}

/**
 * @brief Controleur /leds/set
*/
void handleSetLeds()
{
  if (wdg.isRunning()==true)
    wdg.start();

  ldsrv.handlerSetLeds();
}

/**
 * @brief Controlleur WS informations
 * 
 * Retourne les informations sur le noeud
*/
void handleInfo()
{      
  char strJson[500];
  strcpy(strJson,"[{");
  addJsonVarStr(strJson,"name","leds");
  strcpy(strJson,",");
  addJsonVarStr(strJson,"description","Gestion des LEDs cabane oiseaux");
  strcpy(strJson,",");
  addJsonVarInt(strJson,"num_leds",NUM_LEDS);
  strcat(strJson,"}]");
  
  server.send(200, "text/plain", strJson);
}



void handleSensors(void)
{
  char strJson[500]="";

  makeJsonSensors(strJson);
  
  server.send(200, "text/plain", strJson);  
}

/**
 * @brief Controlleur WS pour gerer les programmes MACRO d'animation des LEDs
 * 
 * On envoi une chaine contenant le programme MACRO
 * 
 * - /leds/anim?prog="S26E33LrOS1E23..." => Modifie le programme en cours. Retourne un echo du programme si reussite.
 * - /leds/anim?enable=1 => Active l'animation. Retourne ANIM ENABLED ou ANIM DISABLED
 * - /leds/anim?info => Retourne la configuration de l'animation
*/
void handleAnim()
{
  if (wdg.isRunning()==true)
    wdg.start();

  ldsrv.handlerAnim();
}

/**
 * @brief Controleur WS en cas d'erreur
*/
void handleNotFound()
{
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void measureTempHum(DHTesp &dht,int *temp_dg,int *hum_pc,Timer *tmr)
{
  bool flgFound=false;
  for (int i=4;i>0;i--)
  {
    TempAndHumidity newValues = dht.getTempAndHumidity();
  
    if (dht.getStatus()==DHTesp::ERROR_NONE)
    {
      *temp_dg=newValues.temperature;
      *hum_pc=newValues.humidity;  
      tmr->start();
      break;
    }

    delay(20);
  }

  if (tmr->tick()==true)
  {
    *temp_dg=-255;
    *hum_pc=-255;
  }
}

/**
 * @brief Setup d'initialisation de l'arduino
*/
void setup(void)
{
  g_power=POWER_MAX;
  
  Serial.begin(115200);
  Serial.println("BOOT");

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  
  pinMode(PIN_DATA_LED, OUTPUT);
  digitalWrite(PIN_DATA_LED, LOW);

  pinMode(PIN_MOVE, INPUT);
  mv.begin(PIN_MOVE);

  pinMode(PIN_DHT11, INPUT_PULLUP);
  pinMode(PIN_BATT, INPUT);  
  
  dht.setup(PIN_DHT11, DHTesp::DHT11);

  FastLED.addLeds<NEOPIXEL, PIN_DATA_LED>(leds, NUM_LEDS); 
  FastLED.setBrightness(g_power);

  setAll(leds,CRGB::Red);
  FastLED.show();
  delay(2000);
  
  clearAll(leds);
  FastLED.show();

  #ifndef ENABLE_MQTT
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) 
  {
    Serial.println("MDNS responder started");
  }
  #else
    setup_mqtt();  
  #endif

  ldsrv.init();

  server.on(UriBraces("/leds/clearall"), handleClearAll);
  server.on(UriBraces("/leds/anim"), handleAnim); 
  server.on(UriBraces("/leds/set"), handleSetLeds);
  
  server.on("/leds/info", handleInfo);
  server.on("/leds/wdg", handleWdg);
  server.on("/leds/wdg/info", handleWdgInfo);  
  
  server.on("/sensors", handleSensors);  

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");

  wdg.stop();

  #ifdef ENABLE_OTA
    ArduinoOTA.setHostname(OTA_NAME);
    ArduinoOTA.setPassword((const char *)"123");
  
    ArduinoOTA.onStart([]() {
      Serial.println("Start");
    });
    ArduinoOTA.onEnd([]() {
      Serial.println("\nEnd");
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    });
    ArduinoOTA.onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });
    ArduinoOTA.begin();
  #endif
}

/**
 * @loop Boucle principale de l'arduino
*/
void test_loop(void) 
{
  static int cnt=200;
  static int tst_etat=1;

  measureTempHum(dht,&g_temp,&g_hum,&tmrTemp);

  delay(10);
   
  if (mv.tick()==true)
  {
    Serial.println("Move");
  }

  FastLED.show();

  cnt--;
  if (cnt<=0)
  {
    cnt=400;
    if (tst_etat==0)
    {
      setAll(leds,CRGB::Red);
      tst_etat=1;

      Serial.print("Hum:");
      Serial.println(g_hum);
      Serial.print("Tmp:");
      Serial.println(g_temp);
      
      Serial.print("Move:");
      Serial.println(mv.getCount());

      Serial.println("");
    }
    else    
    {
      clearAll(leds);
      tst_etat=0;
    }    
  }

  #ifdef ENABLE_OTA
    ArduinoOTA.handle();
  #endif  

}

void wdgSwitchAllOff()
{  
  clearAll(leds);
}

void loop_app()
{
  measureTempHum(dht,&g_temp,&g_hum,&tmrTemp);
  g_batt=analogRead(PIN_BATT);

  //if (wdg.isRunning()==false)
  { 
    if (mv.tick()==true)
    {
      setAll(leds,CRGB::Red);
      
      Serial.println("Move");
      
      #ifdef ENABLE_MOVE_EVENT
        if (mvtmr.isRunning()==false)        
          sendMoveEvent();
      #endif
      
      mvtmr.start();
    }
  }    

  if (mvtmr.tick()==true)
  { 
      Serial.println("Timer move off");     
      clearAll(leds);
  }
  
  server.handleClient();
  MDNS.update();

  ldsrv.tick();
  
  if (wdg.tick()==true)
  {
    wdgSwitchAllOff();    
  }

  FastLED.show();

  #ifdef ENABLE_OTA
    ArduinoOTA.handle();
  #endif  

  #ifdef ENABLE_MQTT
    mqttClient.loop();
  #endif

  digitalWrite(LED_BUILTIN, !digitalRead(PIN_MOVE));
  
  unsigned long ul=millis();
  if ( (ul % 1000) == 0 )
  {
    //Serial.println(g_batt);
    //digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    delay(2);
  }
}

void loop()
{
  loop_app();
  //test_loop();
}
