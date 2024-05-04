#include "master.h"
#include "prot.h"

void Master::init()
{
  pos=-1;
  pCurSlave=nullptr;

  list.init_all();
  nbcycles=0;
  tmrAnswer.stop();
  tmrWait.stop();  

  rxFrame.setReceiver(this);
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

void Master::enable_slaves(unsigned short ens)
{
  list.enable_slaves(ens);
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

bool Master::OnFrameReceive(FramePump *pump)
{
  log("Pompe");
  return false;
}

bool Master::OnFrameReceive(FrameOya *oya)
{
  log("Oya");
  return false;
}
   
bool Master::OnFrameReceive(FramePong *pong)
{
  log("pong");
  return false;
}
  
bool Master::loop(void)
{
  recv();
  
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
        log("tmt");

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
      if ( (res == FRAME_OK) || (res == UNHANDLED_FRAME) )
      {
        eState = WAIT;
        tmrWait.start();
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
