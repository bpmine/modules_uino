/*
 * pins.h
 */
#ifndef GLOBALS_INCLUDED
#define GLOBALS_INCLUDED

#define DEF_TIMEOUT   0x0001
#define DEF_WDG       0x0002

extern bool g_cmd_ev;
extern bool g_cpt_low;
extern bool g_cpt_high;
extern bool g_enabled;

extern unsigned short g_defaults;

extern unsigned short g_mes_v;
extern unsigned short g_mes_i;

extern bool set_slave_addr(unsigned char bNewAddr);
extern void reset_comm_alive_timer(void);

#endif
