#ifndef REQUEST_HEADER_INCLUDED
#define REQUEST_HEADER_INCLUDED

#define DATA_SIZE (12)

#include "prot.h"

#include <arduino.h>

class Request
{
  protected:
    uint8_t datas[DATA_SIZE];
  
  public:
    uint8_t addr;
    uint8_t fct;
    uint8_t cmd;
    bool comm_ok;
    bool cserr;
    bool frmerr;
    int nbCyclesErr;
    int nbCsErr;
    
    Request(uint8_t addr,uint8_t fct)
    {
      this->addr=addr;
      this->fct=fct;
      cmd=0;
      comm_ok=false;
      cserr=false;
      frmerr=false;
      nbCyclesErr=0;
      nbCsErr=0;
    }

    void razErrors(void)
    {
      nbCsErr=0;
      nbCyclesErr=0;
      cserr=false;
      frmerr=false;
    }

    void setRecvData(uint8_t *pDatas)
    {
      memcpy(datas,pDatas,DATA_SIZE);
    }

    void setCmd(bool flgON)
    {
      cmd=flgON==true?1:0;
    }

    virtual void decodeData(void) {};
};

class RqSlave:public Request
{
  protected:
    bool enabled;
    bool on;
    int temp;
    int hum;

    uint8_t hex_val(uint8_t hex)
    {
      if ( (hex>='0') && (hex<='9') )
      {
        return hex-'0';
      }
      else if ( (hex>='A') && (hex<='F') )
      {
        return (hex-'A')+10;
      }
      else
      {
        return 0;
      }
    }

    uint8_t decode_hex_byte(uint8_t a,uint8_t b)
    {
      return hex_val(a)*16+hex_val(b);      
    }
    
    
  public:
    RqSlave(uint8_t addr,uint8_t fct):Request(addr,fct)
    {
      enabled=false;
      on=false;
    } 

    bool getEnabled(void)
    {
      return enabled;
    }
    
    bool getCommOk(void)
    {
      return comm_ok;
    }

    bool getOn(void)
    {
      return on;
    }
    
    int getTemp(void)
    {
      return temp;
    }
    
    int getHum(void)
    {
      return hum;
    }

    virtual void decodeData(void) override
    {
      if (comm_ok==true)
      {
        uint8_t st=decode_hex_byte(datas[0],datas[1]);
        if ( (st&ST_ENABLED) == ST_ENABLED )
          enabled=true;
        else
          enabled=false;

        if ( (st&ST_CMD_ON) == ST_CMD_ON )
          on=true;
        else
          on=false;   
      }
    }    
};

class RqPump:public RqSlave
{
  private:
    int flow;
    
  public:
    RqPump(uint8_t addr):RqSlave(addr,FCT_PUMP)
    {
    }

    int getFlow(void)
    {
      return flow;
    }

    virtual void decodeData(void) override
    {
      RqSlave::decodeData();
      if (comm_ok==true)
      {
        uint8_t MSB=decode_hex_byte(datas[2],datas[3]);
        uint8_t LSB=decode_hex_byte(datas[4],datas[5]);
        flow=(((int)MSB)<<8) | (((int)LSB));
        
        temp=decode_hex_byte(datas[6],datas[7]);        
        hum=decode_hex_byte(datas[8],datas[9]);
      }
    }
};


class RqOya:public RqSlave
{
  private:
    bool low;
    bool high;
    
  public:
    RqOya(uint8_t addr):RqSlave(addr,FCT_OYA)
    {
    }

    bool getHigh(void) {return high;}
    bool getLow(void) {return low;}

    virtual void decodeData(void) override
    {
      RqSlave::decodeData();
      
      if (comm_ok==true)
      {
        uint8_t st=decode_hex_byte(datas[0],datas[1]);

        if ( (st&ST_LVL_LOW) == ST_LVL_LOW )
          low=true;
        else
          low=false;

        if ( (st&ST_LVL_HIGH) == ST_LVL_HIGH )
          high=true;
        else
          high=false;   

        temp=decode_hex_byte(datas[2],datas[3]);
        
        hum=decode_hex_byte(datas[4],datas[5]);  
      }
    }
};

#endif
