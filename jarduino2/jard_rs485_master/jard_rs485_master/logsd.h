#ifndef LOGGER_HEADER_INCLUDED
#define LOGGER_HEADER_INCLUDED

#include "slaves.hpp"
#include <RTClib.h>

extern bool logsd_init(void);
extern bool logsd_log(DateTime &now,OyasList &list,Pump &pump);

#endif
