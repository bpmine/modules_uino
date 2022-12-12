/*
 * pins.h
 */
#ifndef GLOBALS_INCLUDED
#define GLOBALS_INCLUDED

extern bool g_cmd_ev;
extern bool g_cpt_low;
extern bool g_cpt_high;

extern bool set_slave_addr(unsigned char bNewAddr);
extern void resetWdg(void);

#endif
