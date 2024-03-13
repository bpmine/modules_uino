#include "master.h"

#include <Wire.h>

static T_IN _master_in;
static T_OUT _master_out;

static void _write_ctrl(unsigned char val);
static void _write_level(unsigned char level);
static void _write_modeA(unsigned char val);
static void _write_modeB(unsigned char val);
static unsigned char _do_alive(unsigned char val);
static unsigned char _read_eep(unsigned char addr);
static void _write_eep(unsigned char addr,unsigned char val);


void _write_ctrl(unsigned char val)
{
  Wire.beginTransmission(0xA);
  Wire.write(byte(1));
  Wire.write(byte(val));
  Wire.endTransmission();   
}

void _write_level(unsigned char level)
{
  Wire.beginTransmission(0xA);
  Wire.write(byte(2));
  Wire.write(byte(level));
  Wire.endTransmission();   
}


void _write_modeA(unsigned char val)
{
  Wire.beginTransmission(0xA);
  Wire.write(byte(3));
  Wire.write(byte(val));
  Wire.endTransmission();   
}

void _write_modeB(unsigned char val)
{
  Wire.beginTransmission(0xA);
  Wire.write(byte(4));
  Wire.write(byte(val));
  Wire.endTransmission();   
}

unsigned char _do_alive(unsigned char val)
{
  Wire.beginTransmission(0xA);
  Wire.write(byte(20));
  Wire.write(byte(val));
  Wire.endTransmission();  
  Wire.requestFrom(0xA, 1); 
  if (1 <= Wire.available())
    return Wire.read();
  else
    return 0;
}

/*void test_read_value(const char *name,unsigned char reg)
{
  Serial.print(name);
  Serial.print(": ");
  
  Wire.beginTransmission(0xA);
  Wire.write(byte(reg));
  Wire.endTransmission();    
  Wire.requestFrom(0xA, 1);

  if (1 <= Wire.available())
  {
    Serial.println(Wire.read());
  }  
}*/

unsigned char _read_eep(unsigned char addr)
{
  Serial.print("Read EEP @");
  Serial.print(addr);
  Serial.print(": ");
  
  Wire.beginTransmission(0xA);
  Wire.write(byte(16));
  Wire.write(byte(addr));
  Wire.endTransmission();    
  Wire.requestFrom(0xA, 1);

  if (1 <= Wire.available())
  {
    return Wire.read();
  }  
  else
  {
    return 0;
  }
}

void _write_eep(unsigned char addr,unsigned char val)
{
  Serial.print("Write EEP @");
  Serial.print(addr);
  Serial.print(": ");
  
  Wire.beginTransmission(0xA);
  Wire.write(byte(5));
  Wire.write(byte(addr));
  Wire.write(byte(val));
  Wire.endTransmission();    
}

void master_init(void)
{
  _master_out.ctrl=0;
  _master_out.level=0;
  _master_out.modeRGB1=0;
  _master_out.modeRGB2=0;
  _master_out.modeRGB3=0;
  _master_out.leds=0;
}

void master_loop(void)
{
  _write_ctrl(_master_out.ctrl);
  _write_level(_master_out.level);

  byte a,b;
  a=(_master_out.modeRGB1&0xF);
  a=a|((_master_out.modeRGB2<<4)&0xF);
  b=(_master_out.modeRGB3&0xF);
  _write_modeA(a);
  _write_modeB(b);

  /// + LEDs à finir
  _do_alive(0x10);

  /// + lire les valeurs en entree
}
