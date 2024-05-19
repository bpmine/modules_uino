/**
 * @file wificomm.h
 * @brief EN-TETE - GESTION DE LA COMMUNICATION WIFI
 **/
#ifndef COMM_HEADER_INCLUDED
#define COMM_HEADER_INCLUDED

#define COMM_BUFFER_MAX_SIZE  (255)
#define TIMEOUT_WIFI_COMM_MS  (5000)

#include "timer.h"

class HardwareSerial;
class WifiComm
{
  private:
    HardwareSerial* pStr;
    unsigned char buffer[COMM_BUFFER_MAX_SIZE];
    int pos;
    Timer tmrComms= Timer(TIMEOUT_WIFI_COMM_MS);
    bool flgActive;
    unsigned short commands;

    void pubMasterInfo(void);
    void pubPumpInfo(void);
    void pubOyaInfo(int addr);

    void execCommands(unsigned short cmds);

  public:
    WifiComm();

    void begin(HardwareSerial* pSerial);
    void loop(void);

    bool isActive(void);
    unsigned short getCommands(void);
};

extern WifiComm Comm;

#endif
