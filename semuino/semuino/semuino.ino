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

#define VERSION "V1.1"

//#define SET_TIME
#define SET_TIME_OFFSET (1) ///< 1 heure en hiver et 2 heures en été
#define SET_DATE_STR "15/02/2025"
#define SET_TIME_STR __TIME__

#define START_HOUR  (6)  ///< Heure de la montee en lumiere (au max au bout d'1h)
#define END_HOUR    (21)  ///< Heure de debut de la baisse de luminosite (OFF au bout d'1h) 

#define POWER_MAX         255
#define POWER_MIN         0
#define BOOST_POWER_MAX   255

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

#define MAX_MODE  4         ///< Nombre de modes
DS1307 rtc;
unsigned long g_t0=0;
bool g_blink_1s=false;
bool g_blink_2s=false;

#define MAGIC1  (0x55)
#define MAGIC2  (0xAA)
typedef struct
{
  unsigned char magic1;
  unsigned char mode;
  unsigned char states;
  unsigned char magic2;
} T_CONFIG;

bool g_mode_config=false;
T_CONFIG g_config=
{
  MAGIC1,
  0,
  0x07,
  MAGIC2
};

#define ST_LED_P1   (0x01)
#define ST_LED_P2   (0x02)
#define ST_LED_P3   (0x04)

int g_power=POWER_MAX;        ///< Puissance
bool g_boost=false;           ///< BOOST (au milieu de la journée)
CRGB leds_bas[NUM_LEDS];      ///< Tableau des LEDs du bas
CRGB leds_haut[NUM_LEDS];     ///< Tableau des LEDs du haut
//CRGB leds_dessus[NUM_LEDS]; ///< Tableau des LEDs du dessus du meuble

/**
 * @brief Sauvegarde la configuration en EEPROM
 * 
 * - mode
 * - etat des LEDs ouhaité lors du fonctionnement
*/
void save_config(void)
{
  EEPROM.write(0,g_config.magic1);
  EEPROM.write(1,g_config.mode);
  EEPROM.write(2,g_config.states);
  EEPROM.write(3,g_config.magic2);  
}

/**
 * @brief Chargement de la configuration en EEPROM et reinit en cas d'incohérence
 * 
 * - mode
 * - etat des LEDs ouhaité lors du fonctionnement
*/
void load_config(void)
{
  g_config.magic1=EEPROM.read(0);
  g_config.mode=EEPROM.read(1);
  g_config.states=EEPROM.read(2);
  g_config.magic2=EEPROM.read(3);

  if ( (g_config.magic1!=MAGIC1) || (g_config.magic2!=MAGIC2) || (g_config.mode>MAX_MODE) )
  {
    g_config.magic1=MAGIC1;
    g_config.mode=0;
    g_config.states=0;
    g_config.magic2=MAGIC2;
    
    save_config();  
  }
}

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

// Algorithme de Zeller pour trouver le jour de la semaine (0 = samedi, 1 = dimanche, ...)
int getWeekday(int year, int month, int day)
{
  if (month < 3) 
  {
    month += 12;
    year--;
  }
  int K = year % 100;
  int J = year / 100;
  int weekday = (day + (13 * (month + 1)) / 5 + K + (K / 4) + (J / 4) + 5 * J) % 7;
  return (weekday + 6) % 7; // Convertir en format (0 = dimanche, 1 = lundi, ...)
}

int getLastSunday(int year, int month)
{
  int daysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

  // Vérifier si l'année est bissextile
  if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
  {
    daysInMonth[1] = 29; // Février = 29 jours
  }

  int lastDay = daysInMonth[month - 1]; // Dernier jour du mois
  int dayOfWeek = (lastDay + getWeekday(year, month, 1) - 1) % 7; // Jour de la semaine du dernier jour

  return lastDay - dayOfWeek;
}

int isDST(int day, int month, int year, int hour)
{
  int last_sunday_march = getLastSunday(year, 3);
  int last_sunday_october = getLastSunday(year, 10);

  if (month > 3 && month < 10) return 1;
  if (month < 3 || month > 10) return 0;

  if (month == 3) {
    if (day > last_sunday_march || (day == last_sunday_march && hour >= 2)) return 1;
    return 0;
  }

  if (month == 10) {
    if (day > last_sunday_october || (day == last_sunday_october && hour >= 2)) return 0;
    return 1;
  }

  return 0;
}

void convertToLocalTime(int &year,int &month,int &day,int &hour)
{  
  int offset = 1;
  if (isDST(day, month, year, hour))
  {
    offset = 2;
  }

  hour += offset;
  if (hour >= 24) 
  {
    hour -= 24;
    day += 1;

    int days_in_month[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) 
    {
      days_in_month[1] = 29;
    }
    if (day > days_in_month[month - 1])
    {
      day = 1;
      month += 1;
      if (month > 12) 
      {
        month = 1;
        year += 1;
      }
    }
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

void display_mode(int mode)
{
  for (int i=0;i<mode;i++)
  {
    delay(300);
    digitalWrite(PIN_LED_GREEN,HIGH);
    delay(300);
    digitalWrite(PIN_LED_GREEN,LOW);
  }
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
  g_boost=false;
  
  Serial.begin(9600);
  Serial.println("Boot...");  
  Serial.print("Standalone ");
  Serial.println(VERSION);

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

  load_config();
  Serial.print("Mode: ");
  Serial.println(g_config.mode);
  Serial.print("States: ");
  Serial.println(g_config.states,HEX);

  rtc.begin();

#ifdef SET_TIME
  Serial.print("Set datetime ");
  Serial.print(SET_DATE_STR);
  Serial.print(" ");
  Serial.println(SET_TIME_STR);

  int year2, month2, day2, hour2, minute2, second2;
  sscanf(SET_DATE_STR, "%02d/%02d/%04d", &day2, &month2, &year2);
  sscanf(SET_TIME_STR, "%02d:%02d:%02d", &hour2, &minute2, &second2);
  DateTime newDte(year2, month2, day2, hour2-SET_TIME_OFFSET, minute2, second2);
  rtc.adjust(newDte);
#endif

  char tmp[22];
  DateTime now = rtc.now();  
  sprintf(tmp, "%02d/%02d/%04d %02d:%02d:%02d", now.day() , now.month(), now.year(), now.hour(), now.minute(), now.second());
  Serial.println(tmp);

  int year=now.year();
  int month=now.month();
  int day=now.day();
  int hour=now.hour();
  convertToLocalTime(year,month,day,hour);
  sprintf(tmp, "%02d/%02d/%04d %02d:%02d:%02d", day , month, year, hour, now.minute(), now.second());
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

  display_mode(g_config.mode);
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
  DateTime nowUTC = rtc.now();
  int year=nowUTC.year();
  int month=nowUTC.month();
  int day=nowUTC.day();
    
  hour=nowUTC.hour();
  minute=nowUTC.minute();
  second=nowUTC.second();
  
  convertToLocalTime(year,month,day,hour);

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
  if (h==START_HOUR)
  {    
    //int res=m*255/59;
    int res=map(m,0,59,POWER_MIN,POWER_MAX);
    return (res&0xFF);
  }
  else if (h==END_HOUR)
  {
    //int res=(59-m)*255/60;
    int res=map(59-m,0,59,POWER_MIN,POWER_MAX);
    return (res&0xFF);
  }
  else if ( (h>START_HOUR) && (h<END_HOUR) )
  {
    return POWER_MAX;
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
    /// @remark Si power -1, tout est coupé (c'est la nuit)
    clearAll();
    digitalWrite(PIN_POWER_5V,LOW);
    digitalWrite(PIN_CMD_P1,LOW);
    digitalWrite(PIN_CMD_P2,LOW);
    digitalWrite(PIN_CMD_P3,LOW);
    clearAll(leds_haut);
    clearAll(leds_bas);
    FastLED.show();
  }
  else
  {
    /// @remark Sinon on alimente le RGB 5V
    digitalWrite(PIN_POWER_5V,HIGH);
    
    if (g_power==POWER_MAX)
    {
      /// @remark Au maximum, on allume systematiquement les LEDs 12V choisies
      digitalWrite(PIN_CMD_P1,(g_config.states&ST_LED_P1)==ST_LED_P1?HIGH:LOW);
      digitalWrite(PIN_CMD_P2,(g_config.states&ST_LED_P2)==ST_LED_P2?HIGH:LOW);
      digitalWrite(PIN_CMD_P3,(g_config.states&ST_LED_P3)==ST_LED_P3?HIGH:LOW);

      /// @remark, lors du boost, on met en blanc les deux étages RGBs choisis et à la puissance max du boost sinon en rouge horticole à la puissance actuelle
      if ((g_config.states&ST_LED_P1)==ST_LED_P1)           
        setAll(leds_bas,g_boost==true?COL_WHITE:COL_GROWING);
      else
        clearAll(leds_bas);
                 
      if ((g_config.states&ST_LED_P2)==ST_LED_P2)
        setAll(leds_haut,g_boost==true?COL_WHITE:COL_GROWING);
      else
        clearAll(leds_haut);

      FastLED.setBrightness(g_boost==true?BOOST_POWER_MAX:g_power);            
      FastLED.show();
    }
    else
    {
      /// @remark Lors des montées et descente, on éteint les 12V et on applique la luminosité des RGBs souhaitee
      digitalWrite(PIN_CMD_P1,LOW);
      digitalWrite(PIN_CMD_P2,LOW);
      digitalWrite(PIN_CMD_P3,LOW);

      if ((g_config.states&ST_LED_P1)==ST_LED_P1)
        setAll(leds_bas,COL_WHITE);
      else
        clearAll(leds_bas);
        
      if ((g_config.states&ST_LED_P2)==ST_LED_P2)
        setAll(leds_haut,COL_WHITE);
      else
        clearAll(leds_haut);

      FastLED.setBrightness(g_power);
      FastLED.show();
    }
  }  
}

/**
 * @brief Tache de config
*/
void taskConfig(void)
{  
  digitalWrite(PIN_CMD_P1,(g_config.states&ST_LED_P1)==ST_LED_P1?HIGH:LOW);
  digitalWrite(PIN_CMD_P2,(g_config.states&ST_LED_P2)==ST_LED_P2?HIGH:LOW);
  digitalWrite(PIN_CMD_P3,(g_config.states&ST_LED_P3)==ST_LED_P3?HIGH:LOW);
}

/**
 * @brief Tache du SEMUINO
*/
void taskSemuino(void)
{
  int h,m,s;

  getTime(h,m,s);
  g_power=getPower(h,m);

  // Periode de BOOST... (LEDs RGB blanches au lieu de Rouge horticole)
  if ( (h>=12) && (h<=13) )
    g_boost=true;
  else
    g_boost=false;

  Serial.print("Time: ");
  Serial.print(h);
  Serial.print(":");
  Serial.print(m);
  Serial.print("; mode: ");
  Serial.print(g_config.mode);
  Serial.print("; power: ");
  Serial.println(g_power);
  
  switch (g_config.mode)
  {
    case 0:   /// All OFF
    default:
    {
      g_config.mode=0;
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
    case 1: ///< Smart mode
    {
      manageMode_smart();
      break;
    }
    case 2: /// Manual mode
    {
      clearAll(leds_bas);
      clearAll(leds_haut);
      FastLED.setBrightness(0);
      FastLED.show();  
      digitalWrite(PIN_POWER_5V,LOW);
      
      digitalWrite(PIN_CMD_P1,(g_config.states&ST_LED_P1)==ST_LED_P1?HIGH:LOW);
      digitalWrite(PIN_CMD_P2,(g_config.states&ST_LED_P2)==ST_LED_P2?HIGH:LOW);
      digitalWrite(PIN_CMD_P3,(g_config.states&ST_LED_P3)==ST_LED_P3?HIGH:LOW);
      
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
      if (t-t0>4500)
      {
        if (g_mode_config==false)
        {
          for (int i=0;i<10;i++)
          {
            delay(100);
            digitalWrite(PIN_LED_GREEN,LOW);
            delay(100);
            digitalWrite(PIN_LED_GREEN,HIGH);
          }
          g_mode_config=true;
        }
        else
        {
          save_config();
          g_mode_config=false;
          digitalWrite(PIN_LED_GREEN,LOW);
          while (digitalRead(PIN_SELECT_BTN)==0)
            delay(500);
        }
  
        return true;
      }
    }
    
    if (g_mode_config==false)
    {
      g_config.mode=g_config.mode+1;
      if (g_config.mode>MAX_MODE)
        g_config.mode=0;
  
      Serial.print("Mode: ");
      Serial.println(g_config.mode);
      Serial.print("States: ");
      Serial.println(g_config.states,HEX);
      delay(500);
      digitalWrite(PIN_LED_GREEN,LOW);
  
      // On sauvegarde la config pour les modes 0, 1 et 2 seulement
      save_config();
  
      taskSemuino();
  
      /// Indiquer le mode en clignotant n fois
      display_mode(g_config.mode);
      delay(1500);
    }
    else
    {
      g_config.states=g_config.states+1;
      if (g_config.states>0x07)
        g_config.states=0;
    }
    
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

  if (g_mode_config==false)
  {
    if (delta>=1000)
    {
      taskSemuino();
      
      g_blink_1s=!g_blink_1s;    
      if (g_blink_1s==true)
        g_blink_2s=!g_blink_2s;
      g_t0=millis();
    }
  }
  else
  {
    taskConfig();
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

/*inline void _term_exec_time()
{
  DateTime now=rtc.now();
  Serial.print("UTC:");
  Serial.print(now.day());Serial.print("/");Serial.print(now.month());Serial.print("/");Serial.print(now.year());
  Serial.print(" ");
  Serial.print(now.hour());Serial.print(":");Serial.print(now.minute());Serial.print(":");Serial.println(now.second());  
}

inline void _term_execCmd(const char *strCmd,const char *strParams)
{
  if (strcmp(strCmd,"test")==0)
  {
    Serial.println("TEST OK");
  }
  else if (strcmp(strCmd,"time")==0)
  {
    _term_exec_time();
  }
  else if (strcmp(strCmd,"settime")==0)
  {
    int hour,minute,second;
    if ( sscanf(strParams,"%02d:%02d:%02d",&hour,&minute,&second)==3 )
    {
      DateTime now=rtc.now();
      DateTime newDte(now.year(),now.month(),now.day(),hour, minute,second);
      rtc.adjust(newDte);
      _term_exec_time();
    }
    else
    {
      Serial.println("Paramètre incorrect!");
    }    
  }
  else if (strcmp(strCmd,"setdate")==0)
  {
    int day,month,year;
    if ( sscanf(strParams,"%02d/%02d/%04d",&day,&month,&year)==3 )
    {
      DateTime now=rtc.now();
      DateTime newDte(year,month,day,now.hour(), now.minute(),now.second());
      rtc.adjust(newDte);
      _term_exec_time();
    }
    else
    {
      Serial.println("Paramètre incorrect!");
    }    
  }
  else if (strcmp(strCmd,"help")==0)
  {
    Serial.println("Aide:");
    Serial.println("  - help: Cette aide.");
    Serial.println("  - test: Tester la comm.");
    Serial.println("  - setdate <dd/mm/yyyy>: Regler la date UTC.");
    Serial.println("  - settime <HH:MM:SS>: Regler l'heure UTC.");
    Serial.println("_____________________________________________");    
  }
}

void serialEvent(void)
{
  if (Serial.available()>0)
  {
    String st=Serial.readString();
    if (st.length()>20)
      return;

    st.trim();

    int inx=st.indexOf(" ");
    if (inx==-1)
    {
      _term_execCmd(st.c_str(),NULL);
    }
    else
    {
      _term_execCmd(st.substring(0,inx).c_str(),st.substring(inx+1).c_str());
    }
  }
}*/
