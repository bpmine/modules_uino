#include "app.h"

void setup() 
{
  Serial.begin(9600);  
  Serial.println("Boot");

  app_init();
} 

void loop() 
{
 app_loop();
}
