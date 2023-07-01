#ifndef APP_HEADER_INCLUDED
#define APP_HEADER_INCLUDED

#include "slaves.hpp"
#include "pins.h"

extern bool g_trace_on;

extern void app_init(void);
extern void app_loop();

extern void app_term_master(bool on);
extern Oya * app_term_get_next_oya(int &pos);
extern Pump * app_term_get_pump(void);
extern void app_term_log(bool on);
extern void app_term_trace(bool on);
extern bool app_slave(char addr,bool on);
extern void app_set_date(int year,int month,int day);
extern void app_set_time(int hour,int minute,int second);

#endif
