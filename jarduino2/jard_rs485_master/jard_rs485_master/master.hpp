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

    enum {IDLE,SEND,RECV,NEXT} eState;
    int cur;
    uint8_t buffer[DATA_SIZE+10];
    int pos;
    unsigned long m_tick0;

    uint8_t calc_cs(uint8_t *datas,uint8_t len)
    {
      uint8_t cs=0;
      for (int i=1;i<len;i++)
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

      eState=IDLE;
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
      delay(2); 
      digitalWrite(this->txen,LOW);

      char tmp[20];
      sprintf(tmp,"Send %c %c",pReq->addr,pReq->fct);
      Serial.println(tmp);
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
      eState=IDLE;
      m_tick0=millis();
    }

    void start_cycle(void)
    {
      eState=SEND;
    }
  
    void loop(void)
    {
      if (pStr==NULL)
        return;

      switch (eState)
      {
        case IDLE:
        {
          m_tick0=millis();
          break;
        }
        
        case SEND:
        {
          eState=RECV;
          memset(buffer,0,sizeof(buffer));
          pos=0;
          send(cycle[cur]);
          m_tick0=millis();
          break;
        }
        
        case RECV:
        {
          unsigned long delta;
          unsigned long t=millis(); 
          if (t >= m_tick0)
          {
            delta=t-m_tick0;
          }
          else
          {
            delta=0xFFFFFFFFUL-m_tick0+t;
          }

          if (delta > 200UL)
          {
            char tmp[15];
            sprintf(tmp,"TMT: %c",cycle[cur]->addr);
            Serial.println(tmp);
            
            cycle[cur]->comm_ok=false;
            eState=NEXT;          
          }

          break;
        }

        case NEXT:
        {
          cur++;
          if (cur>=sizeof(cycle)/sizeof(Request *))
          {
            eState=IDLE;
            cur=0;
          }
          else
          {
            delay(20);
            eState=SEND;
          }
          
          break;          
        }
      }
    }  

    void recv(void)
    {
      while (pStr->available())
      {
        int b=pStr->read();
        //Serial.write(b);
        if (eState!=RECV)
          continue;
          
        if ( (b>0) && (b<255) )
        {
          if ( (pos==0) && (b!=SOH) )
            continue;

          buffer[pos]=(uint8_t)b;
          pos++;
          if (pos>sizeof(buffer)-1)
          {
            pos=0;
            continue;
          }
          m_tick0=millis();          

          if (b==STX)
          {
            uint8_t addr=buffer[1];
            uint8_t fct=buffer[2];
            uint8_t csa=buffer[pos-3];
            uint8_t csb=buffer[pos-2];
            uint8_t cs=decode_hex_byte(csa,csb);
            uint8_t cs_calc=calc_cs(buffer,pos-3);

            char str[50];
            sprintf(str,"recv %c %c cs=%02x calc=%02x",addr,fct,cs,cs_calc);
            Serial.println(str);
            if (cs_calc==cs)
            {
              cycle[cur]->cserr=false;
              if ( (addr!=cycle[cur]->addr) || (fct!=cycle[cur]->fct) )
              {
                cycle[cur]->frmerr=true;
                cycle[cur]->nbCyclesErr++;
                cycle[cur]->comm_ok=false;                
                Serial.println("Nosync");
              }
              else
              {
                cycle[cur]->setRecvData(buffer+3);                
                cycle[cur]->frmerr=false;
                cycle[cur]->comm_ok=true;
                cycle[cur]->decodeData();
                Serial.println("Frame ok");
                eState=NEXT;
              }              
            }
            else
            {
              cycle[cur]->cserr=true;
              cycle[cur]->nbCyclesErr++;
              cycle[cur]->nbCsErr++;
              cycle[cur]->comm_ok=false;
              Serial.println("CS Error");
            }            
          }
        }        
      }
    }
};

#endif