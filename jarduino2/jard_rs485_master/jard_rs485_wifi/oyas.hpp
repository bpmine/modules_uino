#ifndef OYAS_HEADER_INCLUDES
#define OYAS_HEADER_INCLUDES

#define NB_SLAVES_MAX   (14)
#define DATE_BUFF_SIZE  (22)

#include <ArduinoJson.h>

class OyasInfo
{
  public:
    unsigned short config_slaves;
    unsigned short commands;
    unsigned short ons;
    unsigned short lows;
    unsigned short highs;
    unsigned short flow;
    unsigned short comms_ok;

    unsigned short ticks_ms[NB_SLAVES_MAX];
    unsigned short totals_s[NB_SLAVES_MAX];
    unsigned short voltages[NB_SLAVES_MAX];
    unsigned short temps[NB_SLAVES_MAX];
    unsigned short hums[NB_SLAVES_MAX];

    char date[DATE_BUFF_SIZE];

    void copyFrom(OyasInfo &src)
    {
      config_slaves=src.config_slaves;
      commands=src.commands;
      ons=src.ons;
      lows=src.lows;
      highs=src.highs;
      flow=src.flow;
      comms_ok=src.comms_ok;
      strncpy(date,src.date,DATE_BUFF_SIZE);
      date[DATE_BUFF_SIZE-1]=0;

      for (int i=0;i<NB_SLAVES_MAX;i++)
      {
        ticks_ms[i]=src.ticks_ms[i];
        totals_s[i]=src.totals_s[i];
        voltages[i]=src.voltages[i];
        temps[i]=src.temps[i];
        hums[i]=src.hums[i];
      }
    }
    
    OyasInfo()
    {
      config_slaves=0;
      commands=0;
      ons=0;
      lows=0;
      highs=0;
      flow=0;
      comms_ok=0;
      strcpy(date,"");

      for (int i=0;i<NB_SLAVES_MAX;i++)
      {
        ticks_ms[i]=0;
        totals_s[i]=0;
        voltages[i]=0;
        temps[i]=0;
        hums[i]=0;
      }
    }

    void makeJsonTxt(char *txt,int szMax)
    {
      StaticJsonDocument<2000> doc;
      doc["type"]="oyas";      

      JsonObject master= doc.createNestedObject("master");
      master["config_slaves"]=config_slaves;
      master["commands"]=commands;
      master["ons"]=ons;
      master["lows"]=lows;
      master["highs"]=highs;
      master["flow"]=flow;
      master["comms_ok"]=comms_ok;
      master["date"]=date;
      
      JsonArray list = doc.createNestedArray("slaves");
      for (int i=0;i<NB_SLAVES_MAX;i++)
      {
        unsigned short mask= 1 << i;
        if ((config_slaves & mask) == mask)
        {
          JsonObject slv= list.createNestedObject();
          slv["addr"]=i+1;
          if ( (comms_ok & mask) == mask)
          {
            slv["comm_ok"]=true;
            slv["on"]=(ons&mask)==mask?true:false;
            slv["cmd"]=(commands&mask)==mask?true:false;
            slv["temp"]=temps[i];
            slv["hum"]=hums[i];
            slv["pwr"]=voltages[i];
            slv["tick"]=ticks_ms[i];
            slv["total_on_s"]=totals_s[i];

            if (i==0)
            {
              slv["flow"]=flow;
            }
            else
            {
              slv["low"]=(lows&mask)==mask ? true:false;
              slv["high"]=(highs&mask)==mask ? true:false;
            }            
          }
          else
          {
            slv["comm_ok"]=false;
          }
        }
      }
      
      serializeJson(doc,txt,szMax);
    }
};

#endif
