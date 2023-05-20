/*
 * pins.h
 */
#ifndef GLOBALS_INCLUDED
#define GLOBALS_INCLUDED

#define DEF_TIMEOUT   0x0001
#define DEF_WDG       0x0002

extern unsigned char g_bAddr;
extern unsigned char g_bFct;

extern bool g_cmd_ev;
extern bool g_cpt_low;
extern bool g_cpt_high;
extern bool g_enabled;

extern unsigned short g_defaults;

extern unsigned short g_mes_v;
extern unsigned short g_mes_i;
extern int g_flow_mLpMin;
extern unsigned char g_temp;
extern unsigned char g_hum;
extern unsigned long g_pump_s;


extern bool set_slave_addr(unsigned char bNewAddr);
extern bool set_slave_function(unsigned char bNewFunction);
extern void reset_comm_alive_timer(void);
extern void reset_stats(void);
extern void set_enabled(bool enabled);
#endif
