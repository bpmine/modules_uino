//#include "DHTesp.h"
#include <SD.h>
//#include  <SPI.h>
#include <Wire.h>
#include <RTClib.h>
#include "timer.h"

#define POW_SUN_LEVEL_HIGH  (130)
#define POW_SUN_LEVEL_LOW   (125)
#define POW_BATT_LEVEL_HIGH (115)
#define POW_BATT_LEVEL_LOW  (105)

#define DELAY_S             (5U*60U)


#define PIN_SDCARD_CS     (10)

#define PIN_CMD_PMP1      (3)
#define PIN_CMD_PMP2      (2)
#define PIN_LED_PMP1      (4)
#define PIN_LED_PMP2      (5)
#define PIN_LED_BATT      (6)
#define PIN_LED_SUN       (7)
#define PIN_LED_CPU       (8)

#define PIN_BTN_PMP1      (12)
#define PIN_BTN_PMP2      (11)
#define PIN_BTN_ONOFF     (9)

#define PIN_MES_POW_SUN   (A0)
#define PIN_MES_POW_BATT  (A1)


#define SB_FIRST_CYCLE    (0x01)
#define SB_NOSD           (0x02)
byte g_bitsSystem=0;

byte g_temp=0;
byte g_hum=0;
unsigned long tick0_ms;

bool g_btnOnOff=false;
bool g_btnPmp1=false;
bool g_btnPmp2=false;
bool g_dryPeriod=false;

int g_powBatt=0;
int g_powSun=0;

bool g_cmdPmp1=false;
bool g_cmdPmp2=false;

bool g_flgLevelBattOk=true;
bool g_flgLevelSunOk=true;


unsigned long g_cnt_ms=0;
bool g_blink_500ms=false;
bool g_blink_1s=false;

Timer tmrBlink(250UL);
Timer tmrPmp1(10000UL,true);
Timer tmrPmp2(2UL*60UL*1000UL,true);

//DHTesp dht;
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
        dataFile.print("BATT;");
        dataFile.println("SUN;");
        dataFile.println("STAT;");
        
        dataFile.close(); 
      }
    }
}

void logLine(char *strLine)
{
  byte flgIHM=0;

  if (g_btnOnOff==true)
    flgIHM|=0x01;
  if (g_btnPmp1==true)
    flgIHM|=0x02;
  if (g_btnPmp2==true)
    flgIHM|=0x04;
    
  if (g_dryPeriod==true)
    flgIHM|=0x08;

  if (g_cmdPmp1==true)
    flgIHM|=0x10;
  if (g_cmdPmp2==true)
    flgIHM|=0x20;    

  DateTime now = RTC.now();
  sprintf(strLine,"%02d/%02d/%04d;%02d:%02d:%02d;%d;%d;%02x;",
    now.day() ,
    now.month(), 
    now.year(),
    now.hour(), 
    now.minute(),
    now.second(),
    g_powBatt,
    g_powSun,
    flgIHM
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

  pinMode(PIN_CMD_PMP1,OUTPUT);
  digitalWrite(PIN_CMD_PMP1,LOW);
  pinMode(PIN_CMD_PMP2,OUTPUT);
  digitalWrite(PIN_CMD_PMP2,LOW);
  
  pinMode(PIN_LED_PMP1,OUTPUT);
  digitalWrite(PIN_LED_PMP1,HIGH);
  pinMode(PIN_LED_PMP2,OUTPUT);
  digitalWrite(PIN_LED_PMP2,HIGH);

  pinMode(PIN_LED_BATT,OUTPUT);
  digitalWrite(PIN_LED_BATT,HIGH);
  pinMode(PIN_LED_SUN,OUTPUT);
  digitalWrite(PIN_LED_SUN,HIGH);
  pinMode(PIN_LED_CPU,OUTPUT);
  digitalWrite(PIN_LED_CPU,HIGH);
  
  pinMode(PIN_BTN_PMP1,INPUT_PULLUP);
  pinMode(PIN_BTN_PMP2,INPUT_PULLUP);
  pinMode(PIN_BTN_ONOFF,INPUT_PULLUP);

  pinMode(PIN_MES_POW_SUN,INPUT);
  pinMode(PIN_MES_POW_BATT,INPUT);

  //dht.setup(DHT11_PIN, DHTesp::DHT11);

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

  delay(200);
  digitalWrite(PIN_LED_PMP1,LOW);
  digitalWrite(PIN_LED_PMP2,LOW);
  digitalWrite(PIN_LED_BATT,LOW);
  digitalWrite(PIN_LED_SUN,LOW);
  digitalWrite(PIN_LED_CPU,LOW);    

  initFileName();
  tick0_ms=millis();

  tmrBlink.start();
}

/*void latch_value(byte *i_pOutVal,byte i_iNewValue)
{
  byte old=(*i_pOutVal);
  if ( (old!=i_iNewValue) && ( (i_iNewValue!=0) || (abs(i_iNewValue-old)<2) ) )
  {
    (*i_pOutVal)=i_iNewValue;
  }
}*/

void calcPowLevels()
{
  if ( (g_powSun>=POW_SUN_LEVEL_HIGH) )
  {
    g_flgLevelSunOk=true;
  }
  else if (( (g_powSun<=POW_SUN_LEVEL_LOW) ))
  {
    g_flgLevelSunOk=false;    
  }

  if ( (g_powBatt>=POW_BATT_LEVEL_HIGH) )
  {
    g_flgLevelBattOk=true;
  }
  else if (( (g_powBatt<=POW_BATT_LEVEL_LOW) ))
  {
    g_flgLevelBattOk=false;    
  }  
}

void cycle_inputs()
{
  //TempAndHumidity newValues = dht.getTempAndHumidity();
  //latch_value(&g_hum,(byte)newValues.humidity);
  //latch_value(&g_temp,(byte)newValues.temperature);    

  DateTime now = RTC.now();
  g_dryPeriod=false;
  if ( (now.hour()==7) || (now.hour()==19) )
  { 
    if ( (now.minute()>=0) && (now.minute()<=15) )
    {
      g_dryPeriod=true;      
    }
  }

  g_btnPmp1=digitalRead(PIN_BTN_PMP1)==0?true:false;
  g_btnPmp2=digitalRead(PIN_BTN_PMP2)==0?true:false;
  g_btnOnOff=digitalRead(PIN_BTN_ONOFF)==0?true:false;

  long tmp=analogRead(PIN_MES_POW_SUN);
  g_powSun=(int)((tmp*150L)/1023L);  
  

  tmp=analogRead(PIN_MES_POW_BATT);
  g_powBatt=(int)((tmp*150L)/1023L);
}
  
void loop() 
{
  bool flgPmp1Tmr=false;
  bool flgPmp2Tmr=false;
  
  char strLine[200];
  
  cycle_inputs();

  if (g_btnPmp1 && g_btnOnOff)
    tmrPmp1.start();

  if (g_btnPmp2 && g_btnOnOff)
    tmrPmp2.start();

  if (g_btnOnOff==false)
  {
    tmrPmp1.stop();
    tmrPmp2.stop();
  }

  flgPmp1Tmr=tmrPmp1.isRunning();
  flgPmp2Tmr=tmrPmp2.isRunning();

  g_cmdPmp1=(flgPmp1Tmr || g_dryPeriod || flgPmp2Tmr) && g_btnOnOff;
  //g_cmdPmp2=(flgPmp2Tmr || g_dryPeriod) && g_btnOnOff;
  g_cmdPmp2=false;

  calcPowLevels();
  

  // Log line every...
  if (    ((g_bitsSystem&SB_FIRST_CYCLE)==SB_FIRST_CYCLE)
       || (getElapsedTimeFrom_ms(tick0_ms)>(DELAY_S*1000U))
       )
  {
    logLine(strLine);
    Serial.println(strLine);
    writeToSD(strLine);
    tick0_ms=millis();
  }

  digitalWrite(PIN_CMD_PMP1,g_cmdPmp1==true?HIGH:LOW);
  digitalWrite(PIN_CMD_PMP2,g_cmdPmp2==true?HIGH:LOW);
  digitalWrite(PIN_LED_PMP1,g_cmdPmp1==true?HIGH:LOW);
  digitalWrite(PIN_LED_PMP2,g_cmdPmp2==true?HIGH:LOW);
  
  digitalWrite(PIN_LED_CPU,g_btnOnOff==true?HIGH:LOW);
  digitalWrite(PIN_LED_SUN,g_flgLevelSunOk==true?HIGH:LOW);
  digitalWrite(PIN_LED_BATT,g_flgLevelBattOk==true?HIGH:LOW);  

  delay(100);  

  g_bitsSystem=g_bitsSystem&(~SB_FIRST_CYCLE);  

  if (tmrBlink.tick()==true)
  {
    g_cnt_ms+=250;
    
    if ((g_cnt_ms%500)==0)
      g_blink_500ms=!g_blink_500ms;
    if ((g_cnt_ms%1000)==0)
      g_blink_1s=!g_blink_1s;
  }
  
  tmrPmp1.tick();
  tmrPmp2.tick();
}
