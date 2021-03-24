#include "DHTesp.h"
#include <SD.h>
#include  <SPI.h>
#include <Wire.h>
#include <RTClib.h>

#define PIN_SDCARD_CS     10

#define DHT11_PIN         5

#define DELAY_S           (5*60U)

#define SB_FIRST_CYCLE    (0x01)
#define SB_NOSD           (0x02)
byte g_bitsSystem=0;

byte g_temp=0;
byte g_hum=0;
unsigned long tick0_ms;

DHTesp dht;
DS1307 RTC;

File dataFile;
char fn[10];

unsigned long getElapsedTimeFrom_ms(unsigned long tick0_ms)
{
  unsigned long delta_ms=0;
  unsigned long t=millis();

  if (t>=tick0_ms)
  {
    delta_ms=t-tick0_ms;
  }
  else
  {
    delta_ms=0xFFFFFFFF-tick0_ms+t;    
  }

  return delta_ms;  
}

void initFileName()
{
  DateTime dte = RTC.now();
  int year=(dte.year()-2000);
  sprintf(fn, "%02d%02d%02d.g",year,dte.month(),dte.day());

  Serial.print("Fichier:");
  Serial.println(fn);
}

void logBoot()
{
      File file=SD.open("bt.log", FILE_WRITE);
      if (file) 
      {
        char tmp[15];
        DateTime now = RTC.now();
        sprintf(tmp, "%02d/%02d/%04d",now.day() , now.month(), now.year());
        file.print(tmp);
        file.print(" ");

        sprintf(tmp, "%02d:%02d:%02d",now.hour() , now.minute(), now.second()); 
        file.print(tmp);
        file.println("> Boot");          

        file.close();
      }  
}


void printHeader()
{
    bool flg=SD.exists(fn);
    if (flg==false)
    {
      dataFile=SD.open(fn, FILE_WRITE);
      if (dataFile) 
      {
        dataFile.print("DATE;");
        dataFile.print("HEURE;");
        dataFile.print("TEMP;");
        dataFile.println("HUM;");
        
        dataFile.close(); 
      }
    }
}

void logLine(char *strLine)
{
  DateTime now = RTC.now();
  sprintf(strLine,"%02d/%02d/%04d;%02d:%02d:%02d;%d;%d;",
    now.day() ,
    now.month(), 
    now.year(),
    now.hour(), 
    now.minute(),
    now.second(),
    g_temp,
    g_hum
    );  
}

void writeToSD(char *strLine)
{
  if ((g_bitsSystem&SB_NOSD)==SB_NOSD)
    return;

  dataFile=SD.open(fn, FILE_WRITE);  
  if (dataFile) 
  {    
    dataFile.print(strLine);
    dataFile.print("\n");    
    
    dataFile.close();
  }
}

void printTime()
{
  char tmp[35];
  DateTime now = RTC.now();
  sprintf(tmp, "Time: %02d/%02d/%04d %02d:%02d:%02d",now.day() , now.month(), now.year(), now.hour(), now.minute(), now.second());
  Serial.println(tmp);    
}

void setup()
{
  bool flgNOSD=false;
  
  Serial.begin(9600);
  Serial.println("Boot");

  dht.setup(DHT11_PIN, DHTesp::DHT11);

  Wire.begin();
  RTC.begin();
  //RTC.adjust(DateTime((__DATE__), (__TIME__))); 
  printTime();

  delay(200);
  Serial.print("Find SD card: ");
  if (!SD.begin(PIN_SDCARD_CS))
  {
    Serial.println("[NOK]");
    flgNOSD=true;
  }  
  else
  {
    Serial.println("[OK]");
  }
  
  g_bitsSystem=SB_FIRST_CYCLE;
  if (flgNOSD)
    g_bitsSystem|=SB_NOSD;

  initFileName();
  tick0_ms=millis();
}

void latch_value(byte *i_pOutVal,byte i_iNewValue)
{
  byte old=(*i_pOutVal);
  if ( (old!=i_iNewValue) && ( (i_iNewValue!=0) || (abs(i_iNewValue-old)<2) ) )
  {
    (*i_pOutVal)=i_iNewValue;
  }
}

void cycle_inputs()
{
  TempAndHumidity newValues = dht.getTempAndHumidity();
  latch_value(&g_hum,(byte)newValues.humidity);
  latch_value(&g_temp,(byte)newValues.temperature);    
}
  
void loop() 
{
  char strLine[200];
  
  cycle_inputs();
  if (    ((g_bitsSystem&SB_FIRST_CYCLE)==SB_FIRST_CYCLE)
       || (getElapsedTimeFrom_ms(tick0_ms)>(DELAY_S*1000U))
       )
  {
    logLine(strLine);
    Serial.println(strLine);
    writeToSD(strLine);
    tick0_ms=millis();
  }

  delay(100);  

  g_bitsSystem=g_bitsSystem&(~SB_FIRST_CYCLE);  
}
