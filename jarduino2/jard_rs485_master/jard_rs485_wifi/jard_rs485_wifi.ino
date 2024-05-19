#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <string.h>

#include <uri/UriBraces.h>

#include <ArduinoJson.h>

#include "wifi_code.h"
#include "wificomm.h"

#define VERSION "V0.3"
//#define MODE_SIMU
#define MODE_AP
//#define USE_OTA

#ifdef USE_OTA
  #include <ArduinoOTA.h>
#endif

#ifdef MODE_AP
  const char ssid[] = "Master";
  const char password[] = "Master38";  ///< Minimum 8 chars
  
  IPAddress local_IP(192,168,0,1);
  IPAddress gateway(192,168,0,1);
  IPAddress subnet(255,255,255,0);  
#else
  const char* ssid = STASSID;      ///< Voir le fichier wifi_code.h
  const char* password = STAPSK;    ///< Voir le fichier wifi_code.h
#endif

ESP8266WebServer server(80);    ///< Instance du serveur HTTP

void handleInfo()
{
  char strList[30];
    
  String s="{\n";
  s+="  \"name\":\"master\",\n";  
  s+="}\n";
  
  server.send(200, "text/plain", s);
}

char buffer_answer[2000];

void handleState()
{
  Comm.getOyasInfo().makeJsonTxt(buffer_answer,2000);
  server.send(200, "text/plain", buffer_answer);
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

void setup(void)
{
  Serial.begin(9600);
  
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);  

  #ifdef MODE_AP
    if (WiFi.softAP(ssid,password,2,false,8)==false)
      Serial.println("Impossible to config AP!");
      
    if (WiFi.softAPConfig(local_IP,gateway,subnet)==false)
      Serial.println("Impossible to config IPs!");
    

    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(myIP);    
  #else
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
  #endif

  if (MDNS.begin("esp8266")) 
  {
    //Serial.println("MDNS responder started");
  }

  server.on(UriBraces("/info"), handleInfo);
  server.on(UriBraces("/state"), handleState);  
  server.onNotFound(handleNotFound);

  server.begin();
  server.enableCORS(true);
  Serial.println("HTTP server started"); 

  for (int i=0;i<10;i++)
  {
    delay(100);
    yield();
  }

  Comm.begin(&Serial);
}

/**
 * @brief Boucle cyclique de l'arduino
 * */
void loop()
{
  server.handleClient();
  MDNS.update();


  Comm.loop();
}
