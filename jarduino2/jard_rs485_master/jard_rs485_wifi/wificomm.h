/**
 * @file wificomm.h
 * @brief EN-TETE - GESTION DE LA COMMUNICATION WIFI
 **/
#ifndef COMM_HEADER_INCLUDED
#define COMM_HEADER_INCLUDED

#define COMM_BUFFER_MAX_SIZE    (255)
#define TIMEOUT_WIFI_ANSWER_MS  (500)
#define CYCLE_MS                (5000)

#include "timer.h"
#include "oyas.hpp"

class HardwareSerial;
class WifiComm
{
  private:
    HardwareSerial* pStr;
    OyasInfo curInfos;
    OyasInfo latchedInfos;
    
    unsigned char buffer[COMM_BUFFER_MAX_SIZE];
    int pos;

    int state;
    int curAddr;

    unsigned short commands_to_send;
    unsigned short config_slaves;
    
    Timer tmrAnswer= Timer(TIMEOUT_WIFI_ANSWER_MS);
    Timer tmrCycle= Timer(CYCLE_MS,false);
    bool flgActive;
    bool flgCmdActive;

    void cycle_management(void);
    void recv_management(void);
    
  public:
    WifiComm();

    void begin(HardwareSerial* pSerial);
    void loop(void);

    void setCommands(unsigned short cmds);

    OyasInfo &getOyasInfo(void);
};

extern WifiComm Comm;

#endif
