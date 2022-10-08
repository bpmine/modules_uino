#include "DHTesp.h"
#include <SD.h>
#include  <SPI.h>
#include <Wire.h>
#include <RTClib.h>

/**
 * 
 * A4: RTC SDA
 * A5: RTC_SCL
 * 
 * D10: SD_CS
 * D11: SD_MOSI
 * D12: SD_MISO
 * D13: SD_SCK
 * 
 * D5: DHT11
 * A1: BATT
 * A2: VCC
*/

#define PIN_SDCARD_CS     10

#define DHT11_PIN         5
#define BATT_PIN          A1
#define VCC_PIN           A2

#define DELAY_MS           (60UL*1000UL)

#define SB_FIRST_CYCLE    (0x01)
#define SB_NOSD           (0x02)
byte g_bitsSystem=0;

int g_temp=0;
int g_hum=0;
int g_batt=0;
int g_vcc=0;
unsigned long g_tick0_ms;
char strLine[120];

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
  sprintf(strLine,"%02d/%02d/%04d;%02d:%02d:%02d;%d;%d;%d;%d;",
    now.day() ,
    now.month(), 
    now.year(),
    now.hour(), 
    now.minute(),
    now.second(),
    g_temp,
    g_hum,
    g_batt,
    g_vcc
    );  
}

void logBoot()
{
  if ((g_bitsSystem&SB_NOSD)==SB_NOSD)
    return;

  Serial.println("Log boot");
  
  File file=SD.open("boot.log", FILE_WRITE);
  if (file) 
  {
    logLine(strLine);
    file.println(strLine);
    file.close();
  }  
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

  pinMode(BATT_PIN,INPUT);
  pinMode(VCC_PIN,INPUT);

  logBoot();
  
  g_bitsSystem=SB_FIRST_CYCLE;
  if (flgNOSD)
    g_bitsSystem|=SB_NOSD;

  initFileName();
  g_tick0_ms=millis();
}

void cycle_inputs()
{
  TempAndHumidity newValues = dht.getTempAndHumidity();
  if (dht.getStatus()==DHTesp::ERROR_NONE)
  {
    g_hum=newValues.humidity;
    g_temp=newValues.temperature;
  }
  else
  {
    g_hum=-100;
    g_temp=-100;
  }

  g_batt=analogRead(BATT_PIN);
  g_vcc=analogRead(VCC_PIN);
}
  
void loop() 
{   
  cycle_inputs();
  if (    ((g_bitsSystem&SB_FIRST_CYCLE)==SB_FIRST_CYCLE)
       || (getElapsedTimeFrom_ms(g_tick0_ms)>DELAY_MS)
       )
  {
    logLine(strLine);
    Serial.println(strLine);
    writeToSD(strLine);
    g_tick0_ms=millis();
  }

  delay(100);  

  g_bitsSystem=g_bitsSystem&(~SB_FIRST_CYCLE);  
}
