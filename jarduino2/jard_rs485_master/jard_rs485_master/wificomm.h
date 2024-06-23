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
    Timer tmrAlive= Timer(TIMEOUT_WIFI_COMM_MS);
    Timer tmrRemoteActive= Timer(TIMEOUT_WIFI_COMM_MS);
    bool flgRemoteActive;
    bool flgAlive;
    unsigned short commands;

    void pubMasterInfo(void);
    void pubPumpInfo(void);
    void pubOyaInfo(int addr);

    void execCommands(unsigned short cmds,bool ctrl);

  public:
    WifiComm();

    void begin(HardwareSerial* pSerial);
    void setPower(bool on);
    void loop(void);

    bool isRemoteActive(void);
    bool isAlive(void);
    unsigned short getCommands(void);
};

extern WifiComm Comm;

#endif
