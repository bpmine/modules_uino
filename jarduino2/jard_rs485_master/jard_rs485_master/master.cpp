#include "master.h"

uint8_t Master::calc_cs(uint8_t *datas,uint8_t len)
{
  uint8_t cs=0;
  for (int i=1;i<len;i++)
  {
    cs+=datas[i];       
  }
  return cs;
}

uint8_t Master::hex_val(uint8_t hex)
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

uint8_t Master::decode_hex_byte(uint8_t a,uint8_t b)
{
  return hex_val(a)*16+hex_val(b);      
}
    
char Master::tohexchar(uint8_t b)
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

void Master::init()
{
  int pos=0;
  Request * p=itNext(pos);
  while (p!=NULL)
  {
    p->cmd=0;
    p=itNext(pos);
  }

  curReq=0;
  pos=0;
  eState=IDLE;
  m_tick0=millis();  
}
    
Master::Master()
{
  txen=-1;    
  init();
}

void Master::send(Request *pReq)
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

void Master::begin(HardwareSerial *pSerial,int txen)
{
  this->txen=txen;
  pinMode(this->txen,OUTPUT);
  digitalWrite(this->txen,LOW);
  pSerial->begin(9600);
  this->pStr=pSerial;
  
  init();
}

void Master::start_cycle(void)
{
  if (eState!=OFF)
    eState=SEND;
}

void Master::setEnable(bool enable)
{
  if (enable==false)
    eState=OFF;
  else if (eState==OFF)
  {
    init();
    eState=IDLE;
  }
}
  
bool Master::loop(void)
{
  if ( pStr==NULL )
    return false;

  Request *pReq=getCurRequest();

  switch (eState)
  {
    case OFF:
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
      send(pReq);
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
        sprintf(tmp,"TMT: %c",pReq->addr);
        Serial.println(tmp);
            
        pReq->comm_ok=false;
        eState=NEXT;          
      }

      break;
    }

    case NEXT:
    {
      if (nextRequest()==false)
      {
        eState=END;
      }
      else
      {
        delay(20);
        eState=SEND;
      }
          
      break;          
    }

    case END:
    {
      eState=IDLE;
      return true;
    }
  }

  return false;
}  

    void Master::recv(void)
    {
      Request *pReq=getCurRequest();
      if (pReq==NULL)
        return;
      
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
              pReq->cserr=false;
              if ( (addr!=pReq->addr) || (fct!=pReq->fct) )
              {
                pReq->frmerr=true;
                pReq->nbCyclesErr++;
                pReq->comm_ok=false;                
                Serial.println("Nosync");
              }
              else
              {
                pReq->setRecvData(buffer+3);                
                pReq->frmerr=false;
                pReq->comm_ok=true;
                pReq->decodeData();
                Serial.println("Frame ok");
                eState=NEXT;
              }              
            }
            else
            {
              pReq->cserr=true;
              pReq->nbCyclesErr++;
              pReq->nbCsErr++;
              pReq->comm_ok=false;
              Serial.println("CS Error");
            }            
          }
        }        
      }
    }

static RqPump _rqpump('A');
static RqOya _rqoyaA('B');
static RqOya _rqoyaB('C');
static RqOya _rqoyaC('D');
static RqOya _rqoyaD('E');

static Request *_cycles[]=
{
  &_rqpump,
  &_rqoyaA,
  &_rqoyaB,
  &_rqoyaC,
  &_rqoyaD
};

bool Master::nextRequest(void)
{
  curReq++;
  if (curReq>=sizeof(_cycles)/sizeof(Request *))
  {
    curReq=0;
    return false;
  }
  else
  {
    return true;
  }
}

Request * Master::getCurRequest(void)
{
  return _cycles[curReq];
}

Request * Master::itNext(int &pos)
{
  if ( pos >= sizeof(_cycles)/sizeof(Request *) )
  {
    return NULL;
  }
  else
  {    
    Request *pReq=_cycles[pos];
    pos++;
    return pReq;
  }
}

Request *Master::getRequestFrom(char addr)
{
  for (int i=0;i<sizeof(_cycles)/sizeof(Request *);i++)
    if (_cycles[i]->addr==addr)
      return _cycles[i];
      
  return NULL;
} 
