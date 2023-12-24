/**
 * @brief semuino_test.ino
 * 
 * SEMUINO - Programme de test autonome de la carte semuino
 * 
 * Compatible avec Carte semuino Rev A2
**/
#include "FastLED.h"
#include <EEPROM.h>
#include <DHT.h>
#include "Wire.h"
#include <RTClib.h>

//#define SET_TIME
#define SET_DATE_STR "23/12/2023"
#define SET_TIME_STR __TIME__


#define POWER_MAX         255
#define POWER_MIN         10

#define NUM_LEDS          (23*7)  ///< Nombre de LEDs a chaque etage (3 rangées de 23 LEDs)

#define PIN_SELECT_BTN      (2)   ///< Interrupteur de selection

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
#define COL_RED             CRGB(127,0,0)       ///< Rouge
#define COL_GREEN           CRGB(0,127,0)       ///< Vert
#define COL_BLUE            CRGB(0,0,127)       ///< Bleu

#define MAX_MODE  10        ///< Nombre de modes
unsigned char mode=0;       ///< Mode en cours

int g_power=255;              ///< Puissance
CRGB leds_bas[NUM_LEDS];      ///< Tableau des LEDs du bas
CRGB leds_haut[NUM_LEDS];     ///< Tableau des LEDs du haut
//CRGB leds_dessus[NUM_LEDS];   ///< Tableau des LEDs du dessus du meuble

DS1307 rtc;
DHT dht(PIN_TEMP, DHT22);

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

  Wire.begin();
  rtc.begin();
  dht.begin();

  #ifdef SET_TIME
    Serial.print("Set datetime ");
    Serial.print(SET_DATE_STR);
    Serial.print(" ");
    Serial.println(SET_TIME_STR);
    
    int year,month,day,hour,minute,second;
    sscanf(SET_DATE_STR,"%02d/%02d/%04d",&day,&month,&year);
    sscanf(SET_TIME_STR,"%02d:%02d:%02d",&hour,&minute,&second);
    DateTime newDte(year,month,day,hour,minute,second);
    rtc.adjust(newDte);
  #endif

  char tmp[30];
  DateTime now = rtc.now();
  sprintf(tmp, "%02d/%02d/%04d %02d:%02d:%02d",now.day() , now.month(), now.year(), now.hour(), now.minute(), now.second());  
  Serial.println(tmp);

  clearAll(leds_bas);
  clearAll(leds_haut);
  //clearAll(leds_dessus);
  FastLED.show();

  mode=EEPROM.read(0);
  
  delay(3000);   
}

void test_dht22(void)
{
  float tauxHumidite = dht.readHumidity();              // Lecture du taux d'humidité (en %)
  float temperatureEnCelsius = dht.readTemperature();   // Lecture de la température, exprimée en degrés Celsius

  // Vérification si données bien reçues
  if (isnan(tauxHumidite) || isnan(temperatureEnCelsius))
  {
    Serial.println("Aucune valeur retournée par le DHT22. Est-il bien branché ?");
    delay(2000);
    return;
  }

  // Calcul de la température ressentie
  float temperatureRessentieEnCelsius = dht.computeHeatIndex(temperatureEnCelsius, tauxHumidite, false); // Le "false" est là pour dire qu'on travaille en °C, et non en °F
  
  // Affichage des valeurs
  Serial.print("Humidité = "); Serial.print(tauxHumidite); Serial.println(" %");
  Serial.print("Température = "); Serial.print(temperatureEnCelsius); Serial.println(" °C");
  Serial.print("Température ressentie = "); Serial.print(temperatureRessentieEnCelsius); Serial.println(" °C");
  Serial.println();  
}

void test_hum(void)
{
  int hum=analogRead(PIN_HUM1);
  Serial.print("Humidité 1:");
  Serial.println(hum);

  hum=analogRead(PIN_HUM2);
  Serial.print("Humidité 2:");
  Serial.println(hum);
  
  hum=analogRead(PIN_HUM3);
  Serial.print("Humidité 3:");
  Serial.println(hum);
}

/**
 * @brief BOUCLE PRINCIPALE ARDUINO
*/
void loop() 
{   
  test_hum();
  delay(1000);

  /*test_dht22();
  delay(1000);*/

  Serial.println("Allumage 5V");
  digitalWrite(PIN_POWER_5V,HIGH);
  delay(2000);

  Serial.println("LED RGB 1 RED");
  setAll(leds_bas,COL_RED);
  FastLED.show();  
  delay(2000);

  Serial.println("LED RGB 1 GREEN");
  setAll(leds_bas,COL_GREEN);
  FastLED.show();  
  delay(2000);

  Serial.println("LED RGB 1 BLUE");
  setAll(leds_bas,COL_BLUE);
  FastLED.show();  
  delay(2000);

  Serial.println("LED RGB 1 GROWING");
  setAll(leds_bas,COL_GROWING);
  FastLED.setBrightness(g_power);
  FastLED.show();  
  delay(2000);

  Serial.println("LED RGB 1 WHITE");
  setAll(leds_bas,COL_WHITE);
  FastLED.show();  
  delay(2000);
  clearAll(leds_bas);
  FastLED.show();  

  Serial.println("Coupure 5V");
  digitalWrite(PIN_POWER_5V,LOW);
  delay(4000);

  Serial.println("ALLUMAGE LED P1 12V");
  digitalWrite(PIN_CMD_P1,HIGH);
  delay(4000);
  Serial.println("COUPURE LED P1 12V");
  digitalWrite(PIN_CMD_P1,LOW);
  delay(4000);
}
