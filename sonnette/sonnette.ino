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
#include <ArduinoMqttClient.h>

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
MqttClient client(espClient);

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

void shortMelodie()
{
  digitalWrite(PIN_LED_GREEN,HIGH);

  #ifdef NO_SOUND
    delay(100);
    return;
  #endif

  tone(PIN_BUZZER, 440*2);
  delay(50);
  noTone(PIN_BUZZER);
}

int LONG=200;
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

void tryReconnect()
{
  if ( (flgPasDeReseau==true) && (flgTick5sUpFront==true) )
  {
    Serial.print("Tentative reconnexion: ");
    
    if (!client.connect(mqtt_server,1883))
    {
      flgPasDeReseau=true;
      Serial.println("[NOK]");
      return;
    }

    client.stop();
    Serial.println("[OK]");

    sendEvent("Reconnect");
  }  
}

void sendEvent(char *strEvent)
{
  Serial.print("Send ");
  Serial.print(strEvent);
  Serial.print(": ");
  
  long lTicks=millis();
  char strMsg[100];
  sprintf(strMsg,"{\"event\":\"%s\",\"moves\":\"%d\",\"drings\":\"%d\",\"ticks\":\"%ld\"}",strEvent,iCtrMouvement,iCtrSonnette,lTicks);  
  
  if (!client.connect(mqtt_server,1883))
  {
     flgPasDeReseau=true;
     Serial.println("[ERROR COMM.]");
     return;
  }

  client.beginMessage("/maison/sonnette/events", true,1,false);
  client.write((uint8_t *)strMsg,strlen(strMsg));
  client.endMessage();
  client.stop();

  Serial.println("[OK]");
  
  flgPasDeReseau=false;
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
  client.setId(mqtt_id);
  client.setUsernamePassword(mqtt_login, mqtt_pass);
  client.setCleanSession(true);

  sendEvent("boot");

  lTick250ms=millis();
  lTick500ms=millis();
  lTick1s=millis();
  lTick5s=millis();
}


void loop() 
{
  /// ******************* SURVEILLANCE COMM *********************

  client.poll();
  tryReconnect();
    
  /// ******************* PROCESS ENTREES *********************
  
  /// Lire les entrees (en filtrant)
  flgBouton=readFiltered(PIN_BOUTON,LOW) ;
  flgMouvement=readFiltered(PIN_MOVE,HIGH) ;
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

      /*char strMsg[50];
      long lDelta=getElapsedTimeFrom(lTickStartMove);
      sprintf(strMsg,"Fin move: %ld",lDelta);
      sendEvent("/maison/sonnette/logs", strMsg,true);*/
    }
  }

  /// Appui sur le bouton de la sonnette...
  if ( flgBouton==true )
  {
    iCtrSonnette++;

    if (flgSilent==false)
    {
      melodie();
    }
    
    sendEvent("dring");
    flgAsonne=true;

    if (flgSilent==true)
    {
      /// Timing pour filtrer si pas de melodie
      delay(2000);      
    }
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
