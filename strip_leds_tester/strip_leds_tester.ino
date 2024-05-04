/**
*
*    ----------------------------
*    |                          |
*    |   [RED]        [GREEN]   |
*    |                          |        
*    |          [SW1]           |
*    |                          |              
*    |          [SW2]           |
*    |                          |
*    ----------------------------
**/
#include "FastLED.h"

//#define NUM_LEDS          (23*7)
#define NUM_LEDS          (276)
#define PIN_DATA_LEDS     (2)
#define PIN_SW1           (3)
#define PIN_SW2           (4)
#define PIN_RED           (5)
#define PIN_GREEN         (6)


CRGB leds[NUM_LEDS];


void set(CRGB *pLeds)
{
  pLeds[0]=CRGB(255,0,0);
  pLeds[1]=CRGB(0,255,0);
  pLeds[2]=CRGB(0,0, 255);

  pLeds[50]=CRGB(255,0,0);
  pLeds[51]=CRGB(0,255,0);
  pLeds[52]=CRGB(0,0, 255);

  pLeds[100]=CRGB(255,0,0);
  pLeds[101]=CRGB(0,255,0);
  pLeds[102]=CRGB(0,0, 255);

  pLeds[150]=CRGB(255,0,0);
  pLeds[151]=CRGB(0,255,0);
  pLeds[152]=CRGB(0,0, 255);

  pLeds[200]=CRGB(255,0,0);
  pLeds[201]=CRGB(0,255,0);
  pLeds[202]=CRGB(0,0, 255);

  pLeds[250]=CRGB(255,0,0);
  pLeds[251]=CRGB(0,255,0);
  pLeds[252]=CRGB(0,0, 255);
  /*for (int i=0;i<NUM_LEDS;i++)
  {
    if ((i%3==0))
      pLeds[i] = CRGB(255,0,0);
    else if (((i+1)%3==0))
      pLeds[i] = CRGB(0,255,0);
    else if (((i+2)%3==0))
      pLeds[i] = CRGB(0,0,255);
  }*/
}


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
  setAll(leds,col);
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
  clearAll(leds);
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
  randomSeed(analogRead(0));
  Serial.begin(9600);
  Serial.println("Boot...");  

  pinMode(PIN_DATA_LEDS, OUTPUT);
  digitalWrite(PIN_DATA_LEDS,LOW);

  pinMode(PIN_SW1, INPUT_PULLUP);
  pinMode(PIN_SW2, INPUT_PULLUP);

  pinMode(PIN_RED, OUTPUT);
  digitalWrite(PIN_RED,LOW);
  pinMode(PIN_GREEN, OUTPUT);
  digitalWrite(PIN_GREEN,LOW);

  FastLED.addLeds<NEOPIXEL, PIN_DATA_LEDS>(leds, NUM_LEDS); 
  FastLED.setBrightness(10);

  pinMode(LED_BUILTIN,OUTPUT);
  digitalWrite(LED_BUILTIN,HIGH);

  //clearAll();
  set(leds);
  FastLED.show();
}

void demo1()
{
  setAll(leds,CRGB::Red);
  FastLED.show();
  delay(1000);
  setAll(leds,CRGB::Blue);
  FastLED.show();
  delay(1000);
  setAll(leds,CRGB::Green);
  FastLED.show();
  delay(1000);
  setAll(leds,CRGB::Black);
  FastLED.show();
  delay(1000);  
}

void demo2()
{
  clearAll();
  for (int i=0;i<NUM_LEDS;i++)
  {
    if (i>6)
    {
      leds[i-6]=CRGB::Black;
      leds[i-5]=CRGB::Red;
      leds[i-4]=CRGB::Red;
      leds[i-3]=CRGB::Green;
      leds[i-2]=CRGB::Green;
      leds[i-1]=CRGB::Blue;
      leds[i]=CRGB::Blue;
    }
    FastLED.show();
    //delay(1);
  }
}

int ctr=0;
bool dir=true;

void demo3()
{
  ctr++;
  
  FastLED.setBrightness(30);

  if (dir==true)
  {
    CRGB tmp=leds[NUM_LEDS-1];
    for (int i=NUM_LEDS;i>0;i--)
    {
      leds[i]=leds[i-1];
    }
    leds[0]=tmp;
  }
  else
  {
    CRGB tmp=leds[0];
    for (int i=0;i<=NUM_LEDS;i++)
    {
      leds[i]=leds[i+1];
    }
    leds[NUM_LEDS-1]=tmp;
  }
    
  FastLED.show();
  int d=random(20,300);
  delay(d);

 // digitalWrite(PIN_RED,digitalRead(PIN_SW1));
 // digitalWrite(PIN_GREEN,digitalRead(PIN_SW2));

  //int n=random(0,NUM_LEDS-1 );
  //leds[n]=CRGB(0,0,0);

  int n=random(0,100);
  if (n==1)
    dir=!dir;  
}

int mode=0;
int pos=0;

void test(void)
{
  digitalWrite(PIN_RED,HIGH);
  
  switch (mode)
  {
    case 0:
    {
      setAll(CRGB(255,0,0));
      digitalWrite(PIN_GREEN,LOW);
      break;
    }
    case 1:
    {
      setAll(CRGB(0,255,0));
      digitalWrite(PIN_GREEN,LOW);
      break;
    }
    case 2:
    {
      setAll(CRGB(0,0,255));
      digitalWrite(PIN_GREEN,LOW);
      break;
    }
    case 8:
    {
      digitalWrite(PIN_GREEN,HIGH);
      
      clearAll();
      if (pos<NUM_LEDS)
      {
        leds[pos]=CRGB(0,255,0);
        pos++;
      }
      else
      {
        mode=3;
        pos=0;
      }
      delay(100);
      break;
    }
    case 3:
    default:
    {
      clearAll();
      digitalWrite(PIN_GREEN,LOW);
      break;
    }
  }

  if (digitalRead(PIN_SW1)==LOW)
  {
    delay(10);
    if (digitalRead(PIN_SW1)==LOW)
    {
      mode++;
      if (mode>3)
        mode=0;
      delay(500);
    }
  }
  
  if (digitalRead(PIN_SW2)==LOW)
  {
    delay(10);
    if (digitalRead(PIN_SW2)==LOW)
    {
      mode=8;
      pos=0;
      delay(500);
    }
  }

  FastLED.setBrightness(30);
  FastLED.show();
}

/**
 * @brief BOUCLE PRINCIPALE ARDUINO
*/
void loop() 
{
  test();
  
  //demo2();
  //demo3();

  
}