#include <ACAN2515.h>

static const byte MCP2515_CS  = 10 ; 
static const byte MCP2515_INT =  2 ;

ACAN2515 can (MCP2515_CS, SPI, MCP2515_INT) ;

static const uint32_t QUARTZ_FREQUENCY = 16UL * 1000UL * 1000UL ; // 16 MHz

void printCfg(ACAN2515Settings settings)
{
    Serial.print ("CFG: Bit Rate prescaler: ") ;
    Serial.println (settings.mBitRatePrescaler) ;
    Serial.print ("CFG: Propagation Segment: ") ;
    Serial.println (settings.mPropagationSegment) ;
    Serial.print ("CFG: Phase segment 1: ") ;
    Serial.println (settings.mPhaseSegment1) ;
    Serial.print ("CFG: Phase segment 2: ") ;
    Serial.println (settings.mPhaseSegment2) ;
    Serial.print ("CFG: SJW: ") ;
    Serial.println (settings.mSJW) ;
    Serial.print ("CFG: Triple Sampling: ") ;
    Serial.println (settings.mTripleSampling ? "yes" : "no") ;
    Serial.print ("CFG: Actual bit rate: ") ;
    Serial.print (settings.actualBitRate ()) ;
    Serial.println (" bit/s") ;
    Serial.print ("CFG: Exact bit rate ? ") ;
    Serial.println (settings.exactBitRate () ? "yes" : "no") ;
    Serial.print ("CFG: Sample point: ") ;
    Serial.print (settings.samplePointFromBitStart ()) ;
    Serial.println ("%") ;  
}

uint16_t initCan(long spd_kbs,bool verbose)
{
  Serial.println ("SYS:Configure ACAN2515") ;
  ACAN2515Settings settings (QUARTZ_FREQUENCY, spd_kbs * 1000UL) ; 
  settings.mRequestedMode = ACAN2515Settings::NormalMode ;
  
  const uint16_t errorCode = can.begin (settings, [] { can.isr () ; }) ;
  if (errorCode == 0) 
  {
    if (verbose==true)
    {
      printCfg(settings);      
    }
    return 0;
  }
  else
  {
    return errorCode;
  }  
}

unsigned long next=0;

void setup () 
{
  pinMode (LED_BUILTIN, OUTPUT) ;
  digitalWrite (LED_BUILTIN, HIGH) ;

  Serial.begin (38400) ;

  while (!Serial) 
  {
    delay (50) ;
    digitalWrite (LED_BUILTIN, !digitalRead (LED_BUILTIN)) ;
  }

  SPI.begin () ;

  int errorCode=initCan(125UL,true);
  if (errorCode!=0)
  {
    Serial.print ("SYS: Configuration error 0x") ;
    Serial.println (errorCode, HEX) ;    
  }

  next=millis();
}

#define U32 unsigned long
#define BYTE unsigned char
#define I16 short

void _fms_makeHourFrame(U32 *pID,BYTE *pDLC,BYTE *datas,U32 u32Hours)
{
  *pID=0xFEE500;
  *pDLC=8;
  u32Hours=(U32)(u32Hours/0.05);
  datas[0]=u32Hours&0xFF;
  datas[1]=(u32Hours>>8)&0xFF;
  datas[2]=(u32Hours>>16)&0xFF;
  datas[3]=(u32Hours>>24)&0xFF;  
}

void _fms_makeTempCool(U32 *pID,BYTE *pDLC,BYTE *datas,I16 i16TempCool)
{
  *pID=0xFEEE00;
  *pDLC=8;
  datas[0]=i16TempCool+40;
}

void _fms_makeTempAmbient(U32 *pID,BYTE *pDLC,BYTE *datas,U32 u32TempAmbiant)
{
  *pID=0xFEF500;
  *pDLC=8;
  u32TempAmbiant=u32TempAmbiant+273;
  u32TempAmbiant=(U32)(u32TempAmbiant/0.03125);
  datas[3]=u32TempAmbiant&0xFF;
  datas[4]=(u32TempAmbiant>>8)&0xFF;
}

void _fms_makeSpeedBrake(U32 *pID,BYTE *pDLC,BYTE *datas,BYTE bSpeed_kmph,BYTE flgBrake)
{
  *pID=0xFEFEF1;
  *pDLC=8;
  datas[2]=bSpeed_kmph;  
  
  datas[3]=0;
  if (flgBrake)
    datas[3]|=0x30;  
}

void hard_can_send_fms(U32 id,int dlc,BYTE *datas)
{
  CANMessage frame ;
  frame.ext=true;
  frame.rtr=false;
  frame.len=dlc;
  frame.id=id;
  for (int i=0;i<8;i++)
  {
    frame.data[i]=datas[i];
  }
  
  const bool ok = can.tryToSend (frame) ;
  if (ok)
  {
    Serial.println("+OK");
  }
  else
  {
    Serial.println("-ERR");
  }  
}
    


U32 g_u32Hours=0;
U32 g_i16TempCool_dg=75;
U32 g_i32TempAmbient_dg=24;
U32 bSpeed_kmph=45;

void _envoi_can(void)
{
  U32 id;
  BYTE dlc;
  BYTE datas[8];
 
  g_u32Hours++;
  _fms_makeHourFrame(&id,&dlc,datas,g_u32Hours);
  hard_can_send_fms(id,dlc,datas);
  _fms_makeTempCool(&id,&dlc,datas,g_i16TempCool_dg);
  hard_can_send_fms(id,dlc,datas);
  _fms_makeTempAmbient(&id,&dlc,datas,g_i32TempAmbient_dg);
  hard_can_send_fms(id,dlc,datas);
  _fms_makeSpeedBrake(&id,&dlc,datas,bSpeed_kmph,0);
  hard_can_send_fms(id,dlc,datas);
}


void loop () 
{
  if (millis()>=next)
  {
    _envoi_can();
    next=millis()+5000;
  }  
}

//——————————————————————————————————————————————————————————————————————————————
