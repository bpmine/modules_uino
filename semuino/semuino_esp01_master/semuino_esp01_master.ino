/**
 * @file semuino_esp01_master.ino
 * 
 * @brief Gestion du semuino avec interface wifi
 *
 * Ce composant est basée sur un ESP8266EX et un module ESP01
 *
 * PIN OUT:
 *   - D1: SCL
 *   - D2: SDA
 *   - 5V: Aliment? par un 7805
 *   - GND: Masse
 *   
*/
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <string.h>
#include <Wire.h>

#include "semuino.h"
#include "wsctrl.h"

#define VERSION "V0.0"

//#define MODE_SIMU
#define MODE_AP
//#define USE_OTA

#ifdef USE_OTA
  #include <ArduinoOTA.h>
#endif

#ifdef MODE_AP
  const char ssid[] = "Semuino";
  const char password[] = "Semuino38";  ///< Minimum 8 chars
  
  IPAddress local_IP(192,168,0,1);
  IPAddress gateway(192,168,0,1);
  IPAddress subnet(255,255,255,0);  
#else
  #include "wifi_code.h"
  
  const char* ssid = STASSID;      ///< Voir le fichier wifi_code.h
  const char* password = STAPSK;    ///< Voir le fichier wifi_code.h
#endif

/**
 * @brief Setup d'initialisation de l'arduino
*/
void setup(void)
{ 
  Serial.begin(115200);
  char tmp[20];
  sprintf(tmp,"BOOT %s",VERSION);
  Serial.println(tmp);

  Wire.begin();
  Wire.setClock(10000);
  
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
    Serial.println("MDNS responder started");
  }

  wsctrl_init();
  Serial.println("HTTP server started");

  #ifdef USE_OTA
    ArduinoOTA.setHostname(OTA_NAME);

    // No authentication by default
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

  semuino_init();
}


/**
 * @brief Boucle cyclique de l'arduino
 * */
void loop()
{  
  MDNS.update();

  #ifdef USE_OTA
    ArduinoOTA.handle();
  #endif

  wsctrl_loop();
  //semuino_loop();  // Code de Bernard commente en attendant qu'il soit fini....
  
  delay(1);
}
