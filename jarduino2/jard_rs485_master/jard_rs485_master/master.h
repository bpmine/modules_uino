#ifndef MASTER_HEADER_INCLUDED
#define MASTER_HEADER_INCLUDED

#include "slaves.hpp"
#include "framebuilder.h"
#include "frames.hpp"
#include "timer.h"

#define BUS_RS485_SPEED		(9600)
#define TIMEOUT_MS		    (200UL)

class Master : public IFrameReceiver
{
  private:
    unsigned long nbcycles;

    SlavesList list;
    int pos;
    
    enum {OFF,IDLE,SEND,RECV,NEXT,WAIT,END} eState;
    unsigned short commands;
    Slave *pCurSlave;
    FrameBuilder rxFrame;
    Timer tmrAnswer=Timer(2000);
    Timer tmrWait=Timer(10);

    bool flgTrace;

    bool Master::OnFrameReceive(FramePump *pump) override;
    bool Master::OnFrameReceive(FrameOya *oya) override;
    bool Master::OnFrameReceive(FramePong *pong) override;

  protected:
    virtual void sendBytes(unsigned char* buffer, int size) = 0;
    virtual int available(void) = 0;
    virtual int readByte(void) = 0;
    virtual void log(char *msg) = 0;

  public:
    Master();
    virtual ~Master();
    
    void init();    
    void start_cycle(void);
    void setEnable(bool enable);  
    bool loop(void);
    void recv(void);
    bool isRunning(void);
    unsigned long cycles(void);

    void enable_slaves(unsigned short ens);
    
    void setTrace(bool flgEnabled);
};

#endif
