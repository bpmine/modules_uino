#include "app.h"
#include "pins.h"

void setup() 
{
  pinMode(PIN_TX_EN,INPUT);    
  pinMode(PIN_TEST_BTN,INPUT_PULLUP);
  
  pinMode(PIN_PWR_ON,OUTPUT);
  digitalWrite(PIN_PWR_ON,LOW);  
  
  pinMode(LED_BUILTIN,OUTPUT);
  digitalWrite(LED_BUILTIN,LOW);
  
  pinMode(PIN_DATA_LEDS,OUTPUT);
  digitalWrite(PIN_DATA_LEDS,LOW);

  pinMode(PIN_PWR_LEDS,OUTPUT);
  digitalWrite(PIN_PWR_LEDS,LOW);  
  
  pinMode(PIN_SD_CS,OUTPUT);
  digitalWrite(PIN_SD_CS,LOW);
  pinMode(11,INPUT);
  pinMode(12,INPUT);
  pinMode(13,INPUT);

  Serial.begin(9600);  
  Serial.println("Boot");

  app_init();  
} 

void loop() 
{
 app_loop();
}
