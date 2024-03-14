#include "master.h"
#include "i2c_cmn.h"

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
  Wire.beginTransmission(ADDR_SLAVE);
  Wire.write(byte(REG_CTRL));
  Wire.write(byte(val));
  Wire.endTransmission();   
}

void _write_level(unsigned char level)
{
  Wire.beginTransmission(ADDR_SLAVE);
  Wire.write(byte(REG_LEVEL));
  Wire.write(byte(level));
  Wire.endTransmission();   
}

void _write_modeA(unsigned char val)
{
  Wire.beginTransmission(ADDR_SLAVE);
  Wire.write(byte(REG_MODE_RGB_A));
  Wire.write(byte(val));
  Wire.endTransmission();   
}

void _write_modeB(unsigned char val)
{
  Wire.beginTransmission(ADDR_SLAVE);
  Wire.write(byte(REG_MODE_RGB_B));
  Wire.write(byte(val));
  Wire.endTransmission();   
}

unsigned char _do_alive(unsigned char val)
{
  Wire.beginTransmission(ADDR_SLAVE);
  Wire.write(byte(20));
  Wire.write(byte(val));
  Wire.endTransmission();  
  Wire.requestFrom(ADDR_SLAVE, 1); 
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
  
  Wire.beginTransmission(ADDR_SLAVE);
  Wire.write(byte(REG_EEP_READ));
  Wire.write(byte(addr));
  Wire.endTransmission();    
  Wire.requestFrom(ADDR_SLAVE, 1);

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
  
  Wire.beginTransmission(ADDR_SLAVE);
  Wire.write(byte(REG_EEP_WRITE));
  Wire.write(byte(addr));
  Wire.write(byte(val));
  Wire.endTransmission();    
}

void master_get_in_values(T_IN *pIN)
{  
}

void master_set_out_values(T_OUT *pOUT)
{
  memcpy(&_master_out,pOUT,sizeof(T_OUT));
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

int cycle=0;

void master_loop(void)
{
  if (cycle++>2000)
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
  cycle=0;
  }
}
