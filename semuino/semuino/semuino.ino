/**
 * @brief semuino.ino
 * 
 * SEMUINO - Gestion du meuble à semi
**/
#include "FastLED.h"
#include <EEPROM.h>
#include <DHT.h>
#include <RTClib.h>
#include <Wire.h>

//#define SET_TIME
#define SET_DATE_STR "27/01/2024"
#define SET_TIME_STR __TIME__

#define POWER_MAX         255
#define POWER_MIN         10

#define NUM_LEDS          (23*7)  ///< Nombre de LEDs a chaque etage (3 rangées de 23 LEDs)

#define PIN_SELECT_BTN      (2)   ///< Interrupteur de selection
#define PIN_LED_GREEN       (4)   ///< LED verte

#define PIN_POWER_5V        (3)   ///< Alumage alimentation 5V
#define PIN_DATA_LED_BAS    (5)   ///< Broche etage du bas
#define PIN_DATA_LED_HAUT   (6)   ///< Broche etage du haut
#define PIN_DATA_LED_DESSUS (7)   ///< Broche lampes au dessus du meuble

#define PIN_CMD_P1          (9)   ///< Commande LEDs de puissance 1
#define PIN_CMD_P2          (10)  ///< Commande LEDs de puissance 2
#define PIN_CMD_P3          (11)  ///< Commande LEDs de puissance 3

#define PIN_TEMP            (12)  ///< Capteur de température DHT
#define PIN_HUM1            (A0)  ///< Entree analogique capteur humidité bas
#define PIN_HUM2            (A1)  ///< Entree analogique capteur humidité haut
#define PIN_HUM3            (A2)  ///< Entree analogique capteur humidité dessus du meuble


#define COL_GROWING         CRGB(127,0,32)      ///< Couleur de croissance
#define COL_WHITE           CRGB(127,127,127)   ///< Blanc

#define MAX_MODE  2         ///< Nombre de modes
unsigned char g_mode=0;     ///< Mode en cours
DS1307 rtc;
unsigned long g_t0=0;
bool g_blink_1s=false;
bool g_blink_2s=false;

int g_power=255;              ///< Puissance
CRGB leds_bas[NUM_LEDS];      ///< Tableau des LEDs du bas
CRGB leds_haut[NUM_LEDS];     ///< Tableau des LEDs du haut
//CRGB leds_dessus[NUM_LEDS]; ///< Tableau des LEDs du dessus du meuble

/**
 * @brief Applique une couleur a tout un etage
 * @param pLeds Tableau de LEDs de l'etage
 * @param col Couleur
*/
void setAll(CRGB *pLeds,CRGB col)
{
  for (int i=0;i<NUM_LEDS;i++)
     pLeds[i] = col;
}

/**
 * @brief Applique une couleur a tous les etages en meme temps
 * @param col Couleur
*/
void setAll(CRGB col)
{
  setAll(leds_bas,col);
  setAll(leds_haut,col);
  //setAll(leds_dessus,col);
}

/**
 * @brief Eteint tout un etage
 * @param pLeds Tableau de LEDs de l'etage
*/
void clearAll(CRGB *pLeds)
{
  setAll(pLeds,CRGB::Black);
}

/**
 * @brief Eteint tous les etages
*/
void clearAll()
{
  clearAll(leds_bas);
  clearAll(leds_haut);
  //clearAll(leds_dessus);
}

/**
 * @brief Applique un bleu/blanc/rouge a tout un etage
 * @param pLeds Tableau de LEDs de l'etage
*/
void setBBR(CRGB *pLeds)
{
  for (int i=0;i<(23*3);i++)
  {
     pLeds[i] = CRGB(127,0,0);
  }
  
  for (int i=0;i<23;i++)
  {
     pLeds[i+(23*3)] = CRGB(127,127,127);
  }

  for (int i=0;i<(23*3);i++)
  {
     pLeds[i+(23*4)] = CRGB(0,0,127);
  }
}

/**
 * @brief Animation au demarage (pour tester les LEDs)
 * 
 * Allume successivemeent les LEDs de chacune des 3 rangees de l'etage de 3 couleurs differentes
 * 
 * @param pLeds Tableau de LEDs de l'etage a animer
*/
void animDemarrage(CRGB *pLeds)
{
  CRGB cols[]={CRGB(127,0,0),CRGB(0,127,0),CRGB(0,0,127),CRGB(127,127,127),CRGB(127,0,0),CRGB(0,127,0),CRGB(0,0,127)};
  
  for (int n=0;n<7;n++)
  {
    clearAll(pLeds);
    for (int i=0;i<23;i++)
    {
      pLeds[n*23+i]=cols[n];
      FastLED.show();
      delay(8);      
    }
  } 

   clearAll(pLeds);
}

/**
 * @brief Setup d'arduino (demarage)
 * 
 * - Init des I/O
 * - Lecture du mode en EEPROM
 * - Lecture RTC -> Part en défaut si date incorrecte
*/
void setup() 
{
  randomSeed(millis());
  
  g_power=POWER_MAX;
  
  Serial.begin(9600);
  Serial.println("Boot...");  

  pinMode(PIN_DATA_LED_BAS, OUTPUT);
  digitalWrite(PIN_DATA_LED_BAS,LOW);
  pinMode(PIN_DATA_LED_HAUT, OUTPUT);
  digitalWrite(PIN_DATA_LED_HAUT,LOW);
  pinMode(PIN_DATA_LED_DESSUS, OUTPUT);
  digitalWrite(PIN_DATA_LED_DESSUS,LOW);

  FastLED.addLeds<NEOPIXEL, PIN_DATA_LED_BAS>(leds_bas, NUM_LEDS); 
  FastLED.addLeds<NEOPIXEL, PIN_DATA_LED_HAUT>(leds_haut, NUM_LEDS); 
  //FastLED.addLeds<NEOPIXEL, PIN_DATA_LED_DESSUS>(leds_dessus, NUM_LEDS); 
  FastLED.setBrightness(g_power);

  pinMode(PIN_POWER_5V, OUTPUT);
  digitalWrite(PIN_POWER_5V,LOW);

  pinMode(PIN_CMD_P1, OUTPUT);
  digitalWrite(PIN_CMD_P1,LOW);
  pinMode(PIN_CMD_P2, OUTPUT);
  digitalWrite(PIN_CMD_P2,LOW);
  pinMode(PIN_CMD_P3, OUTPUT);
  digitalWrite(PIN_CMD_P3,LOW);
  pinMode(PIN_TEMP, OUTPUT);
  digitalWrite(PIN_TEMP,LOW);

  pinMode(PIN_SELECT_BTN,INPUT_PULLUP);
  pinMode(PIN_HUM1,INPUT);
  pinMode(PIN_HUM2,INPUT);
  pinMode(PIN_HUM3,INPUT);

  pinMode(LED_BUILTIN,OUTPUT);
  digitalWrite(LED_BUILTIN,HIGH);

  pinMode(PIN_LED_GREEN,OUTPUT);
  digitalWrite(PIN_LED_GREEN,HIGH);

  Wire.begin();

  clearAll(leds_bas);
  clearAll(leds_haut);
  //clearAll(leds_dessus);
  FastLED.show();

  //animDemarrage(leds_dessus);
  //animDemarrage(leds_haut);
  //animDemarrage(leds_bas);

  clearAll(leds_bas);
  clearAll(leds_haut);
  //clearAll(leds_dessus);
  FastLED.show();

  g_mode=EEPROM.read(0);
  Serial.print("Mode ");
  Serial.println(g_mode);

  rtc.begin();

#ifdef SET_TIME
  Serial.print("Set datetime ");
  Serial.print(SET_DATE_STR);
  Serial.print(" ");
  Serial.println(SET_TIME_STR);

  int year, month, day, hour, minute, second;
  sscanf(SET_DATE_STR, "%02d/%02d/%04d", &day, &month, &year);
  sscanf(SET_TIME_STR, "%02d:%02d:%02d", &hour, &minute, &second);
  DateTime newDte(year, month, day, hour, minute, second);
  rtc.adjust(newDte);
#endif

  char tmp[22];
  DateTime now = rtc.now();
  sprintf(tmp, "%02d/%02d/%04d %02d:%02d:%02d", now.day() , now.month(), now.year(), now.hour(), now.minute(), now.second());
  Serial.println(tmp);

  delay(500);
  digitalWrite(PIN_LED_GREEN,LOW);

  /// Si RTC non reglee, on bloque en defaut au demarrage !
  if (now.year()<2024)
  {
    Serial.println("Date incorrecte !!!");
    while (1)
    {
      digitalWrite(PIN_LED_GREEN,HIGH);
      delay(100);
      digitalWrite(PIN_LED_GREEN,LOW);
      delay(100);
    }
  }

  g_t0=millis();
}

/**
 * @brief Lecture de l'heure
 * @param[out] hour Heure
 * @param[out] minute Minute
 * @param[out] second Seconde
*/
void getTime(int &hour,int &minute, int &second)
{
  DateTime now = rtc.now();
  hour=now.hour();
  minute=now.minute();
  second=now.second();
  return 0;
}

/**
 * @brief Retourne la puissance des LEDs en fonction de l'heure
 * @param[in] h Heure
 * @param[in] m Minute
 * @return -1 signifie OFF et 255 à fond
*/
int getPower(int h,int m)
{
  if ( (h>=5) && (h<=23) )
  {
    return 255;
  }
  else if (h==5)
  {
    int res=m*255/59;
    return (res&0xFF);
  }
  else if (h==23)
  {
    int res=(59-m)*255/60;
    return (res&0xFF);
  }
  else
  {
    return -1;
  }
}

/**
 * @brief Gestion du mode intelligent (en fonction de l'heure pour simuler aube et crépuscule)
*/
void manageMode_smart(void)
{
  if ( g_power==-1 )
  {
    clearAll();
    digitalWrite(PIN_POWER_5V,LOW);
    digitalWrite(PIN_CMD_P1,LOW);
    digitalWrite(PIN_CMD_P2,LOW);
    digitalWrite(PIN_CMD_P3,LOW);
    FastLED.show();
  }
  else
  {
    digitalWrite(PIN_POWER_5V,HIGH);
    FastLED.setBrightness(g_power);
    FastLED.show();

    if (g_power==255)
    {
      digitalWrite(PIN_CMD_P1,HIGH);
      digitalWrite(PIN_CMD_P2,HIGH);
      digitalWrite(PIN_CMD_P3,HIGH);
    }
    else
    {
      digitalWrite(PIN_CMD_P1,LOW);
      digitalWrite(PIN_CMD_P2,LOW);
      digitalWrite(PIN_CMD_P3,LOW);
    }
  }  
}

/**
 * @brief Tache du SEMUINO
*/
void taskSemuino(void)
{
  int h,m,s;

  getTime(h,m,s);
  g_power=getPower(h,m);
  
  switch (g_mode)
  {
    case 0:   /// All OFF
    default:
    {
      g_mode=0;
      clearAll(leds_bas);
      clearAll(leds_haut);
      //clearAll(leds_dessus);
      FastLED.setBrightness(0);
      FastLED.show();  
      digitalWrite(PIN_POWER_5V,LOW);
      
      digitalWrite(PIN_CMD_P1,LOW);
      digitalWrite(PIN_CMD_P2,LOW);
      digitalWrite(PIN_CMD_P3,LOW);
      break;
    }
    case 1: /// Only 12V LEDs ON
    {
      clearAll(leds_bas);
      clearAll(leds_haut);
      FastLED.setBrightness(0);
      FastLED.show();  
      digitalWrite(PIN_POWER_5V,LOW);
      
      digitalWrite(PIN_CMD_P1,HIGH);
      digitalWrite(PIN_CMD_P2,HIGH);
      digitalWrite(PIN_CMD_P3,HIGH);
      
      break;
    }
    case 2: ///< Smart mode
    {
      manageMode_smart();
      break;
    }
  }  
}

/**
 * @brief Detection de la pression sur l'interrupteur de selection du mode
 * @return true si presse
*/
bool inputInterrupteur()
{
  if (digitalRead(PIN_SELECT_BTN)==0)
  {
    digitalWrite(PIN_LED_GREEN,HIGH);

    long t0=millis();
    while (digitalRead(PIN_SELECT_BTN)==0)
    {
      long t=millis();
      if (t-t0>5000)
      {
        for (int i=0;i<10;i++)
        {
          digitalWrite(PIN_LED_GREEN,LOW);
          delay(100);
          digitalWrite(PIN_LED_GREEN,HIGH);
          delay(100);
        }

        digitalWrite(PIN_LED_GREEN,LOW);
        return true;
      }
    }
    
    g_mode=g_mode+1;
    if (g_mode>MAX_MODE)
      g_mode=0;

    Serial.print("Mode: ");
    Serial.println(g_mode);
    delay(500);
    digitalWrite(PIN_LED_GREEN,LOW);

    EEPROM.write(0,g_mode);
    taskSemuino();

    /// Indiquer le mode en clignotant n fois
    for (int i=0;i<g_mode;i++)
    {
      delay(300);
      digitalWrite(PIN_LED_GREEN,HIGH);
      delay(300);
      digitalWrite(PIN_LED_GREEN,LOW);      
    }
    delay(1500);
    
    return true;
  }  
  
  return false;
}

/**
 * @brief BOUCLE PRINCIPALE ARDUINO
*/
void loop() 
{
  unsigned long delta=0;
  unsigned long t=millis();
  if (t>=g_t0)
    delta=t-g_t0;
  else
    delta=0xFFFFFFFF-g_t0+t;
  
  if (delta>=1000)
  {
    taskSemuino();
    
    g_blink_1s=!g_blink_1s;    
    if (g_blink_1s==true)
      g_blink_2s=!g_blink_2s;
    g_t0=millis();
  }

  delay(50);

  inputInterrupteur();

  /*int tmp=analogRead(PIN_HUM3);
  if (tmp>700)
    digitalWrite(PIN_LED_GREEN,g_blink_2s?HIGH:LOW);
  else
    digitalWrite(PIN_LED_GREEN,LOW);

  Serial.print("Hum:");
  Serial.println(tmp);*/
}
