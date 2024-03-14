/**
 * @file semuino.h
 * @brief Gestion de l'application semuino (avec ESP01)
*/
#ifndef SEMUINO_HEADER_INCLUDED
#define SEMUINO_HEADER_INCLUDED

extern void semuino_set_time(int h,int m,int s);
extern void semuino_set_date(int d,int m,int y);

extern void semuino_set_auto(bool enabled);
extern void semuino_set_auto_settings(int h_sunrise,int h_sunset,int mode_rgb1,int mode_rgb2,int mode_rgb3);

extern void semuino_manual_set_p1(bool on);
extern void semuino_manual_set_p2(bool on);
extern void semuino_manual_set_p3(bool on);

extern void semuino_manual_set_power_5v(bool on);
extern void semuino_manual_set_rgb1(bool on);
extern void semuino_manual_set_rgb2(bool on);
extern void semuino_manual_set_rgb3(bool on);
extern void semuino_manual_set_rgb1_mode(int mode);
extern void semuino_manual_set_rgb2_mode(int mode);
extern void semuino_manual_set_rgb3_mode(int mode);

extern void semuino_init(void);
extern void semuino_loop(void);

#endif
