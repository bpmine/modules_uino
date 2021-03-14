#include "FastLED.h"

#define POWER_MAX         100
#define POWER_MIN         50
#define NUM_LEDS          (110)

#define PIN_DATA_LED      3

int g_power;
CRGB leds[NUM_LEDS];

void setAll(CRGB *pLeds,CRGB a)
{
  for (int i=0;i<NUM_LEDS;i++)
  {
     pLeds[i] = a;
  }
}

void clearAll(CRGB *pLeds)
{
  setAll(pLeds,CRGB::Black);
}


void setup() 
{
  g_power=POWER_MAX;
  
  Serial.begin(9600);
  Serial.println("Boot");

  FastLED.addLeds<NEOPIXEL, PIN_DATA_LED>(leds, NUM_LEDS); 
  FastLED.setBrightness(g_power);

  setAll(leds,CRGB::Red);
  FastLED.show();
  delay(1000); 

  setAll(leds,CRGB::Green);
  FastLED.show();
  delay(1000); 

  clearAll(leds);
  FastLED.show();
}

void loop() 
{
  delay(10);
  
}
