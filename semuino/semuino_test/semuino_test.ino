/**
   @brief semuino_test.ino

   SEMUINO - Programme de test autonome de la carte semuino

   Compatible avec Carte semuino Rev A2
**/
#include "FastLED.h"
#include <EEPROM.h>
#include <DHT.h>
#include "Wire.h"
#include <RTClib.h>

#define TEST_WITH_SERIAL

//#define SET_TIME
#define SET_DATE_STR "20/01/2024"
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
unsigned char mode = 0;     ///< Mode en cours

int g_power = 255;            ///< Puissance
CRGB leds_bas[NUM_LEDS];      ///< Tableau des LEDs du bas
CRGB leds_haut[NUM_LEDS];     ///< Tableau des LEDs du haut
//CRGB leds_dessus[NUM_LEDS];   ///< Tableau des LEDs du dessus du meuble

DS1307 rtc;
DHT dht(PIN_TEMP, DHT22);

unsigned long t0 = 0;

/**
   @brief Applique une couleur a tout un etage
   @param pLeds Tableau de LEDs de l'etage
   @param col Couleur
*/
void setAll(CRGB *pLeds, CRGB col)
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    pLeds[i] = col;
  }
}

/**
   @brief Applique une couleur a tous les etages en meme temps
   @param col Couleur
*/
void setAll(CRGB col)
{
  setAll(leds_bas, col);
  setAll(leds_haut, col);
  //setAll(leds_dessus,col);
}

/**
   @brief Eteint tout un etage
   @param pLeds Tableau de LEDs de l'etage
*/
void clearAll(CRGB *pLeds)
{
  setAll(pLeds, CRGB::Black);
}

/**
   @brief Eteint tous les etages
*/
void clearAll()
{
  clearAll(leds_bas);
  clearAll(leds_haut);
  //clearAll(leds_dessus);
}

/**
   @brief Setup d'arduino (demarage)

   Une petite animation avec test de toutes les LEDs
*/
void setup()
{
  randomSeed(millis());

  g_power = POWER_MAX;

  Serial.begin(9600);
  Serial.println("Boot");

  pinMode(PIN_DATA_LED_BAS, OUTPUT);
  digitalWrite(PIN_DATA_LED_BAS, LOW);
  pinMode(PIN_DATA_LED_HAUT, OUTPUT);
  digitalWrite(PIN_DATA_LED_HAUT, LOW);
  pinMode(PIN_DATA_LED_DESSUS, OUTPUT);
  digitalWrite(PIN_DATA_LED_DESSUS, LOW);

  FastLED.addLeds<NEOPIXEL, PIN_DATA_LED_BAS>(leds_bas, NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, PIN_DATA_LED_HAUT>(leds_haut, NUM_LEDS);
  //FastLED.addLeds<NEOPIXEL, PIN_DATA_LED_DESSUS>(leds_dessus, NUM_LEDS);
  FastLED.setBrightness(g_power);

  pinMode(PIN_POWER_5V, OUTPUT);
  digitalWrite(PIN_POWER_5V, LOW);

  pinMode(PIN_CMD_P1, OUTPUT);
  digitalWrite(PIN_CMD_P1, LOW);
  pinMode(PIN_CMD_P2, OUTPUT);
  digitalWrite(PIN_CMD_P2, LOW);
  pinMode(PIN_CMD_P3, OUTPUT);
  digitalWrite(PIN_CMD_P3, LOW);
  pinMode(PIN_TEMP, OUTPUT);
  digitalWrite(PIN_TEMP, LOW);

  pinMode(PIN_SELECT_BTN, INPUT_PULLUP);
  pinMode(PIN_HUM1, INPUT);
  pinMode(PIN_HUM2, INPUT);
  pinMode(PIN_HUM3, INPUT);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  Wire.begin();
  rtc.begin();
  dht.begin();

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

  char tmp[30];
  DateTime now = rtc.now();
  sprintf(tmp, "%02d/%02d/%04d %02d:%02d:%02d", now.day() , now.month(), now.year(), now.hour(), now.minute(), now.second());
  Serial.println(tmp);

  clearAll(leds_bas);
  clearAll(leds_haut);
  //clearAll(leds_dessus);
  FastLED.show();

  mode = EEPROM.read(0);

  t0 = millis();
  Serial.println("ok");
}

unsigned long delta(unsigned long lT0)
{
  unsigned long t = millis();
  unsigned long delt = 0;
  if (t >= lT0)
    delt = t - lT0;
  else
    delt = 0xFFFFFFFF - lT0 + t;

  return delt;
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
  char tmp[15];
  
  int hum = analogRead(PIN_HUM1);  
  Serial.print("H1:");
  sprintf(tmp,"%d",hum);
  Serial.println(tmp);

  hum = analogRead(PIN_HUM2);
  Serial.print("H2:");
  sprintf(tmp,"%d",hum);
  Serial.println(tmp);

  hum = analogRead(PIN_HUM3);
  Serial.print("H3:");
  sprintf(tmp,"%d",hum);
  Serial.println(tmp);
}

bool enable_temp = false;

typedef struct cmdPin
{
  char name[10];
  char cmd;
  int pin;
  int state;
} T_CMD_PIN;


const T_CMD_PIN cmds[] =
{
  {"P1", (char)'a', PIN_CMD_P1, HIGH},
  {"P1", (char)'q', PIN_CMD_P1, LOW},
  {"P2", (char)'z', PIN_CMD_P2, HIGH},
  {"P2", (char)'s', PIN_CMD_P2, LOW},
  {"P3", (char)'e', PIN_CMD_P3, HIGH},
  {"P3", (char)'d', PIN_CMD_P3, LOW},
  {"POWER 5V", (char)'r', PIN_POWER_5V, HIGH},
  {"POWER 5V", (char)'f', PIN_POWER_5V, LOW}
};

typedef struct
{
  char name[20];
  char cmd;
  CRGB *leds;
  CRGB *col;
} T_CMD_RGB;

const CRGB black(0, 0, 0);
const CRGB green(0, 127, 0);
const CRGB red(127, 0, 0);
const CRGB white(127, 127, 127);

const T_CMD_RGB rgb_cmds[] =
{
  {"RGB 1 BLACK", (char)'1', leds_bas, &black},
  {"RGB 1 RED", (char)'4', leds_bas, &red},
  {"RGB 1 WHITE", (char)'7', leds_bas, &white},
  {"RGB 2 BLACK", (char)'2', leds_haut, &black},
  {"RGB 2 RED", (char)'5', leds_haut, &red},
  {"RGB 2 WHITE", (char)'8', leds_haut, &white}
  /*{"RGB 3 BLACK",'1',leds_bas,&black},
    {"RGB 3 RED",'4',leds_bas,&red},
    {"RGB 3 GREEN",'7',leds_bas,&green},*/
};

void serialEvent()
{
  int b = Serial.read();
  if ( (b < 0) || (b == 10) || (b == 13) || (b > 127))
    return;

  char c = (char)b;
  for (int i = 0; i < sizeof(cmds) / sizeof(T_CMD_PIN); i++)
  {
    if (cmds[i].cmd == c)
    {
      char msg[20];
      sprintf(msg, "%s= %d", cmds[i].name, cmds[i].state);
      Serial.println(msg);
      digitalWrite(cmds[i].pin, cmds[i].state);
      return;
    }
  }

  for (int i = 0; i < sizeof(rgb_cmds) / sizeof(T_CMD_RGB); i++)
  {
    Serial.print(i);
    Serial.print(" ");
    Serial.print(c);
    Serial.print(" ");
    Serial.println(rgb_cmds[i].cmd);

    if (rgb_cmds[i].cmd == c)
    {
      Serial.println(rgb_cmds[i].name);
      setAll(rgb_cmds[i].leds, *(rgb_cmds[i].col));
      return;
    }
  }

  if (b == '0')
  {
    Serial.println("CLEAR ALL");

    for (int i = 0; i < sizeof(cmds) / sizeof(T_CMD_PIN); i++)
      digitalWrite(cmds[i].pin, LOW);

    clearAll(leds_bas);
    clearAll(leds_haut);
    //clearAll(leds_dessus);
  }
  else if (b == '-')
  {
    g_power -= 10;
    if (g_power < 0)
      g_power = 0;

    Serial.print("Power: ");
    Serial.println(g_power);
  }
  else if (b == '+')
  {
    g_power += 10;
    if (g_power > 255)
      g_power = 255;
    Serial.print("Power: ");
    Serial.println(g_power);
  }
  else if (b=='h')
  {
    test_hum();    
  }
  else if (b=='t')
  {
    test_dht22();
  }
}

/**
   @brief BOUCLE PRINCIPALE ARDUINO
*/
void loop()
{
  if (delta(t0) > 1000)
  {
    //test_hum();
    t0 = millis();
  }
  delay(100);
  FastLED.show();

  /*test_dht22();
    delay(1000);*/

#ifndef TEST_WITH_SERIAL
  Serial.println("Allumage 5V");
  digitalWrite(PIN_POWER_5V, HIGH);
  delay(2000);
  
  Serial.println("LED RGB 1 RED");
  setAll(leds_bas, COL_RED);
  FastLED.show();
  delay(2000);

  Serial.println("LED RGB 1 GREEN");
  setAll(leds_bas, COL_GREEN);
  FastLED.show();
  delay(2000);

  Serial.println("LED RGB 1 BLUE");
  setAll(leds_bas, COL_BLUE);
  FastLED.show();
  delay(2000);

  Serial.println("LED RGB 1 GROWING");
  setAll(leds_bas, COL_GROWING);
  FastLED.setBrightness(g_power);
  FastLED.show();
  delay(2000);

  Serial.println("LED RGB 1 WHITE");
  setAll(leds_bas, COL_WHITE);
  FastLED.show();
  delay(2000);
  clearAll(leds_bas);
  FastLED.show();

  Serial.println("Coupure 5V");
  digitalWrite(PIN_POWER_5V, LOW);
  delay(4000);

  Serial.println("ALLUMAGE LED P1 12V");
  digitalWrite(PIN_CMD_P1, HIGH);
  delay(4000);
  Serial.println("COUPURE LED P1 12V");
  digitalWrite(PIN_CMD_P1, LOW);
  delay(4000);
#endif
}
