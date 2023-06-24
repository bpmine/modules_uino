#ifndef MASTER_HEADER_INCLUDED
#define MASTER_HEADER_INCLUDED

#include "request.hpp"

#include <arduino.h>


class Master
{
  private:
    HardwareSerial *pStr;
    int txen;

    enum {OFF,IDLE,SEND,RECV,NEXT,END} eState;
    
    uint8_t buffer[DATA_SIZE+10];
    int pos;
    unsigned long m_tick0;

    uint8_t calc_cs(uint8_t *datas,uint8_t len);
    uint8_t hex_val(uint8_t hex);

    uint8_t decode_hex_byte(uint8_t a,uint8_t b);
    
    char tohexchar(uint8_t b);
  
    int curReq;

    bool nextRequest(void);
    Request * getCurRequest(void);
    Request * itNext(int &Pos);
    bool flgTrace;

  public:
    Master();
    
    void init();    
    void send(Request *pReq);
    void begin(HardwareSerial *pSerial,int txen);
    void start_cycle(void);
    void setEnable(bool enable);  
    bool loop(void);
    void recv(void);
    
    Request *getRequestFrom(char addr);

    void setTrace(bool flgEnabled);
};

#endif
