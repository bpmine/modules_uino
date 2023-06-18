#ifndef MASTER_HEADER_INCLUDED
#define MASTER_HEADER_INCLUDED

#include "request.hpp"

#include <arduino.h>

Request pump('A',FCT_PUMP);
Request oyaA('B',FCT_OYA);
Request oyaB('C',FCT_OYA);
Request oyaC('D',FCT_OYA);
Request oyaD('E',FCT_OYA);

class Master
{
  private:
    HardwareSerial *pStr;
    int txen;
  
    Request *cycle[5]=
    {
      &pump,
      &oyaA,
      &oyaB,
      &oyaC,
      &oyaD
    };
  
    int cur;
    uint8_t buffer[DATA_SIZE+10];
    int pos;
    int expected;
    bool pending;
    unsigned long tick0;
    bool discard;

    uint8_t calc_cs(uint8_t *datas,uint8_t len)
    {
      uint8_t cs=0;
      for (int i=0;i<len;i++)
      {
        cs+=datas[i];       
      }
      return cs;
    }

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

    void next_request(void)
    {
      pos=0;
      pending=false;
      discard=false;
      cur++;
      if (cur>=sizeof(cycle)/sizeof(Request *))
        cur=0;      
    }

    char tohexchar(uint8_t b)
    {
      b=b&0xF;
      if ( (b>=0) && (b<=9) )
      {
        return '0'+b;
      }
      else if ( (b>=0x0A) && (b<=0x0F) )
      {
        return 'A'+(b-10);
      }
      else
      {
        return '0';
      }  
    }

  public:
    Master()
    {    
      txen=-1;
      cur=0;
      for (int i=0;i<sizeof(cycle)/sizeof(Request *);i++)
      {
        cycle[i]->cmd=0;
      }
      pos=0;
      pending=false;
      discard=false;
    }

    void send(Request *pReq)
    {
      uint8_t req[8];
      uint8_t cs=0;
      req[0]=SOH;
      req[1]=pReq->addr;
      cs+=pReq->addr;
      req[2]=pReq->fct;
      cs+=pReq->fct;
      req[3]=tohexchar((pReq->cmd>>4)&0x0F);
      cs+=req[3];
      req[4]=tohexchar(pReq->cmd&0xFF);
      cs+=req[4];
      req[5]=tohexchar((cs>>4)&0xFF);
      req[6]=tohexchar(cs&0xFF);
      req[7]=STX;

      delay(10);
      digitalWrite(this->txen,HIGH);
      pStr->write(req,sizeof(req));
      pStr->flush();
      while ((UCSR0A & _BV (TXC0)) == 0) {}
      delay(1); 
      digitalWrite(this->txen,LOW);
    }

    void begin(HardwareSerial *pSerial,int txen)
    {
      this->txen=txen;
      pinMode(this->txen,OUTPUT);
      digitalWrite(this->txen,LOW);
      pSerial->begin(9600);
      this->pStr=pSerial;
      pos=0;
      cur=0;
      pending=false;    
      discard=false;  
      expected=0;
    }
  
    void loop(void)
    {
      if (pStr==NULL)
        return;
        
      if (pending==false)
      {
        pending=true;
        discard=false;
        send(cycle[cur]);
        tick0=millis();
      }
      else
      {
        unsigned long delta;
        unsigned long t=millis();        
        if (t>tick0)
          delta=t-tick0;
        else
          delta=0xFFFFFFFF-tick0+t;
        
        if (delta>20)
        {
          cycle[cur]->comm_ok=false;
          pending=false;
          pos=0;
          discard=false;
          next_request();
        }
      }
    }  

    void recv(void)
    {
      while (pStr->available())
      {
        int b=pStr->read();
        if ( (b>0) && (b<255) && (discard==false) )
        {
          if ( (pos==0) && (b!=SOH) )
            break;

          buffer[pos]=(uint8_t)b;

          if (b==STX)
          {
            uint8_t addr=buffer[1];
            uint8_t fct=buffer[2];
            uint8_t csa=buffer[pos-1];
            uint8_t csb=buffer[pos];
            uint8_t cs=decode_hex_byte(csa,csb);
            uint8_t cs_calc=calc_cs(buffer,pos);
            if (cs_calc==cs)
            {
              cycle[cur]->cserr=false;
              if ( (addr!=cycle[cur]->addr) || (fct!=cycle[cur]->fct) )
              {
                cycle[cur]->frmerr=true;
                cycle[cur]->nbCyclesErr++;
                cycle[cur]->comm_ok=false;
              }
              else
              {
                cycle[cur]->setRecvData(buffer+3);                
                cycle[cur]->frmerr=false;
                cycle[cur]->comm_ok=true;
                cycle[cur]->decodeData();         
                Serial.write(buffer,pos);
                Serial.println("");              
              }              
            }
            else
            {
              cycle[cur]->cserr=true;
              cycle[cur]->nbCyclesErr++;
              cycle[cur]->nbCsErr++;
              cycle[cur]->comm_ok=false;
            }

            next_request();
          }

          if (pos>sizeof(buffer)-1)
          {
            pos=0;
            discard=true;
          }

          pos++;          
        }
        else
        {
          discard=true;
        }

        tick0=millis();
      }
    }
};

#endif
