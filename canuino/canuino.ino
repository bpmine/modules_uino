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

void serialEvent()
{
  if (!Serial.available())
    return;

  // TX:0000005502AA55000000000000
  String sCmd=Serial.readString();
  if (sCmd!=NULL)
  {
    if (    (sCmd.startsWith("TX:"))
         && (sCmd.length()>=29)
       )
    { 
      int dlcflgs=0;
      CANMessage frame;     
      int res=sscanf(sCmd.c_str(),"TX:%08x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
        &(frame.id),
        &dlcflgs,
        &(frame.data[0]),
        &(frame.data[1]),
        &(frame.data[2]),
        &(frame.data[3]),
        &(frame.data[4]),
        &(frame.data[5]),
        &(frame.data[6]),
        &(frame.data[7])
        );

        if (res!=10)
        {
          Serial.println("-ERR");
          return;
        }
        
        frame.len=dlcflgs&0x0F;
        frame.ext=((dlcflgs&0x10)==0x10?true:false);
        frame.rtr=((dlcflgs&0x20)==0x20?true:false);
        
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
    else if (    (sCmd.startsWith("SPEED:"))
         && (sCmd.length()>=31)
       )
    { 
      int speed=0;
      int res=sscanf(sCmd.c_str(),"SPEED:%d",&speed);
      if (res==1)
      {
        if (initCan(speed,false)==0)
        {
          Serial.println("+OK");
          return;
        }
      }
      Serial.println("-ERR");
    }
    else
    {
      Serial.println("-UNK");
    }
  }
}

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
}

void loop () 
{
  if (can.available ()) 
  {
    char strMsg[35];
    int dlcflgs=0;        
    
      CANMessage frame ;
      can.receive (frame) ;
      if (frame.ext==true)
        dlcflgs|=0x10;
      if (frame.rtr==true)
        dlcflgs|=0x20;

      dlcflgs|=(frame.len&0x0F);
        
      sprintf(strMsg,"RX:%08x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
      frame.id,
      dlcflgs,
      frame.data[0],
      frame.data[1],
      frame.data[2],
      frame.data[3],
      frame.data[4],
      frame.data[5],
      frame.data[6],
      frame.data[7]          
      );
      
      Serial.println(strMsg);
  }
}

//——————————————————————————————————————————————————————————————————————————————
