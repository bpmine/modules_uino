/*
 * IJardCmd.h
 */
#ifndef IJARDCMD_INCLUDED
#define IJARDCMD_INCLUDED

class IJardCmd
{
public:
  virtual void aliveComm(void);
  virtual void setDate(int day,int month,int year)=0;
  virtual void setHour(int hour,int minute)=0;
  virtual void getDateStr(char *o_strDate,int i_maxSize)=0;
  virtual void getDate(unsigned short *o_pusYear,unsigned char *o_pucMonth,unsigned char *o_pucDay,unsigned char *o_pucHour,unsigned char *o_pucMin,unsigned char *o_pucSec)=0;
  virtual void getHour(unsigned char *o_pucHour,unsigned char *o_pucMin,unsigned char *o_pucSec)=0;

  virtual void setSheduler(int num,unsigned char ucStartHour,unsigned char ucStartMin,unsigned char ucDuration_min,unsigned char ucDaysOfWeek)=0;
  virtual void getSheduler(int num,unsigned char *o_ucStartHour,unsigned char *o_ucStartMin,unsigned char *o_ucDuration_min,unsigned char *o_ucDaysOfWeek)=0;

  virtual unsigned short getBattLevel(void)=0;
  virtual unsigned short getSunLevel(void)=0;
  virtual int getTemp(void)=0;
  virtual unsigned char getHum(void)=0;

  virtual void load(void)=0;
};

#endif
