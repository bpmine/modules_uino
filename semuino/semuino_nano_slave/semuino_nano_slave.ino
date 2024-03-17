/**
 * @brief semuino.ino
 * 
 * SEMUINO - Gestion du meuble à semi
**/
#include "FastLED.h"
#include <EEPROM.h>
#include <DHT.h>
#include <Wire.h>

#include "i2c_cmn.h"
#include "timer.h"

#define NUM_LEDS          (23*7)  ///< Nombre de LEDs a chaque etage (7 rangées de 23 LEDs)

/**
 * @brief Definition des PINs du Nano
 * @{
*/
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
/**
 * @}
*/

/**
 * @todo Definition des couleurs
 * @{
*/
#define COL_GROWING         CRGB(127,0,32)      ///< Couleur de croissance
#define COL_WHITE           CRGB(127,127,127)   ///< Blanc
#define COL_RED             CRGB(127,0,0)  
#define COL_GREEN           CRGB(0,127,0)  
#define COL_BLUE            CRGB(0,0,127)
/**
 * @}
*/

CRGB leds_bas[NUM_LEDS];              ///< Tableau des LEDs du bas
CRGB leds_haut[NUM_LEDS];             ///< Tableau des LEDs du haut

DHT dht(PIN_TEMP, DHT22);             ///< Gestion du DHT (Non utilise car prend trop de temps CPU avec cette lib)
unsigned char g_alive=0;              ///< Contient la valeur Alive ecrite lors du dernier acces "write"

unsigned char g_reg=0;                ///< Contient la valeur d'ID de registre ecrite au dernier acces "write"
unsigned char g_addr=0;               ///< Contient l'adresse EEP ecrite au dernier acces "write"

unsigned char g_inputs=0;             ///< Etat des entrees lues
unsigned char g_hum_pc=0;             ///< Derniere valeur d'humidité ambiante lue
unsigned char g_temp_dg=0;            ///< Derniere valeur de temperature ambiante lue
unsigned char g_hum1=0;               ///< Derniere valeur lue de capteur d'humidite (bas)
unsigned char g_hum2=0;               ///< Derniere valeur lue de capteur d'humidite (haut)
unsigned char g_hum3=0;               ///< Derniere valeur lue de capteur d'humidite (dessus)

unsigned char g_ctrl=0;               ///< Etat des dernieres commandes reçues
unsigned char g_modeA=0;              ///< Contenu du registre A des modes RGB (RGB1 et RGB2)
unsigned char g_modeB=0;              ///< Contenu du registre A des modes RGB (RGB1 et RGB2)
unsigned char g_level=255;            ///< Niveau de l'éclairage des RGBs [0..255]
unsigned char g_voyants=0;            ///< Etat des voayants

Timer tmrBlink500mS(500,false);       ///< Timer 500ms
Timer tmrAlive(10000);                ///< Timer de detection d'absence du maître (alive)
Timer tmrSelLong(4000);               ///< Timer de detection appui long sur Select

bool g_flgBlink_500ms=false;          ///< Change toutes les 500ms

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
}

/**
 * @brief Reception d'une trame I²C de lecture
 * 
 * Cette fonction est souvent appellee apres receiveEvent
*/
void requestEvent(void) 
{  
    switch (g_reg)
    {
      case REG_ALIVE:
      {
        Wire.write(~g_alive);
        //g_ulLastAliveT0_ms=millis();
        tmrAlive.start();
        break;
      }
      case REG_CTRL:
      {
        Wire.write(g_ctrl);
        break;        
      }
      case REG_INPUTS:
      {
        Wire.write(g_inputs);
        break;        
      }
      case REG_TEMP:
      {
        Wire.write(g_temp_dg);
        break;        
      }
      case REG_HUM:
      {
        Wire.write(g_hum_pc);
        break;        
      }
      case REG_H1:
      {
        Wire.write(g_hum1);
        break;        
      }
      case REG_H2:
      {
        Wire.write(g_hum2);
        break;        
      }
      case REG_H3:
      {
        Wire.write(g_hum3);
        break;        
      }
      case REG_EEP_READ:
      {        
        Wire.write(EEPROM.read(g_addr));
        break;        
      }
    }
}

/**
 * @brief Reception d'une trame I²C d'écriture
 * @param[in] howMany Octets reçus
*/
void receiveEvent(int howMany) 
{
  g_reg=Wire.read();
  if (Wire.available())
  {
    switch (g_reg)
    {
      case REG_ALIVE:
      {
        g_alive=Wire.read();
        //g_ulLastAliveT0_ms=millis();
        tmrAlive.start();
        //Serial.print("Rec alive: ");
        //Serial.println(g_alive,HEX);
        break;
      }
      
      case REG_CTRL:
      {
        g_ctrl=Wire.read();
        //Serial.print("Rec CTRL: ");
        //Serial.println(g_ctrl,HEX);
        break;        
      }
      case REG_LEVEL:
      {
        g_level=Wire.read();
        break;        
      }
      case REG_MODE_RGB_A:
      {
        g_modeA=Wire.read();
        break;        
      }
      case REG_MODE_RGB_B:
      {
        g_modeB=Wire.read();
        break;        
      }
      case REG_EEP_WRITE:
      {
        unsigned char addr=Wire.read();
        unsigned char data=Wire.read();
        EEPROM.write(addr,data);
        break;        
      }
      case REG_VOYANTS:
      {
        g_voyants=Wire.read();
        break;
      }
      case REG_EEP_READ:
      {
        g_addr=Wire.read();
        break;
      }
    }
  }
}


/**
 * @brief Setup d'arduino (Demarage)
*/
void setup() 
{
  Serial.begin(9600);
  Serial.println("Sem. SLV");  

  pinMode(PIN_DATA_LED_BAS, OUTPUT);
  digitalWrite(PIN_DATA_LED_BAS,LOW);
  pinMode(PIN_DATA_LED_HAUT, OUTPUT);
  digitalWrite(PIN_DATA_LED_HAUT,LOW);
  pinMode(PIN_DATA_LED_DESSUS, OUTPUT);
  digitalWrite(PIN_DATA_LED_DESSUS,LOW);

  FastLED.addLeds<NEOPIXEL, PIN_DATA_LED_BAS>(leds_bas, NUM_LEDS); 
  FastLED.addLeds<NEOPIXEL, PIN_DATA_LED_HAUT>(leds_haut, NUM_LEDS); 
  FastLED.setBrightness(g_level);

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

  Wire.begin(ADDR_SLAVE);
  Wire.onRequest(requestEvent);
  Wire.onReceive(receiveEvent);

  dht.begin();

  clearAll();
  FastLED.show();

  g_flgBlink_500ms=false;

  tmrBlink500mS.start();
  tmrAlive.start();
  tmrSelLong.start();
}

/**
 * @brief Mappe la lecture de l'humidite dans un octet
 * @param[in] val Valeur lue dans la plage [0..1023]
 * 
 * @return Valeur dans la plage [0..255]
*/
inline unsigned char _mapHum(int val)
{
  unsigned char mapped=(unsigned char)map(val,0,1023,0,255);
  return mapped;
}

/**
 * @brief Lecture des 3 capteurs d'humidité du sol
*/
void readHumidity(void)
{
  int tmp=analogRead(PIN_HUM1);
  //g_hum1=(unsigned char)(tmp*255/1023);
  g_hum1=_mapHum(tmp);
  
  tmp=analogRead(PIN_HUM2);
  //g_hum2=(unsigned char)(tmp*255/1023);
  g_hum2=_mapHum(tmp);

  tmp=analogRead(PIN_HUM3);
  //g_hum3=(unsigned char)(tmp*255/1023);
  g_hum3=_mapHum(tmp);
}

/**
 * @brief Lecture de la temperature et de l'humidité ambiante
*/
void readDHT(void)
{
  float tauxHumidite = dht.readHumidity();              // Lecture du taux d'humidité (en %)
  if ( (isnan(tauxHumidite)) || (tauxHumidite<0) )
    g_hum_pc=255;
  else
    g_hum_pc=(unsigned char)tauxHumidite;

  float temperatureEnCelsius = dht.readTemperature();              // Lecture du taux d'humidité (en %)
  if ( (isnan(temperatureEnCelsius)) || (temperatureEnCelsius<0) )
    g_temp_dg=255;
  else
    g_temp_dg=(unsigned char)temperatureEnCelsius;  
}

/**
 * @brief Command the RGB LEDs strips with the given mode
 * @param[in] pLeds LEDs array
 * @param[in] mode Mode to apply [0..15] - (0:off, 1:growing, 2:white, 3:mixed)
*/
void set_leds_by_mode(CRGB *pLeds,int mode)
{
  switch (mode)
  {
    case RGB_MOD_GROW:
    {
      setAll(pLeds,COL_GROWING);
      break;
    }
    case RGB_MOD_WHITE:
    {
      setAll(pLeds,COL_WHITE);
      break;
    }
    case RGB_MOD_MIXED:
    {
      for (int i=0;i<NUM_LEDS;i++)
      {
        if ((i%10)==0)
        {
          pLeds[i]=COL_BLUE;
        }
        else if ((i%2)==0)
        {
          pLeds[i]=COL_GROWING;
        }
        else
        {
          pLeds[i]=COL_WHITE;          
        }
      }
      break;
    }
    case RGB_MOD_OFF:
    default:
    {
      clearAll(pLeds);
      break;
    }
  }
}

int cycle=0;

/**
 * @brief BOUCLE PRINCIPALE ARDUINO
*/
void loop() 
{
  if (digitalRead(PIN_SELECT_BTN)==HIGH)
  {
    tmrSelLong.start();
    g_inputs&=~INP_SEL_LONG;
    g_inputs&=~INP_SELECTOR;
  }
  
  if (digitalRead(PIN_SELECT_BTN)==LOW)
  {
    g_inputs|=INP_SELECTOR;

    if (tmrSelLong.isRunning()==false)
    {
      g_inputs|=INP_SEL_LONG;
    }
  }

  readHumidity();
  //readDHT();  ///< Marche pas car bloque trop longtemps

  delay(10);
  
  if (tmrAlive.isRunning()==false)
  {
    g_ctrl=0;
    digitalWrite(LED_BUILTIN,LOW);    
  }
  else
  {
    digitalWrite(LED_BUILTIN,HIGH);
  }

  digitalWrite(PIN_POWER_5V, (g_ctrl&CTRL_5V)==CTRL_5V ? HIGH:LOW);
  digitalWrite(PIN_CMD_P1, (g_ctrl&CTRL_P1)==CTRL_P1 ? HIGH:LOW);
  digitalWrite(PIN_CMD_P2, (g_ctrl&CTRL_P2)==CTRL_P2 ? HIGH:LOW);
  digitalWrite(PIN_CMD_P3, (g_ctrl&CTRL_P3)==CTRL_P3 ? HIGH:LOW);

  if ((g_voyants&OUP_GREEN)==OUP_GREEN)
  {
    if ((g_voyants&OUP_GREEN_BLINK)==OUP_GREEN_BLINK)
      digitalWrite(PIN_LED_GREEN,g_flgBlink_500ms);
    else
      digitalWrite(PIN_LED_GREEN,HIGH);    
  }
  else
    digitalWrite(PIN_LED_GREEN,LOW);
  

  if ( (g_ctrl&CTRL_5V)==CTRL_5V)
  {
    if ( (g_ctrl&CTRL_RGB1)==CTRL_RGB1 )    
      set_leds_by_mode(leds_bas,(g_modeA&0x0F));
    else
      clearAll(leds_bas);

    if ( (g_ctrl&CTRL_RGB2)==CTRL_RGB2 )
      set_leds_by_mode(leds_haut,((g_modeA>>4)&0x0F));
    else
      clearAll(leds_haut);

    FastLED.setBrightness(g_level);
    FastLED.show();    
  }

  if (cycle++>100)
  {
    Serial.print("CTRL: ");Serial.print(g_ctrl,HEX);
    Serial.print(" Modes: ");Serial.print(g_modeA&0x0F);Serial.print(" ");Serial.print((g_modeA>>4)&0x0F);Serial.print(" ");Serial.print(g_modeB&0x0F);
    Serial.print(" Lvl: ");Serial.print(g_level);
    Serial.print(" IN: ");Serial.println(g_inputs,HEX);
    cycle=0;
  }

  if (tmrBlink500mS.tick()==true)
    g_flgBlink_500ms=!g_flgBlink_500ms;

  tmrSelLong.tick();
  tmrAlive.tick();
}
