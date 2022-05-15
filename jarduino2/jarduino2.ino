#include "timer.h"
#include "cmds.h"
#include "jard.h"
#include "pins.h"
#include "autom.h"

#include <DHT.h>

Jard jard;
DHT dhtSensor(PIN_DHT_DATA,DHT21);

T_PIN_MAPPING in_mapping[]=
{
  {PIN_BTN_PMP1,IB_BTN_PMP1},
  {PIN_BTN_PMP2,IB_BTN_PMP2},
  {PIN_BTN_ONOFF,IB_BTN_ON}  
};



T_PIN_MAPPING out_mapping[]=
{
  {PIN_CMD_PMP1,OB_CMD_PMP1},
  {PIN_CMD_PMP2,OB_CMD_PMP2},
  {PIN_LED_PMP1,OB_LED_PMP1},
  {PIN_LED_PMP2,OB_LED_PMP2},
  {PIN_LED_BATT,OB_LED_BATT},
  {PIN_LED_SUN,OB_LED_SUN},
  {PIN_LED_CPU,OB_LED_CPU}
};

void latch_inputs(void)
{
  mbs_inputs.start_latch();
  for (int i=0;i<sizeof(in_mapping)/sizeof(T_PIN_MAPPING);i++)
  {
    if (digitalRead(in_mapping[i].pin)==HIGH)
      mbs_inputs.set(in_mapping[i].bNumMB);
    else
      mbs_inputs.reset(in_mapping[i].bNumMB);
  }
  mbs_inputs.end_latch();
}

void apply_outputs(void)
{  
  for (int i=0;i<sizeof(out_mapping)/sizeof(T_PIN_MAPPING);i++)
  {
    digitalWrite(out_mapping[i].pin,mbs_outputs.get(out_mapping[i].bNumMB)==true?HIGH:LOW);
  } 
}

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

void serialEvent()
{
  cmds.onSerialEvent();
}

void setup()
{
  init_autom();
  
  jard.init();
  cmds.init(&Serial,&jard);

  char tmp[25];
  jard.getDateStr(tmp,25);  
  cmds.println(tmp);

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
  
  pinMode(PIN_BTN_PMP1,INPUT);
  pinMode(PIN_BTN_PMP2,INPUT);
  pinMode(PIN_BTN_ONOFF,INPUT);

  pinMode(PIN_DHT_DATA,INPUT_PULLUP);  

  pinMode(PIN_MES_POW_SUN,INPUT);
  pinMode(PIN_MES_POW_BATT,INPUT);
  
  digitalWrite(PIN_LED_PMP1,LOW);
  digitalWrite(PIN_LED_PMP2,LOW);
  digitalWrite(PIN_LED_BATT,LOW);
  digitalWrite(PIN_LED_SUN,LOW);
  digitalWrite(PIN_LED_CPU,LOW);
}

Analog anBatt(4);
Analog anSun(4);
void read_analog()
{
  unsigned short tmp=analogRead(PIN_MES_POW_BATT);
  unsigned short val=(unsigned short)((tmp*175L)/1023L);  
  anBatt.latch(val);

  tmp=analogRead(PIN_MES_POW_SUN);
  val=(unsigned short)((tmp*175L)/1023L);
  anSun.latch(val);
}

Analog anHum(4);
Analog anTemp(4);
void read_dht(void)
{
  float h = dhtSensor.readHumidity();
  if ( isnan(h) )
  {    
    anHum.reinit();
  }  
  else
  {
    anHum.latch(h);
  }
  
  float t = dhtSensor.readTemperature();
  if ( isnan(t) )
  {    
    anTemp.reinit();
  }  
  else
  {
    anTemp.latch(t);
  }  
}


void loop()
{     
  latch_inputs();
  read_analog();
  read_dht();
  
  jard.setBatLevel(anBatt.get());
  jard.setSunLevel(anSun.get());
  jard.setTemp(anTemp.get());
  jard.setHum(anHum.get());
  
  jard.loop();
  
  apply_outputs();
  delay(50);
}
