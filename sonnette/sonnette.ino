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
#include "PubSubClient.h"

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
bool flgMouvementInside=false;
bool flgBoutonInside=false;

bool flgLedVerte=false;
bool flgLedRouge=false;

bool flgPasDeReseau=false;

bool flgMouvement_front_up=false;
long lTickStartMove=0;
bool flgMouvementEnCours=false;
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
long lTick5s=0;
bool flgTick5s=false;
bool flgTick5s_prev=false;
bool flgTick5sUpFront=false;


WiFiClient espClient;
PubSubClient client(espClient);

// Don't change the function below. This functions connects your ESP8266 to your router
void setup_wifi()
{
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  for (int i=0;i<20;i++)
  {
    if (WiFi.status() != WL_CONNECTED) 
    {
      delay(500);
      Serial.print(".");
    }
  }

  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("");
    Serial.print("WiFi connected - ESP IP address: ");
    Serial.println(WiFi.localIP());
    flgPasDeReseau=false;
  }
  else
  {
    flgPasDeReseau=true;
  }
}

// This functions reconnects your ESP8266 to your MQTT broker
// Change the function below if you want to subscribe to more topics with your ESP8266 
void reconnect() 
{
  // Loop until we're reconnected
  if (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    
    // Attempt to connect
    if (client.connect(mqtt_id,mqtt_login,mqtt_pass)) 
    {
      Serial.println("connected");  
    } 
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
    }
  }
}

void sendEvent(char *strEvent)
{
  long lTicks=millis();
  char strMsg[100];
  sprintf(strMsg,"{\"event\":\"%s\",\"moves\":\"%d\",\"drings\":\"%d\",\"ticks\":\"%ld\"}",strEvent,iCtrMouvement,iCtrSonnette,lTicks);  
  client.publish("/maison/sonnette/events", strMsg,true);
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

  //temps.begin();
  //temps.forceUpdate();
  //rtc.setTime(temps.getEpochTime());
  //rtc.setTime(temps.getSeconds(),temps.getMinutes(),temps.getHours(),temps.getDay(),temps.getMonth(),temps.getYear());

  //Serial.println(temps.getFormattedTime());
  sendEvent("boot");

  lTick250ms=millis();
  lTick500ms=millis();
  lTick1s=millis();
  lTick5s=millis();
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

  if (getElapsedTimeFrom(lTick5s)>=5000)
  {
    flgTick5s=!flgTick5s;
    lTick5s=millis();
  }    
  
  if ( (flgTick5s_prev==false) && (flgTick5s==true) )
  {
    flgTick5sUpFront=true;
  }
  else
  {
    flgTick5sUpFront=false;
  }
  flgTick5s_prev=flgTick5s;
}

void tick_inside(void)
{
  if (flgBoutonInside==false)
    flgBoutonInside=readFiltered(PIN_BOUTON,LOW);

  if (flgMouvementInside==false)
    flgMouvementInside=readFiltered(PIN_MOVE,HIGH);
}

void loop() 
{
  /// ******************* SURVEILLANCE COMM *********************

  flgMouvementInside=false;
  flgBoutonInside=false;
  if (flgTick5sUpFront==true)
  {
    if ( (!client.connected()) && (!client.loop()) ) 
    {
        digitalWrite(PIN_LED_RED,HIGH);
        digitalWrite(PIN_LED_GREEN,LOW);
      flgPasDeReseau=true;
      reconnect();
      sendEvent("connect");
    }
    else
    {
      flgPasDeReseau=false;        
    }
  }
  
  /// ******************* PROCESS ENTREES *********************
  
  /// Lire les entrees (en filtrant)
  flgBouton=readFiltered(PIN_BOUTON,LOW) || flgBoutonInside;
  flgMouvement=readFiltered(PIN_MOVE,HIGH) || flgMouvementInside;
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
    
    lTickStartMove=millis();
    flgMouvementEnCours=true;

    sendEvent("move");
  }

  /// ******************* DETECTION DES EVENEMENTS *********************

  /// Mouvement...
  if (flgMouvementEnCours==true)
  {
    if (flgMouvement==true)
    {
      if (getElapsedTimeFrom(lTickStartMove)>10200)
      {
        if (flgSilent==false)
          shortMelodie();
        flgAbouge=true;

        flgMouvementEnCours=false;

        sendEvent("move_dring");
      }
    }    
    else
    {
      Serial.print("Move detected: ");
      Serial.print(getElapsedTimeFrom(lTickStartMove));
      Serial.println(" ms");
      
      flgMouvementEnCours=false;

      char strMsg[50];
      long lDelta=getElapsedTimeFrom(lTickStartMove);
      sprintf(strMsg,"Fin move: %ld",lDelta);
      client.publish("/maison/sonnette/logs", strMsg,true);
    }
  }

  /// Appui sur le bouton de la sonnette...
  if ( flgBouton==true )
  {
    iCtrSonnette++;

    if (flgSilent==false)
      melodie();
    
    sendEvent("dring");
    flgAsonne=true;
  }

  /// Reset des evenements
  if (flgSelector==true)
  {
    flgAsonne=false;
    flgAbouge=false;
  }

  delay(100);

  /// ******************* DETERMINER ETAT DES LEDS *********************

  // Si mode silencieux, LED rouge a 1s
  if (flgSilent==true)
    flgLedRouge=flgTick1s;
  else
    flgLedRouge=false;

  /// Si quelqu'un a sonne, Verte a 500ms
  if (flgAsonne)
  {
    flgLedVerte=flgTick250ms;
  }
  /// Si quelqu'un a sonne, Rouge a 250ms
  else if (flgAbouge)
  {
    flgLedVerte=flgTick1s;    
  }
  else
  {
    flgLedVerte=false;        
  }

  /// Si pas de reseau, on clignote alternativement Vert/Rouge Pendant 5s puis on affiche l'etat en cours pdt 5s
  if ( flgPasDeReseau==true )
  {
    flgLedRouge=flgTick250ms;
    flgLedVerte=!flgTick250ms;
  }

  /// ******************* APPLIQUER ETAT DES LEDS *********************

  digitalWrite(PIN_LED_RED,(flgLedRouge==true)?HIGH:LOW);
  digitalWrite(PIN_LED_GREEN,(flgLedVerte==true)?HIGH:LOW);

  flgMouvement_prev=flgMouvement;
}
