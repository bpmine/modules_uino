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
#define PIN_LED_RED   0   ///< D3
#define PIN_LED_GREEN 2   ///< D4
#define PIN_MOVE      14  ///< D5
#define PIN_SELECTOR  12  ///< D6
#define PIN_SILENT    13  ///< D7

//#define NO_SOUND

bool flgMouvement=false;
bool flgBouton=false;
bool flgMouvement_prev=false;
bool flgSelector=false;
bool flgSilent=false;

bool flgMouvement_front_up=false;
int iCtrMouvement=0;
int iCtrSonnette=0;

bool flgAsonne=false;
bool flgAbouge=false;

long lTick250ms=0;
bool flgTick250ms=false;
long lTick500ms=0;
bool flgTick500ms=false;
long lTick1s=0;
bool flgTick1s=false;


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

void sendEvent(char *strEvent)
{
  long lTicks=millis();
    char strMsg[100];
    sprintf(strMsg,"{\"event\":\"%s\",\"moves\":\"%d\",\"drings\":\"%d\",\"ticks\":\"%ld\"}",strEvent,iCtrMouvement,iCtrSonnette,lTicks);  
    client.publish("/maison/sonnette/events", strMsg);
}

void setup() 
{
  Serial.begin(9600);

  pinMode(PIN_LED_RED, OUTPUT);
  pinMode(PIN_LED_GREEN, OUTPUT);
  pinMode(PIN_BOUTON, INPUT_PULLUP);  
  pinMode(PIN_MOVE, INPUT); 
  pinMode(PIN_BUZZER, OUTPUT);   

  pinMode(PIN_SELECTOR, INPUT_PULLUP);  
  pinMode(PIN_SILENT, INPUT_PULLUP);  

  digitalWrite(PIN_LED_RED,LOW);
  digitalWrite(PIN_LED_GREEN,LOW);

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
  sendEvent("boot");

  lTick250ms=millis();
  lTick500ms=millis();
  lTick1s=millis();
}

void shortMelodie()
{
  digitalWrite(PIN_LED_GREEN,HIGH);

  #ifdef NO_SOUND
    delay(800);
    return;
  #endif

  tone(PIN_BUZZER, 1500);
  delay(100);
  noTone(PIN_BUZZER);
  delay(100);
  tone(PIN_BUZZER, 800);
  delay(100);
  noTone(PIN_BUZZER);
}

//int LONG=1000;
int LONG=200;
//int SHORT=100;
int SHORT=100;

void melodie()
{
  digitalWrite(PIN_LED_GREEN,HIGH);
  
  #ifdef NO_SOUND
    delay(3000);
    return;
  #endif
  
  digitalWrite(PIN_LED_GREEN,HIGH);
  
  tone(PIN_BUZZER, 440);
  delay(LONG);
  tone(PIN_BUZZER, 349);
  delay(LONG);
  tone(PIN_BUZZER, 392);
  delay(LONG);
  tone(PIN_BUZZER, 261);
  delay(LONG);
  noTone(PIN_BUZZER);
  delay(SHORT);
  tone(PIN_BUZZER, 261);
  delay(LONG);
  tone(PIN_BUZZER, 392);
  delay(LONG);
  tone(PIN_BUZZER, 440);
  delay(LONG);
  tone(PIN_BUZZER, 349);
  delay(LONG);
  noTone(PIN_BUZZER);  
}

bool readFiltered(int pin,int state)
{
  if (digitalRead(pin)==state)
  {
    for (int i=0;i<5;i++)
    {
      delay(1);
      if (digitalRead(pin)!=state)
        return false;
    }

    return true;
  }

  return false;
}

long getElapsedTimeFrom(long t0)
{
  long t=millis();
  long delta=0;
  
  if (t<t0)
    delta=0xFFFFFFFF-t0+t;
  else
    delta=t-t0;

  return delta;
}

void flgTicks()
{
  if (getElapsedTimeFrom(lTick250ms)>=250)
  {
    flgTick250ms=!flgTick250ms;
    lTick250ms=millis();
  }
  
  if (getElapsedTimeFrom(lTick500ms)>=500)
  {
    flgTick500ms=!flgTick500ms;
    lTick500ms=millis();
  }
  
  if (getElapsedTimeFrom(lTick1s)>=1000)
  {
    flgTick1s=!flgTick1s;
    lTick1s=millis();
  }  
}

void loop() 
{
  /// Lire les entrees (en filtrant)
  flgBouton=readFiltered(PIN_BOUTON,LOW);
  flgMouvement=readFiltered(PIN_MOVE,HIGH);
  flgSelector=readFiltered(PIN_SELECTOR,LOW);
  flgSilent=readFiltered(PIN_SILENT,LOW);

  /// Detection front montant pour le mouvement
  if ( (flgMouvement==true) && (flgMouvement_prev==false)  )
    flgMouvement_front_up=true;
  else
    flgMouvement_front_up=false;

  flgTicks();
  
  /// Detection mouvement
  if (flgMouvement_front_up==true)
  {
    iCtrMouvement++;

    if (flgSilent==false)
      shortMelodie();

    sendEvent("move");
    flgAbouge=true;
  }

  if ( flgBouton==true )
  {
    iCtrSonnette++;

    if (flgSilent==false)
      melodie();
    
    sendEvent("dring");
    flgAsonne=true;
  }

  if (flgSelector==true)
  {
    flgAsonne=false;
    flgAbouge=false;
  }

  delay(100);

  if (!client.connected()) {
    reconnect();
  }
  
  if(!client.loop())
  {
    client.connect(mqtt_id,mqtt_login,mqtt_pass);
    sendEvent("reconnect");
  }

  /// Ecrire les sorties
  if (flgSilent==true)
    digitalWrite(PIN_LED_RED,flgTick1s);
  else
    digitalWrite(PIN_LED_RED,LOW);
  

  if (flgAsonne)
  {
    digitalWrite(PIN_LED_GREEN,flgTick250ms);
  }
  else if (flgAbouge)
  {
    digitalWrite(PIN_LED_GREEN,flgTick1s);    
  }
  else
  {
    digitalWrite(PIN_LED_GREEN,LOW);        
  }

  flgMouvement_prev=flgMouvement;
}
