/**
 * @brief semuino.ino
 * 
 * SEMUINO - Gestion du meuble à semi
**/
#include "FastLED.h"
#include <EEPROM.h>
#include "Stc.h"

#define POWER_MAX         255
#define POWER_MIN         10

#define NUM_LEDS          (23*7)  ///< Nombre de LEDs a chaque etage (3 rangées de 23 LEDs)

#define PIN_DATA_LED_BAS    (7)   ///< Broche etage du bas
#define PIN_DATA_LED_HAUT   (6)   ///< Broche etage du haut
#define PIN_DATA_LED_DESSUS (8)   ///< Broche lampes au dessus du meuble

#define PIN_GREEN_LED       (2)   ///< LED Verte
#define PIN_SELECT_BTN      (3)   ///< Interrupteur de selection

#define COL_GROWING         CRGB(127,0,32)      ///< Couleur de croissance
#define COL_WHITE           CRGB(127,127,127)   ///< Blanc

#define MAX_MODE  8     ///< Nombre de modes
int mode=0;             ///< Mode en cours

Stc rtc;

int g_power;                  ///< Puissance
CRGB leds_bas[NUM_LEDS];      ///< Tableau des LEDs du bas
CRGB leds_haut[NUM_LEDS];     ///< Tableau des LEDs du haut
CRGB leds_dessus[NUM_LEDS];   ///< Tableau des LEDs du dessus du meuble

/**
 * @brief Applique une couleur a tout un etage
 * @param pLeds Tableau de LEDs de l'etage
 * @param col Couleur
*/
void setAll(CRGB *pLeds,CRGB col)
{
  for (int i=0;i<NUM_LEDS;i++)
  {
     pLeds[i] = col;
  }
}

/**
 * @brief Applique une couleur a tous les etages en meme temps
 * @param col Couleur
*/
void setAll(CRGB col)
{
  setAll(leds_bas,col);
  setAll(leds_haut,col);
  setAll(leds_dessus,col);
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
  clearAll(leds_dessus);
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
 * Une petite animation avec test de toutes les LEDs
*/
void setup() 
{
  randomSeed(millis());
  
  g_power=POWER_MAX;
  
  Serial.begin(9600);
  Serial.println("Boot");

  //rtc.begin();
  //rtc.setTime(6,6,0);
  //rtc.setDate(1,1,2021);

  pinMode(PIN_DATA_LED_BAS, OUTPUT);
  pinMode(PIN_DATA_LED_HAUT, OUTPUT);
  pinMode(PIN_DATA_LED_DESSUS, OUTPUT);

  FastLED.addLeds<NEOPIXEL, PIN_DATA_LED_BAS>(leds_bas, NUM_LEDS); 
  FastLED.addLeds<NEOPIXEL, PIN_DATA_LED_HAUT>(leds_haut, NUM_LEDS); 
  FastLED.addLeds<NEOPIXEL, PIN_DATA_LED_DESSUS>(leds_dessus, NUM_LEDS); 
  FastLED.setBrightness(g_power);

  pinMode(PIN_GREEN_LED, OUTPUT);
  pinMode(PIN_SELECT_BTN, INPUT_PULLUP);
  digitalWrite(PIN_GREEN_LED,HIGH);

  pinMode(LED_BUILTIN,OUTPUT);
  digitalWrite(LED_BUILTIN,HIGH);

  clearAll(leds_bas);
  clearAll(leds_haut);
  clearAll(leds_dessus);
  FastLED.show();

  animDemarrage(leds_dessus);
  animDemarrage(leds_haut);
  animDemarrage(leds_bas);

  clearAll(leds_bas);
  clearAll(leds_haut);
  clearAll(leds_dessus);
  FastLED.show();

  mode=EEPROM.read(0);

  digitalWrite(PIN_GREEN_LED,LOW);
}

/**
 * @brief Detection de la pression sur l'interrupteur de selection du mode
 * @return true si presse
*/
bool inputInterrupteur()
{
  if (digitalRead(PIN_SELECT_BTN)==0)
  {
    digitalWrite(PIN_GREEN_LED,HIGH);

    long t0=millis();
    while (digitalRead(PIN_SELECT_BTN)==0)
    {
      long t=millis();
      if (t-t0>5000)
      {
        rtc.setTime(8,0,0);

        for (int i=0;i<6;i++)
        {
          digitalWrite(PIN_GREEN_LED,LOW);
          delay(100);
          digitalWrite(PIN_GREEN_LED,HIGH);
          delay(100);
        }

        digitalWrite(PIN_GREEN_LED,LOW);
        return true;
      }
    }
    
    mode=mode+1;
    if (mode>MAX_MODE)
      mode=0;

    Serial.print("Mode: ");
    Serial.println(mode);
    delay(500);
    digitalWrite(PIN_GREEN_LED,LOW);

    EEPROM.write(0,mode);
    
    return true;
  }  
  
  return false;
}

/**
 * @brief Delai d'attente avec detection interrupteur
 * @param ms Duree en ms
 * @return true si l'interrupteur a ete presse et l'attente interrompue
*/
bool delay_inp(long ms)
{
  long delta=0;
  long t0=millis();
  do
  {
    long t=millis();
    if (t>t0)
      delta=t-t0;
    else
      delta=0xFFFFFFFF-t0+t;

    if (inputInterrupteur()==true)
      return true;

    delay(10);
  } while (delta<ms);

  return false;
}

/**
 * @brief Calcul de l'eclairage en fonction de l'heure (theorique)
 * 
 * Entre 6 et 8 heure, on augmente lineairement la puissance d'eclerage de POWER_MIN a POWER_MAX
 * Entre 20 et 22 heure, on diminue linerairement la puissance d'eclairage de POWER_MAX vers POWER_MIN
 * 
 * On eteint entre 22 heure et 6 heure.
 * 
 * Le reste du temps, puissance maximum
*/
void updatePower()
{
  static int old=0;

  int h=rtc.getHours();
  int m=rtc.getMinutes();

  if ( (h>=6) && (h<8) )
  {
    int mins=(h-6)*60+m;
    int qte=POWER_MAX-POWER_MIN;

    g_power=mins*qte/120+POWER_MIN;    
  }
  else if ( (h>=20) && (h<22) )
  {
    int mins=120-((h-20)*60+m);
    int qte=POWER_MAX-POWER_MIN;

    g_power=mins*qte/120+POWER_MIN;    
  }
  else if ( (h>=22) || (h<6) )
  {
    g_power=0;
  }
  else
  {
    g_power=POWER_MAX;
  }

  if (g_power>POWER_MAX)
    g_power=POWER_MAX;

  if (g_power<0)
    g_power=0;
    
  if (g_power!=old)
  {
    Serial.print("Time: ");
    Serial.print(h);
    Serial.print(":");
    Serial.print(m);
    Serial.print("  ");
    
    Serial.print("Power: ");
    Serial.println(g_power);
    old=g_power;
  }  
}

/**
 * @brief BOUCLE PRINCIPALE ARDUINO
*/
void loop() 
{
  delay(100); 

  rtc.tick();

  switch (mode)
  {
    case 0:
    default:
    {
      mode=0;
      clearAll(leds_bas);
      clearAll(leds_haut);
      clearAll(leds_dessus);
      break;
    }
    case 1:
    {
      setAll(leds_haut,COL_GROWING);
      clearAll(leds_bas);
      updatePower();
      break;
    }
    case 2:
    {
      setAll(leds_haut,COL_WHITE);
      clearAll(leds_bas);
      updatePower();
      break;
    }
    
    case 3:
    {
      clearAll(leds_haut);
      setAll(leds_bas,COL_GROWING);
      updatePower();
      break;
    }
    case 4:
    {
      clearAll(leds_haut);
      setAll(leds_bas,COL_WHITE);
      updatePower();
      break;
    }

    case 5:
    {
      setAll(leds_haut,COL_GROWING);
      setAll(leds_bas,COL_WHITE);
      updatePower();
      break;
    }
    case 6:
    {
      setAll(leds_haut,COL_WHITE);
      setAll(leds_bas,COL_WHITE);
      updatePower();
      break;
    }
    case 7: ///< Alternance R G B d'un etage a l'autre (anim pour le fun / demo)
    {
      g_power=POWER_MAX;
      FastLED.setBrightness(g_power);
      
      CRGB col1=CRGB(127,0,0);
      CRGB col2=CRGB(0,127,0);      
      CRGB col3=CRGB(0,0,127);      

      setAll(leds_bas,col1);
      setAll(leds_haut,col2);
      setAll(leds_dessus,col3);
      FastLED.show();

      if (delay_inp(1000)==true)
        break;

      setAll(leds_bas,col3);
      setAll(leds_haut,col1);
      setAll(leds_dessus,col2);
      FastLED.show();

      if (delay_inp(1000)==true)
        break;

      setAll(leds_bas,col2);
      setAll(leds_haut,col3);
      setAll(leds_dessus,col1);
      FastLED.show();

      delay_inp(900);

      break;
    }
    case 8:
    {
      g_power=POWER_MAX;
      setBBR(leds_bas);
      setBBR(leds_haut);
      setBBR(leds_dessus);
      break;
    }
  }
  
  if (g_power==0)
    clearAll();

  FastLED.setBrightness(g_power);
  FastLED.show();

  inputInterrupteur();
}
