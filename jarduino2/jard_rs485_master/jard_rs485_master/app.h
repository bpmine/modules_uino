/**
 * @file app.h
 * @brief EN-TETE - Application du maître des OYAs
 * */
#ifndef APP_HEADER_INCLUDED
#define APP_HEADER_INCLUDED

class Pump;
class Oya;

extern void app_term_trace(bool en);

extern void app_term_master(bool on);
extern Pump *app_term_get_pump(void);
extern Oya *app_term_find_first_oya(int &pos);
extern Oya *app_term_find_next_oya(int &pos);

extern void app_set_pompe(bool on);
extern void app_set_oya(char addr,bool on);

extern void app_get_date_hour(int &day,int &month,int &year,int &hour,int &minute,int &second);
extern void app_get_date_hour(int &day,int &month,int &year,int &hour,int &minute,int &second,int &dow);
extern void app_set_date(int day,int month,int year);
extern void app_set_hour(int hour,int minute,int second);

extern void app_set_slaves_config(unsigned short config);
extern unsigned short app_get_slaves_config(void);

#endif
