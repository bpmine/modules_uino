/**
 * @brief semuino.ino
 * 
 * SEMUINO - Gestion du meuble à semi
**/
#include "FastLED.h"
#include <EEPROM.h>
#include <DHT.h>
#include <Wire.h>

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
#define COL_RED             CRGB(127,0,0)  
#define COL_GREEN           CRGB(0,127,0)  
#define COL_BLUE            CRGB(0,0,127)


CRGB leds_bas[NUM_LEDS];      ///< Tableau des LEDs du bas
CRGB leds_haut[NUM_LEDS];     ///< Tableau des LEDs du haut
//CRGB leds_dessus[NUM_LEDS];   ///< Tableau des LEDs du dessus du meuble

#define ADDR_SLAVE      (0xA)

#define REG_CTRL        (1)
#define REG_LEVEL       (2)
#define REG_MODE_RGB_A  (3)
#define REG_MODE_RGB_B  (4)
#define REG_EEP_WRITE   (5)

#define REG_INPUTS      (10)
#define REG_TEMP        (11)
#define REG_HUM         (12)
#define REG_H1          (13)
#define REG_H2          (14)
#define REG_H3          (15)
#define REG_EEP_READ    (16)

unsigned char g_reg=0;
unsigned char g_addr=0;

unsigned char g_inputs=0;
unsigned char g_hum_pc=0;
unsigned char g_temp_dg=0;
unsigned char g_hum1=0;
unsigned char g_hum2=0;
unsigned char g_hum3=0;

unsigned char g_ctrl=0;
unsigned char g_modeA=0;
unsigned char g_modeB=0;
unsigned char g_level=255;                  ///< Puissance

#define CTRL_5V         0x01
#define CTRL_RGB1       0x02
#define CTRL_RGB2       0x04
#define CTRL_RGB3       0x08
#define CTRL_P1         0x10
#define CTRL_P2         0x20
#define CTRL_P3         0x40
#define CTRL_RESERVED   0x80

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

void requestEvent() 
{  
    switch (g_reg)
    {
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

    Serial.println("Request");  
}

void receiveEvent(int howMany) 
{
  g_reg=Wire.read();
  if (Wire.available())
  {
    switch (g_reg)
    {
      case REG_CTRL:
      {
        g_ctrl=Wire.read();
        Serial.print("Rec CTRL: ");
        Serial.println(g_ctrl,HEX);
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
      case REG_EEP_READ:
      {
        g_addr=Wire.read();
        break;
      }
    }
  }
}


/**
 * @brief Setup d'arduino (demarage)
 * 
 * Une petite animation avec test de toutes les LEDs
*/
void setup() 
{
  g_level=0;
  
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

  clearAll(leds_bas);
  clearAll(leds_haut);
  //clearAll(leds_dessus);
  FastLED.show();

  clearAll(leds_bas);
  clearAll(leds_haut);
  //clearAll(leds_dessus);
  FastLED.show();
}

void readHumidity(void)
{
  int tmp=analogRead(PIN_HUM1);
  g_hum1=(unsigned char)(tmp*255/1023);
  
  tmp=analogRead(PIN_HUM2);
  g_hum2=(unsigned char)(tmp*255/1023);

  tmp=analogRead(PIN_HUM3);
  g_hum3=(unsigned char)(tmp*255/1023);
}

/**
 * @brief BOUCLE PRINCIPALE ARDUINO
*/
void loop() 
{
  if (digitalRead(PIN_SELECT_BTN)==HIGH)
     g_inputs|=0x01;
  else
     g_inputs&=~0x01;

  readHumidity();

  delay(100);

  digitalWrite(PIN_POWER_5V, (g_ctrl&CTRL_5V)==CTRL_5V ? HIGH:LOW);
  digitalWrite(PIN_CMD_P1, (g_ctrl&CTRL_P1)==CTRL_P1 ? HIGH:LOW);
  digitalWrite(PIN_CMD_P2, (g_ctrl&CTRL_P2)==CTRL_P2 ? HIGH:LOW);
  digitalWrite(PIN_CMD_P3, (g_ctrl&CTRL_P3)==CTRL_P3 ? HIGH:LOW);

  if ( (g_ctrl&CTRL_5V)==CTRL_5V)
  {
    if ( (g_ctrl&CTRL_RGB1)==CTRL_RGB1 )
      setAll(leds_bas,COL_RED);
    else
      clearAll(leds_bas);

    if ( (g_ctrl&CTRL_RGB2)==CTRL_RGB2 )
      setAll(leds_haut,COL_BLUE);
    else
      clearAll(leds_haut);

    /*if ( (g_ctrl&CTRL_RGB1)==CTRL_RGB1 )
      setAll(leds_bas,COL_RED);
    else
      clearAll(leds_bas);*/

    FastLED.setBrightness(g_level);
    FastLED.show();
  }
}
