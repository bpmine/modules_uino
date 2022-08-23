#include <ModbusRTU.h>
#include "jardinfo.hpp"

#define MODBUS_SLAVE_ADDRESS  1

ModbusRTU mb;


JardInfo jarduino;
JardInfo jarduino_latched;

bool tmpBool[10];
unsigned short tmpReg[10];

bool cbModbus_ReadCoils_pmp1(Modbus::ResultCode event, uint16_t transactionId, void* data)
{  
  if (event==Modbus::EX_SUCCESS)
  {    
    jarduino.pump1.flgForce=tmpBool[0];
    jarduino.pump1.flgEnabled=tmpBool[1];
    jarduino.pump1.flgAuto=tmpBool[2];
    jarduino.pump1.flgRemote=tmpBool[3];    
  }
  return true;
}

bool cbModbus_ReadCoils_pmp2(Modbus::ResultCode event, uint16_t transactionId, void* data)
{  
  if (event==Modbus::EX_SUCCESS)
  {    
    jarduino.pump2.flgForce=tmpBool[0];
    jarduino.pump2.flgEnabled=tmpBool[1];
    jarduino.pump2.flgAuto=tmpBool[2];
    jarduino.pump2.flgRemote=tmpBool[3];    
  }
  
  return true;
}

bool cbModbus_ReadHoldingReg_Hour(Modbus::ResultCode event, uint16_t transactionId, void* data)
{  
  if (event==Modbus::EX_SUCCESS)
  {    
    jarduino.year=tmpReg[0];
    jarduino.month=tmpReg[1];
    jarduino.day=tmpReg[2];

    jarduino.hour=tmpReg[3];
    jarduino.minute=tmpReg[4];
    jarduino.second=tmpReg[5];
  }
  
  return true;
}

bool cbModbus_ReadInputReg_Measures(Modbus::ResultCode event, uint16_t transactionId, void* data)
{  
  if (event==Modbus::EX_SUCCESS)
  {    
    jarduino.batt_dxV=tmpReg[0];
    jarduino.sun_dxV=tmpReg[1];
    jarduino.temp_deg=tmpReg[2];
    jarduino.hum_pc=tmpReg[3];
  }
  
  return true;
}


void waitReadSlaveInfo(void)
{
  while (1)
  {
    digitalWrite(LED_BUILTIN,LOW);

    static uint16_t tmp[5];
    mb.readIreg(MODBUS_SLAVE_ADDRESS, 200, tmp, 3);
    while (mb.slave())
      mb.task();
    
    jarduino.version_jarduino=tmp[0];
    jarduino.serial=tmp[1];
    jarduino.version_soft=tmp[2];
    digitalWrite(LED_BUILTIN,HIGH);
    
    if (jarduino.version_jarduino==2)
      break;

    for (int i=0;i<4;i++)
    {
      digitalWrite(LED_BUILTIN,LOW);
      delay(100);
      digitalWrite(LED_BUILTIN,HIGH);
      delay(100);      
    }
    
    delay(2000);
  }
  
  while (mb.slave())
    mb.task();
}


void readSlaveData(void)
{  
  mb.readHreg(MODBUS_SLAVE_ADDRESS, 0, tmpReg, 6,cbModbus_ReadHoldingReg_Hour);
  while (mb.slave())
  {
    mb.task();
    yield();
  }

  mb.readIreg(MODBUS_SLAVE_ADDRESS, 0, tmpReg, 4,cbModbus_ReadInputReg_Measures);
  while (mb.slave())
  {
    mb.task();
    yield();
  }

  mb.readCoil(MODBUS_SLAVE_ADDRESS, 10, tmpBool, 4,cbModbus_ReadCoils_pmp1);
  while (mb.slave())
  {
    mb.task();
    yield();
  }

  mb.readCoil(MODBUS_SLAVE_ADDRESS, 20, tmpBool, 4,cbModbus_ReadCoils_pmp2);
  while (mb.slave())
  {
    mb.task();
    yield();
  }
}

void writeCoil(unsigned short usAddr,bool flgValue)
{
  static bool tmp[2];
  tmp[0]=flgValue;
  mb.writeCoil(MODBUS_SLAVE_ADDRESS,usAddr, tmp, 1);
  while (mb.slave())
  {
    mb.task();
    yield();
  }  
}

void setup() 
{
  Serial.begin(115200);
  mb.begin(&Serial);
  
  mb.master();

  pinMode(LED_BUILTIN,OUTPUT);
  digitalWrite(LED_BUILTIN,LOW);
  delay(500);
  digitalWrite(LED_BUILTIN,HIGH);
  delay(500);    

  waitReadSlaveInfo();
}

void loop() 
{
  static bool state=true;
      
  digitalWrite(LED_BUILTIN,LOW);
  readSlaveData();
  digitalWrite(LED_BUILTIN,HIGH);

  memcpy(&jarduino_latched,&jarduino,sizeof(jarduino));
  delay(4000);

  writeCoil(10,state);
  state=!state;
}
