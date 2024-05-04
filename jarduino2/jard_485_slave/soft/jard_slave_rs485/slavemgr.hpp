#ifndef SLAVEMGR_HEADER_INCLUDED
#define SLAVEMGR_HEADER_INCLUDED

#include "frames.hpp"
#include "framebuilder.h"
#include "globals.h"

#define	SLAVE_CMD_TIMEOUT_MS	(4 * 1000UL)
#define SERIAL_SPEED          (9600)

class SlaveMgr : IFrameReceiver
{
  private:
  	unsigned long tick_last_comm_ms;
  	bool flgCommOk;
  	bool flgEndOfCycle;
  	unsigned char addr;
  	bool flgCmd;

    void FillInCommonFields(FrameSlave *pFrmSlave)
    {
  	  pFrmSlave->addr=addr;
  	  pFrmSlave->hum=g_hum_pc;
  	  pFrmSlave->temp=g_temp_dg;
      pFrmSlave->voltage=g_mes_cv;
  	  pFrmSlave->tick_ms=getTick()%0xFFFF;
  	  pFrmSlave->total_time_s=g_total_s;
  	  pFrmSlave->total_errs=g_errors;
  
  	  pFrmSlave->setCmd(flgCmd).setOn(g_on);
    }

  protected:
  	int tx_pin;
  	FrameBuilder rxFrame;
  
  	virtual void send(unsigned char *buffer,int size)=0;
  	virtual unsigned long getTick(void)=0;

  public:
	  SlaveMgr()
    {
  		flgEndOfCycle=false;
  		addr=0;
  		tx_pin=0;
  		tick_last_comm_ms=0;
  		flgCommOk=false;
  		flgCmd=false;
	  }

    virtual ~SlaveMgr()
    {
    }

    virtual void onSerialEvent(void)=0;

    bool OnFrameReceive(FramePing* pFrmPing) override
    {
    	if (addr==pFrmPing->addr)
    	{
    	  FrameBuilder fb;
    	  FramePong pong(addr,~pFrmPing->value);
    	  fb.build(&pong);
    	  send(fb.getBuffer(),fb.size());

    	  return true;
    	}

    	return false;
    }

    bool OnFrameReceive(FrameCmd* pFrmCmd) override
    {
    	/// On applique systematiquement les commandes
		  unsigned short mask = (1<<addr) & (0x7FFE);

		  if ((pFrmCmd->commands&mask)==mask)
		  {
		    flgCmd=true;
  		}
  		else
  		{
  		  flgCmd=false;
  		}

  		tick_last_comm_ms=getTick();
  		flgCommOk=true;
  
  		/// On r�pond une trame si l'adresse de l'esclave est demandee
      	if (addr==pFrmCmd->addr)
      	{
      	  if (addr==1)
      	  {
        		FrameBuilder fb;
        		FramePump pump;
    
        		FillInCommonFields(&pump);
        		pump.flow=g_flow_mLpMin;
    
        		fb.build(&pump);
        		send(fb.getBuffer(),fb.size());
      	  }
      	  else if ( (addr>1) && (addr<15) )
      	  {
        		FrameBuilder fb;
        		FrameOya oya;
    
        		FillInCommonFields(&oya);
        		oya.setLow(g_cpt_low).setHigh(g_cpt_high);
    
        		fb.build(&oya);
        		send(fb.getBuffer(),fb.size());
      	  }
      	}
      	/// On finalise le cycle si l'adresse est celle de sync (initialement 'S')
      	else if (pFrmCmd->addr==ADDR_SYNC)
      	{
      	  flgEndOfCycle=true;
      	}
  
      	return true;
      }

      virtual void begin(int tx_pin,unsigned char addr)
      {
      	this->tx_pin=tx_pin;
  
      	if (addr<=15)
      	  this->addr=addr;
      	else
      	  this->addr=0;
  
      	flgCommOk=false;
      	flgEndOfCycle=false;
      	flgCmd=false;
        rxFrame.setReceiver(this);
      }

      bool loop(void)
      {
      	if (flgCommOk==true)
      	{
    		  unsigned long ulTick=getTick();
    		  unsigned long delta_ms;
    		  if (ulTick>=tick_last_comm_ms)
      			delta_ms=ulTick-tick_last_comm_ms;
    		  else
      			delta_ms=0xFFFFFFFF-tick_last_comm_ms+ulTick;
  
    		  if (delta_ms>SLAVE_CMD_TIMEOUT_MS)
    		  {
      			flgCommOk=false;
      			return false;
    		  }
      	}

      	if (flgEndOfCycle==true)
      	{
      	  flgEndOfCycle=false;
      	  return true;
      	}

      	return false;
      }

      bool isAlive(void)
      {
      	return flgCommOk;
      }

      bool isCmd(void)
      {
      	return flgCmd;
      }
};

#endif
