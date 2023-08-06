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
  char strVals[50]="";

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

	char tmp[20];
	sprintf(tmp,"%d;%d;",p->temp_dg,p->hum_pc);
	strcat(strVals,tmp);

	p=list.itNext(pos);
  }

  if (pump.comm_ok==true)
	ucValid|=1;

  if (pump.on==true)
	ucOn|=1;

  char strLine[200];
  sprintf(strLine,"%s;%02x,%02x,%02x,%02x;%d;",
		  strDteTime,
		  ucValid,
		  ucOn,
		  ucLow,
		  ucHigh,
		  pump.flow
		  );

  strcat(strLine,strVals);
  yield();

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
