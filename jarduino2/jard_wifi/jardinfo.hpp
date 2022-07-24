#ifndef HEADER_INCLUDED_JARDINFO
#define HEADER_INCLUDED_JARDINFO

#include "pumpinfo.hpp"

class JardInfo
{
  public:
    unsigned short version_jarduino=0;
    unsigned short serial=0;
    unsigned short version_soft=0;
    
    unsigned short batt_dxV=0;
    unsigned short sun_dxV=0;
    unsigned short temp_deg=0;
    unsigned short hum_pc=0;
    
    unsigned short year=0;
    unsigned short month=0;
    unsigned short day=0;
    unsigned short hour=0;
    unsigned short minute=0;
    unsigned short second=0;

    PumpInfo pump1;
    PumpInfo pump2;
    
    JardInfo()
    {
    }
};

#endif
