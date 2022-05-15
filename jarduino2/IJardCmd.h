/*
 * IJardCmd.h
 */
#ifndef IJARDCMD_INCLUDED
#define IJARDCMD_INCLUDED

class IJardCmd
{
public:
  virtual void setDate(int day,int month,int year)=0;
  virtual void setHour(int hour,int minute)=0;
  virtual void getDateStr(char *o_strDate,int i_maxSize)=0;
  virtual void forcePompe(int i_iNumPompe,bool i_flgON)=0;
  virtual void enablePompe(int i_iNumPompe,bool i_flgEnabled)=0;
  virtual void setAuto(int i_iNumPompe,bool i_flgAuto)=0;

  virtual unsigned short getBattLevel(void)=0;
  virtual unsigned short getSunLevel(void)=0;
  virtual int getTemp(void)=0;
  virtual unsigned char getHum(void)=0;

  virtual void save(void)=0;
  virtual void load(void)=0;
};

#endif
