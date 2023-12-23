#include <Wire.h>

void setup()
{
  Wire.begin();

  Wire.beginTransmission(0xA);
  Wire.write(byte(0x01));
  Wire.write(byte(0x55));
  Wire.endTransmission();    
}

void loop() 
{
  delay(100);
}
