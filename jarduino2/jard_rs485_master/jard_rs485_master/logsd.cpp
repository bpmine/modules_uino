#include "logsd.h"
#include "pins.h"

#include <RTClib.h>
#include <SPI.h>
#include <SD.h>

bool _sdOk=false;

bool logsd_init(void)
{
  SPI.begin();
  if (!SD.begin(SPI_HALF_SPEED, PIN_SD_CS))
  {
	Serial.println("SD init Failed!");
	_sdOk=false;
	return false;
  }

  _sdOk=true;
  return true;
}

bool logsd_log(DateTime &now,OyasList &list,Pump &pump)
{
  char strDteTime[25];
  sprintf(strDteTime,"%02d/%02d/%04d;%02d:%02d:%02d",
		  now.day(),
		  now.month(),
		  now.year(),
		  now.hour(),
		  now.minute(),
		  now.second()
		  );

  unsigned char ucValid=0;
  unsigned char ucOn=0;
  unsigned char ucHigh=0;
  unsigned char ucLow=0;
  unsigned char ucTempMin=255;
  unsigned char ucTempMoy=0;
  unsigned char ucTempMax=0;
  unsigned char ucHumMin=255;
  unsigned char ucHumMoy=0;
  unsigned char ucHumMax=0;
  long temp=0;
  long hum=0;
  int nbre=0;

  int pos=0;
  Oya *p=list.itNext(pos);
  while (p!=NULL)
  {
	int n=pos+1;

	if (p->comm_ok==true)
	  ucValid|=(1<<n);

	if (p->on==true)
	  ucOn|=(1<<n);

	if (p->low==true)
	  ucLow|=(1<<n);

	if (p->high==true)
	  ucHigh|=(1<<n);

	temp+=p->temp_dg;
	hum+=p->hum_pc;
	nbre++;

	if (p->temp_dg>ucTempMax)
	  ucTempMax=p->temp_dg;
	if (p->temp_dg<ucTempMin)
	  ucTempMin=p->temp_dg;

	if (p->hum_pc>ucHumMax)
	  ucHumMax=p->hum_pc;
	if (p->hum_pc<ucHumMin)
	  ucHumMin=p->hum_pc;

	p=list.itNext(pos);
  }

  if (pump.comm_ok==true)
	ucValid|=1;

  if (pump.on==true)
	ucOn|=1;

  temp+=pump.temp_dg;
  hum+=pump.hum_pc;
  nbre++;

  ucTempMoy=(unsigned char)(temp/nbre);
  ucHumMoy=(unsigned char)(hum/nbre);

  if (pump.temp_dg>ucTempMax)
	ucTempMax=pump.temp_dg;
  if (pump.temp_dg<ucTempMin)
	ucTempMin=pump.temp_dg;

  if (pump.hum_pc>ucHumMax)
	ucHumMax=pump.hum_pc;
  if (pump.hum_pc<ucHumMin)
	ucHumMin=pump.hum_pc;


  char strLine[200];
  sprintf(strLine,"%s;%02x,%02x,%02x,%02x;%d;%d;%d;%d;%d;%d;",
		  strDteTime,
		  ucValid,
		  ucOn,
		  ucLow,
		  ucHigh,
		  ucTempMin,
		  ucTempMoy,
		  ucTempMax,
		  ucHumMin,
		  ucHumMoy,
		  ucHumMax
		  );

  Serial.println(strLine);

  if (_sdOk==false)
  {
	Serial.println("LOG ERR!");
	return false;
  }

  File myFile = SD.open("LOG.TXT", FILE_WRITE);
  myFile.println(strLine);
  myFile.close();

  return true;
}
