/**
 * @file slaves.hpp
 * @brief Representation des états des esclaves (Pompe et Oyas)
*/
#ifndef SLAVES_HEADER_INCLUDED
#define SLAVES_HEADER_INCLUDED

#include <arduino.h>

class Slave
{
  public:
    char addr;
    bool on;
    int temp_dg;
    int hum_pc;
    bool enabled;

    bool comm_ok;
    
    unsigned long cycles_since_off;
    unsigned long cycles_since_on;
    unsigned long cycles_since_nok;
    unsigned long cycles_since_ok;
    unsigned long cycles_errors;

    bool cmd;

    Slave(char addr)
    {
      this->addr=addr;
      on=false;
      temp_dg=-1;
      hum_pc=-1;
      enabled=false;
      comm_ok=false;
      
      cycles_since_off=0;
      cycles_since_on=0;
      cycles_since_nok=0;
      cycles_since_ok=0;
      cycles_errors=0;

      cmd=false;
    }

    void setEnabled(bool en)
    {
      enabled=en;
    }

    void setOn(bool flgOn)
    {
      if ( on != flgOn )
      {
        cycles_since_on=0;
        cycles_since_off=0;
      }
      else if ( on == true )
      {
        cycles_since_on++;
      }
      else if ( on == false )
      {
        cycles_since_off++;
      }

      on=flgOn;
    }

    void setCommOk(bool flgCommOk)
    {
      if ( comm_ok != flgCommOk )
      {
        cycles_since_on=0;
        cycles_since_off=0;
        cycles_since_nok=0;
        cycles_since_ok=0;
      }
      else if ( comm_ok == true )
      {
        cycles_since_ok++;
      }
      else if ( comm_ok == false )
      {
        cycles_since_off++;
        cycles_since_nok++;
      }

      comm_ok=flgCommOk;
    }

    void setTemp_dg(int temp_dg)
    {
       this->temp_dg=temp_dg;
    }
    
    void setHum_pc(int hum_pc)
    {
       this->hum_pc=hum_pc;
    }

    void updCycleStats(void)
    {
      if (comm_ok==true)
      {
        if (on==false)
          cycles_since_off++;
        else
          cycles_since_on++;
          
        cycles_since_nok=0;
        cycles_since_ok++;
      }
      else
      {
        cycles_since_off=0;
        cycles_since_on=0;
        cycles_since_ok=0;
        cycles_since_nok++;
        cycles_errors++;
      }      
    }

    void razErrors(void)
    {
      cycles_errors=0;
    }
};

class Pump : public Slave
{
  public:
    int flow;

    Pump(char addr):Slave(addr) {flow=0;}

    void setFlow(int flow) {this->flow=flow;}
};


class Oya : public Slave
{
  public:
    bool high;
    bool low;

    Oya(char addr) : Slave(addr) {}  

    void setHigh(bool high) {this->high=high;}
    void setLow(bool low) {this->low=low;}
};


class OyasList
{
  private:
    Oya **oyas;
    int nbOyas;

  public:
    OyasList(Oya **listOyas,int nbOyas)
    {
      this->oyas=listOyas;
      this->nbOyas=nbOyas;
    }

    Oya * itNext(int &pos)
    {
      if (oyas==NULL)
        return NULL;
        
      if (pos<nbOyas)
      {
        return oyas[pos++];
      }
      else
      {
        return NULL;
      }
    }

    bool hasOneOyaOn(void)
    {
      if (oyas==NULL)
        return false;
        
      for (int i=0;i<nbOyas;i++)
      {
        if (oyas[i]->on==true)
        return true;
      }

      return false;
    }
    
    int count()
    {
      return nbOyas;
    }

    bool SetOn(char addr,bool on)
    {
      int pos=0;
      Oya *p=itNext(pos);
      while (p!=NULL)
      {
        if (p->addr==addr)
        {
          p->on=on;
          return true;
        }
        p=itNext(pos);
      }

      return false;
    }

    Oya *getOya(int pos)
    {
      if ( (oyas==NULL) || (nbOyas==0) )
        return NULL;

      if (pos<nbOyas)
        return oyas[pos];
      else
        return NULL;
    }
};

#endif
