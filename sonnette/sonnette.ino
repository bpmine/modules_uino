/**
 * 
 * @file sonnette.ino
 * 
 * GESTION DE LA SONNETTE D'ENTREE:
 *   - Détection bouton
 *   - Détection mouvement
 *   - Son 1 si bouton
 *   - Son 2 si detection
 *   - Compteurs de detections (bouton ou mouvement) en eeprom
 *   - Modes de fonctionnement
 *       - Silence total (Juste la LEDs)
 *       - Que l'interrupteur avec Son1
 *       - Interrupteur Son1 + mouvement Son2
 *       
 *   - LED indiquant un déclenchement pendant la periode ecoulee
 *   - Remettre a 0 le detecteur
 *    
*/
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>

//#include <ESP32Time.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#include "credentials.h"

#define PIN_BUZZER    5   ///< D1
#define PIN_BOUTON    4   ///< D2
#define PIN_LED       0   ///< D3
#define PIN_LED2      2   ///< D4
#define PIN_MOVE      14  ///< D5
#define PIN_SELECTOR  12  ///< D6
#define PIN_SILENT    13  ///< D7

bool flgMouvement=false;
bool flgBouton=false;
bool flgMouvement_prev=false;
bool flgSelector=false;
bool flgSilent=false;

WiFiClient espClient;
PubSubClient client(espClient);

WiFiUDP ntpUDP;
NTPClient temps(ntpUDP, "fr.pool.ntp.org", 3600, 60000);
//ESP32Time rtc;

// Don't change the function below. This functions connects your ESP8266 to your router
void setup_wifi()
{
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected - ESP IP address: ");
  Serial.println(WiFi.localIP());
}

// This functions reconnects your ESP8266 to your MQTT broker
// Change the function below if you want to subscribe to more topics with your ESP8266 
void reconnect() 
{
  // Loop until we're reconnected
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    
    // Attempt to connect
    /*
     YOU MIGHT NEED TO CHANGE THIS LINE, IF YOU'RE HAVING PROBLEMS WITH MQTT MULTIPLE CONNECTIONS
     To change the ESP device ID, you will have to give a new name to the ESP8266.
     Here's how it looks:
       if (client.connect("ESP8266Client")) {
     You can do it like this:
       if (client.connect("ESP1_Office")) {
     Then, for the other ESP:
       if (client.connect("ESP2_Garage")) {
      That should solve your MQTT multiple connections problem
    */
    if (client.connect(mqtt_id,mqtt_login,mqtt_pass)) 
    {
      Serial.println("connected");  
    } 
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() 
{
  Serial.begin(9600);

  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_LED2, OUTPUT);
  pinMode(PIN_BOUTON, INPUT_PULLUP);  
  pinMode(PIN_MOVE, INPUT); 
  pinMode(PIN_BUZZER, OUTPUT);   

  pinMode(PIN_SELECTOR, INPUT_PULLUP);  
  pinMode(PIN_SILENT, INPUT_PULLUP);  

  digitalWrite(PIN_LED,LOW);
  digitalWrite(PIN_LED2,LOW);

  flgMouvement=digitalRead(PIN_MOVE);
  flgMouvement_prev=flgMouvement;

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  //client.setCallback(callback);

  temps.begin();
  temps.forceUpdate();
  //rtc.setTime(temps.getEpochTime());
  //rtc.setTime(temps.getSeconds(),temps.getMinutes(),temps.getHours(),temps.getDay(),temps.getMonth(),temps.getYear());

  Serial.println(temps.getFormattedTime());
}

void shortMelodie()
{
  tone(PIN_BUZZER, 440);
  delay(400);
  tone(PIN_BUZZER, 440);
  noTone(PIN_BUZZER);
}

void melodie()
{
  digitalWrite(PIN_LED,HIGH);
  
  tone(PIN_BUZZER, 440);
  delay(1000);
  tone(PIN_BUZZER, 349);
  delay(1000);
  tone(PIN_BUZZER, 392);
  delay(1000);
  tone(PIN_BUZZER, 261);
  delay(1000);
  noTone(PIN_BUZZER);
  delay(100);
  tone(PIN_BUZZER, 261);
  delay(1000);
  tone(PIN_BUZZER, 392);
  delay(1000);
  tone(PIN_BUZZER, 440);
  delay(1000);
  tone(PIN_BUZZER, 349);
  delay(1000);
  noTone(PIN_BUZZER);  

  digitalWrite(PIN_LED,LOW);
}

void loop() 
{
  /// Lire les entrees
  flgBouton=(digitalRead(PIN_BOUTON)==0)?true:false;
  flgMouvement=digitalRead(PIN_MOVE);
  flgSelector=(digitalRead(PIN_SELECTOR)==0)?true:false;
  flgSilent=(digitalRead(PIN_SILENT)==0)?true:false;

  /// Calculs
  if (flgMouvement==true)
  {
    shortMelodie();
  }

  if (flgBouton==true)
  {
    melodie();
    client.publish("/maison/sonnette/events", "Dring");
  }

  delay(100);

  if (!client.connected()) {
    reconnect();
  }
  if(!client.loop())
  {
    client.connect(mqtt_id,mqtt_login,mqtt_pass);
    client.publish("/maison/sonnette/event","BOOT");
  }


  /// Ecrire les sorties

  if (flgSilent==false)
    digitalWrite(PIN_LED2,(flgSelector==true)?HIGH:LOW);
  else
    digitalWrite(PIN_LED2,!digitalRead(PIN_LED2));

}
