#ifndef CLIENT_HEADER_INCLUDED
#define CLIENT_HEADER_INCLUDED

extern void client_init(void *pSerial,unsigned char addr,int pinTxEn);
extern void client_onSerialEvent(void *pSerial);

#endif