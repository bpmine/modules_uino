#include "master.h"
#include "prot.h"

void Master::init()
{
  int pos=-1;

  list.init_all();
  nbcycles=0;
  tmrAnswer.stop();
  tmrWait.stop();
  pCurSlave=nullptr;
}
    
Master::Master()
{   
  init();
  eState = OFF;
}

Master::~Master()
{
}

void Master::start_cycle(void)
{
  if ( eState==IDLE )
  {
	pCurSlave=list.findFirstSlave(pos);
	if (pCurSlave!=nullptr)
	  eState=SEND;
  }
}

void Master::setEnable(bool enable)
{
  if (enable==false)
    eState=OFF;
  else if (eState==OFF)
  {
    init();
    eState=IDLE;
  }
}
  
bool Master::loop(void)
{
  switch (eState)
  {
    case OFF:
    {
      nbcycles=0;
      list.init_all();
      break;
    }
    case IDLE:
    {
      tmrAnswer.start();
      break;
    }

    case SEND:
    {
      if (pCurSlave==nullptr)
      {
    	  eState=IDLE;
      }
      else
      {
        eState = RECV;
        rxFrame.reset();
        tmrAnswer.start();
        FrameBuilder fb;
        if (pCurSlave != nullptr)
        {
          FrameCmd cmd;
          cmd.commands = commands;
          cmd.addr = pCurSlave->addr;
          fb.build(&cmd);
          sendBytes(fb.getBuffer(), fb.size());
        }
      }

      break;
    }
        
    case RECV:
    {
      if (tmrAnswer.tick()==true)
      {
        /*if (flgTrace==true)
        {        
          char tmp[15];
          if (pCurSlave!=nullptr)
        	sprintf(tmp,"TMT: %c",pCurSlave->addr);
          else
        	strcpy(tmp,"TMT: X");

          Serial.println(tmp);
        }*/

        if (pCurSlave!=nullptr)
          pCurSlave->comm_ok=false;

        tmrWait.start();
        eState=WAIT;
      }

      break;
    }

    case WAIT:
    {
      if (tmrWait.tick()==true)
    	eState=NEXT;
      break;
    }

    case NEXT:
    {
      pCurSlave=list.findNextOya(pos);
      if (pCurSlave==nullptr)
      {
        eState=END;
      }
      else
      {

        eState=SEND;
      }
          
      break;          
    }


    case END:
    {
      tmrAnswer.start();
      FrameBuilder fb;
      FrameCmd cmd(commands,'S');
      fb.build(&cmd);
      sendBytes(fb.getBuffer(),fb.size());
      eState=IDLE;
      nbcycles++;
      return true;
    }
  }

  return false;
}  

void Master::recv(void)
{
  while (available())
  {
    int b = readByte();
    if (eState != RECV)
      continue;

    if ((b > 0) && (b < 255))
    {
      E_FRAME_ERR res = rxFrame.recv(b);
      if (res == FRAME_OK)
      {
        eState = WAIT;
      }
    }
  }
}

bool Master::isRunning(void)
{
  return eState==OFF?false:true;
}

unsigned long Master::cycles(void)
{
  return nbcycles;
}

void Master::setTrace(bool flgEnabled)
{
  flgTrace=flgEnabled;
}
