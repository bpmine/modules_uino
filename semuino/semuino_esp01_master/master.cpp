/**
 * @file master.cpp
 * @brief Implémentation - Gestion de la communication I²C avec l'esclave nano
*/
#include "master.h"
#include "i2c_cmn.h"

#include <Wire.h>
#include <Arduino.h>

static T_IN _master_in;
static T_OUT _master_out;

void _write_register(unsigned char reg,unsigned char val);
bool _read_register(unsigned char reg,unsigned char *pVal);

static void _write_ctrl(unsigned char val);
static void _write_level(unsigned char level);
static void _write_modeA(unsigned char val);
static void _write_modeB(unsigned char val);
static unsigned char _do_alive(unsigned char val);
static unsigned char _read_eep(unsigned char addr);
static void _write_eep(unsigned char addr,unsigned char val);

bool _read_inputs(unsigned char *pVal);
bool _read_hums(unsigned char *pHum1,unsigned char *pHum2,unsigned char *pHum3);


/**
 * @brief Ecriture d'une registre
 * @param[in] reg Id du registre
 * @param[in] val Valeur a ecrire
*/
void _write_register(unsigned char reg,unsigned char val)
{
  Wire.beginTransmission(ADDR_SLAVE);
  Wire.write(reg);
  Wire.write(val);
  Wire.endTransmission();   
}

/**
 * @brief Ecriture d'une registre
 * @param[in] reg Id du registre
 * @param[out] pVal Pointeur vers la valeur lue
 * @return true en cas de reussite sinon false
*/
bool _read_register(unsigned char reg,unsigned char *pVal)
{
  Wire.beginTransmission(ADDR_SLAVE);
  Wire.write(reg);
  Wire.endTransmission();    
  Wire.requestFrom(ADDR_SLAVE, 1);

  if (1 <= Wire.available())
  {
    *pVal=Wire.read();
    return true;
  }
  else
    return false;
}

inline void _write_ctrl(unsigned char ctrl)
{
  _write_register(REG_CTRL,ctrl);
}

inline void _write_level(unsigned char level)
{
  _write_register(REG_LEVEL,level);
}

inline void _write_modeA(unsigned char val)
{
  _write_register(REG_MODE_RGB_A,val);
}

inline void _write_modeB(unsigned char val)
{
  _write_register(REG_MODE_RGB_B,val);
}

inline void _write_voyants(unsigned char val)
{
  _write_register(REG_VOYANTS,val);
}

inline unsigned char _do_alive(unsigned char val)
{
  Wire.beginTransmission(ADDR_SLAVE);
  Wire.write(REG_ALIVE);
  Wire.write(val);
  Wire.endTransmission();  
  Wire.requestFrom(ADDR_SLAVE, 1); 
  if (1 <= Wire.available())
    return Wire.read();
  else
    return 0;
}

inline bool _read_inputs(unsigned char *pVal)
{
  return _read_register(REG_INPUTS,pVal);
}

inline bool _read_hums(unsigned char *pHum1,unsigned char *pHum2,unsigned char *pHum3)
{
  if (_read_register(REG_H1,pHum1)==false)
    return false;
  if (_read_register(REG_H2,pHum2)==false)
    return false;
  if (_read_register(REG_H3,pHum3)==false)
    return false;

  return true;
}

inline unsigned char _read_eep(unsigned char addr)
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

inline void _write_eep(unsigned char addr,unsigned char val)
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

/**
 * @brief Lecture d'un octet en EEPROM de l'esclave nano
 * @param[in] addr Adresse
 * @return Octet lu
*/
unsigned char master_read_eep(unsigned char addr)
{
  return _read_eep(addr);
}

/**
 * @brief Ecriture d'un octet en EEPROM de l'esclave nano
 * @param[in] addr Adresse
 * @param[in] val Octet a ecrire
*/
void master_write_eep(int addr,unsigned char val)
{
  _write_eep(addr,val);
}

/**
 * @brief Recuperation des entrees lues lors du dernier cycle
 * @param[out] pIN Pointeur vers les valeurs retournees
*/
void master_get_in_values(T_IN *pIN)
{
  memcpy(pIN,&_master_in,sizeof(T_IN));
}

/**
 * @brief Ecriture des commandes à appliquer au prochain cycle
 * @param[in] pOUT Pointeur vers les valeurs a envoyer
*/
void master_set_out_values(T_OUT *pOUT)
{
  memcpy(&_master_out,pOUT,sizeof(T_OUT));
}

/**
 * @brief Initialisation du maître I²C
*/
void master_init(void)
{
  _master_out.ctrl=0;
  _master_out.level=0;
  _master_out.modeRGB1=0;
  _master_out.modeRGB2=0;
  _master_out.modeRGB3=0;
  _master_out.voyants=0;

  _master_in.inputs=0;
  _master_in.h1=0;
  _master_in.h2=0;
  _master_in.h3=0;
}

/**
 * @brief Gestion d'un cycle de lecture/ecriture I2c ver l'esclave nano
*/
void master_loop(void)
{
  /// @remark Lecture des entrées
  if (_read_inputs(&(_master_in.inputs))==false)
    _master_in.inputs=0;

  /// @remark Lecture des valeurs des dondes d'humidité
  if (_read_hums(&(_master_in.h1),&(_master_in.h2),&(_master_in.h3))==false)
  {
    _master_in.h1=0;
    _master_in.h2=0;
    _master_in.h3=0;
  }

  /// @remark Ecriture des commandes
  _write_ctrl(_master_out.ctrl);
  _write_level(_master_out.level);
  
  unsigned char a,b;
  a=(_master_out.modeRGB1&0xF);
  a= a | ((_master_out.modeRGB2<<4)&0xF0);
  b=(_master_out.modeRGB3&0xF);
  _write_modeA(a);
  _write_modeB(b);

  _write_voyants(_master_out.voyants);

  /*Serial.print(_master_out.modeRGB1);
  Serial.print(" ");
  Serial.print(_master_out.modeRGB2);
  Serial.print(" ");
  Serial.print(a);
  Serial.print(" ");
  Serial.println(_master_out.ctrl,HEX);*/
  
  unsigned char res=_do_alive(0x11);
  if (res!=(unsigned char)~0x11)
  {
    Serial.print("Alive:");
    Serial.println(res,HEX);
  }
  
  /// + lire les valeurs en entree  
}
