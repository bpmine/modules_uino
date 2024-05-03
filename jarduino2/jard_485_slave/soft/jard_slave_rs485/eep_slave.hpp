#ifndef EEP_SLAVE_HEADER_INCLUDED
#define EEP_SLAVE_HEADER_INCLUDED

#include <EEPROM.h>


#define EEP_ADDR_NODE_M1      (0)
#define EEP_ADDR_NODE_ADDR    (1)
#define EEP_ADDR_COUNTER      (2)
#define EEP_ADDR_NODE_M2      (4)

class Eep
{
  private:
    static void Eep::eep_write_us(int addr,unsigned short value)
    {
      uint8_t a=(value>>8)&0xFF;
      uint8_t b=value&0xFF;
    
      EEPROM.write(addr,a);
      EEPROM.write(addr+1,b);
    }
    
    static unsigned short Eep::eep_read_us(int addr)
    {
      uint8_t a=EEPROM.read(addr);
      uint8_t b=EEPROM.read(addr+1);
    
      unsigned short us= ((uint32_t)a<<8) | (uint32_t)b;
    
      return us;
    }
  
  public:
    static bool Eep::readID(uint8_t *pAddr)
    {
      uint8_t m1=EEPROM.read(EEP_ADDR_NODE_M1);
      uint8_t m2=EEPROM.read(EEP_ADDR_NODE_M2);

      *pAddr='Z';
      if ( (m1==0x55) && (m2==0xAA) )
      {
        *pAddr=EEPROM.read(EEP_ADDR_NODE_ADDR);
        return true;        
      }

      writeID('Z');
      writeCounter(0);

      return false;
    }

    static void Eep::writeID(uint8_t addr)
    {
      EEPROM.write(EEP_ADDR_NODE_M1,0x55);
      EEPROM.write(EEP_ADDR_NODE_ADDR,addr);
      EEPROM.write(EEP_ADDR_NODE_M2,0xAA);
    }

    static uint16_t Eep::readCounter(void)
    {
      return Eep::eep_read_us(EEP_ADDR_COUNTER);
    }

    static bool Eep::writeCounter(uint16_t tps_s)
    {
      Eep::eep_write_us(EEP_ADDR_COUNTER,tps_s);    
    }
};

#endif
