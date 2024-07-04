#include "DHTesp.h"
#include <SD.h>
#include <SPI.h>
#include <Wire.h>
#include <RTClib.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>

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
 * A3: Hum sol
*/

#define PIN_SDCARD_CS     10

#define DHT11_PIN         5
#define BATT_PIN          A1
#define VCC_PIN           A2
#define HUM_SOL_PIN       A6

int g_temp=0;
int g_hum=0;
int g_batt=0;
int g_vcc=0;
int g_capHum=0;

#define MAX_SEC_VALUE   (500)

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

int readCapHumidity(int pin)
{
  int val=analogRead(pin);

  if (val>MAX_SEC_VALUE)
    val=MAX_SEC_VALUE;

  int hum=(MAX_SEC_VALUE-val)*100/MAX_SEC_VALUE;

  return hum;
}

void initFileName()
{
  DateTime dte = RTC.now();
  int year=(dte.year()-2000);
  sprintf(fn, "%02d%02d%02d.g",year,dte.month(),dte.day());

  Serial.print("Fichier:");
  Serial.println(fn);
}

void writeToSD(void)
{
  Serial.print("Log: ");
  dataFile=SD.open(fn, FILE_WRITE);  
  if (dataFile) 
  {
    char strLine[80];
    DateTime now = RTC.now();    

    sprintf(strLine,"%02d/%02d/%04d;%02d:%02d:%02d;%d;%d;%d;",
      now.day() ,
      now.month(), 
      now.year(),
      now.hour(), 
      now.minute(),
      now.second(),
      g_temp,
      g_hum,
      g_capHum
      );

    dataFile.print(String(strLine));
    dataFile.print("\n");
    dataFile.close();
    Serial.println("[OK]");

    Serial.println(strLine);
  }
}

void printTime()
{
  char tmp[35];
  DateTime now = RTC.now();
  sprintf(tmp, "Time: %02d/%02d/%04d %02d:%02d:%02d",now.day() , now.month(), now.year(), now.hour(), now.minute(), now.second());
  Serial.println(tmp);    
}

void read_inputs()
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

  g_capHum=readCapHumidity(HUM_SOL_PIN);
}

void sleepDeep8s()
{
  pinMode(LED_BUILTIN,OUTPUT);
  digitalWrite(LED_BUILTIN,LOW);
  
  wdt_enable(WDTO_8S);
  WDTCSR |= (1<<WDCE) | (1<<WDE);
  WDTCSR |= _BV(WDIE);
  
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
    
  sleep_mode();
    
    /*sleep_disable();
    power_all_enable();  
    wdt_disable();    
    delay(2000);*/
}

void setup()
{
  wdt_disable();
  Wire.begin();
  RTC.begin();
  //RTC.adjust(DateTime((__DATE__), (__TIME__))); while(1);
  
  DateTime now = RTC.now();  
  if (now.second()>15)
    sleepDeep8s();

  wdt_disable();
  pinMode(BATT_PIN,INPUT);
  pinMode(VCC_PIN,INPUT);
  pinMode(HUM_SOL_PIN,INPUT);
  pinMode(LED_BUILTIN,OUTPUT);
  dht.setup(DHT11_PIN, DHTesp::DHT11);
  digitalWrite(LED_BUILTIN,HIGH);

  Serial.begin(9600);
  Serial.println("Boot");
  printTime();

  bool flgNOSD=false;
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

  if (flgNOSD==false)
  {
    initFileName();
    read_inputs();
    writeToSD();
  }

  delay(1000);
  while (now.second()<15)
  {
    delay(1000);
    now = RTC.now();    
  }  
  digitalWrite(LED_BUILTIN,LOW);  
  
  sleepDeep8s();
}

  
void loop() 
{
}
