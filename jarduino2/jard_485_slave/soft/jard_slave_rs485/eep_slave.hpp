#ifndef EEP_SLAVE_HEADER_INCLUDED
#define EEP_SLAVE_HEADER_INCLUDED

#include <EEPROM.h>

#define EEP_ADDR_NODE_ADDR    (1)
#define EEP_ADDR_NODE_FCT     (2)

#define EEP_ADDR_ENABLED      (10)
#define EEP_ADDR_COUNTER      (11)

class Eep
{
  private:
    static void Eep::eep_write_ul(int addr,unsigned long value)
    {
      uint8_t a=(value>>24)&0xFF;
      uint8_t b=(value>>16)&0xFF;
      uint8_t c=(value>>8)&0xFF;
      uint8_t d=value&0xFF;
    
      EEPROM.write(addr,a);
      EEPROM.write(addr+1,b);
      EEPROM.write(addr+2,c);
      EEPROM.write(addr+3,d);
    }
    
    static unsigned long Eep::eep_read_ul(int addr)
    {
      uint8_t a=EEPROM.read(addr);
      uint8_t b=EEPROM.read(addr+1);
      uint8_t c=EEPROM.read(addr+2);
      uint8_t d=EEPROM.read(addr+3);
    
      unsigned long ul=((uint32_t)a<<24) | ((uint32_t)b<<16) | ((uint32_t)c<<8) | (uint32_t)d;
    
      return ul;  
    }
  
  public:
    static bool Eep::readID(uint8_t *pAddr,uint8_t *pFct)
    {
      uint8_t m1=EEPROM.read(0);
      uint8_t m2=EEPROM.read(3);

      *pAddr='z';
      *pFct='z';
      if ( (m1==0x55) && (m2==0xAA) )
      {
        *pAddr=EEPROM.read(EEP_ADDR_NODE_ADDR);
        *pFct=EEPROM.read(EEP_ADDR_NODE_FCT);
        return true;        
      }

      return false;
    }

    static void Eep::writeID(uint8_t addr,uint8_t fct)
    {
      EEPROM.write(0,0x55);
      EEPROM.write(EEP_ADDR_NODE_ADDR,addr);
      EEPROM.write(EEP_ADDR_NODE_FCT,fct);
      EEPROM.write(3,0xAA);
    }

    static void Eep::writeAddr(uint8_t addr)
    {     
       EEPROM.write(EEP_ADDR_NODE_ADDR,addr);
    }

    static void Eep::writeFunction(uint8_t fct)
    {
      EEPROM.write(EEP_ADDR_NODE_FCT,fct);
    }

    static bool Eep::readEnabled(void)
    {
      return EEPROM.read(EEP_ADDR_ENABLED)==0x11?true:false;
    }

    static void Eep::writeEnabled(bool enabled)
    {
      EEPROM.write(EEP_ADDR_ENABLED,enabled==true?0x11:0x00);
    }

    static uint32_t Eep::readCounter(void)
    {
      return Eep::eep_read_ul(EEP_ADDR_COUNTER);
    }

    static bool Eep::writeCounter(uint32_t tps_s)
    {
      Eep::eep_write_ul(EEP_ADDR_COUNTER,tps_s);    
    }
};

#endif
