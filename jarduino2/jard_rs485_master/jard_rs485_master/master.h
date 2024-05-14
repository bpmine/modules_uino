/**
 * @file master.h
 * @brief EN-TETE - Gestion du maître des OYAs (RS485)
 * */
#ifndef MASTER_HEADER_INCLUDED
#define MASTER_HEADER_INCLUDED

#include "slaves.hpp"
#include "framebuilder.h"
#include "frames.hpp"
#include "timer.h"

#define BUS_RS485_SPEED		(9600)
#define TIMEOUT_MS		    (200UL)

/**
 * @class Master
 * @brief Classe de gestion du maître des OYAs
 * */
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
    Timer tmrAnswer=Timer(TIMEOUT_MS);
    Timer tmrWait=Timer(10);

    /**
     * @brief Rempli les champs communs de l'objet esclave à partir d'une trame
     * */
    void fillSlaveObjFromSlaveFrame(Slave *pSlave,FrameSlave *pFrmSlave);

    bool OnFrameReceive(FramePump *pump);
    bool OnFrameReceive(FrameOya *oya);
    bool OnFrameReceive(FramePong *pong);

  protected:
    bool flgTrace;

    /**
     * @brief Fonction virtuelle pour commander l'alimentation des esclaves
     * @param[in] on true si activee
     * */
    virtual void setPower(bool on) = 0;

    /**
     * @brief Fonction virtuelle pour l'envoi des octets
     * @param[in] buffer Buffer
     * @param[in] size Nombre d'octets à envoyer
     * */
    virtual void sendBytes(unsigned char* buffer, int size) = 0;

    /**
     * @brief Retourne le nombre d'octets disponibles
     * @return Nombre d'octets disponibles
     * */
    virtual int available(void) = 0;

    /**
     * @brief Lecture d'un octet
     * @return Octet lu (-1 en cas d'erreur)
     * */
    virtual int readByte(void) = 0;

    /**
     * @brief Log d'un message
     * @param[in] msg Message
     * */
    virtual void log(const char *msg) = 0;

    /**
     * @brief Log d'un couple clef/valeur
     * @param[in] key Clef
     * @param[in] val Valeur
     * */
    virtual void log(const char *key,int val) = 0;

  public:
    Master();
    virtual ~Master();
    
    /**
     * @brief Initialisation du maître
     * */
    void init(void);

    /**
     * @brief Démarre un nouveau cycle d'échange
     * */
    void start_cycle(void);

    /**
     * @brief Active/Désactive le maître des OYAs
     * @param[in] enable true pour activer sinon false
     * */
    void setEnable(bool enable);

    /**
     * @brief Fonction a appeller aussi souvent que possible
     * @return true Si un cycle vient de se terminer
     * */
    bool loop(void);

    /**
     * @brief Fonction a appeller lorsque des octets sont disponibles en réception RS485
     * */
    void recv(void);

    /**
     * @brief Indique si le maître est en fonctionnement
     * @return true si le maître est en cours de fonctionnement
     * */
    bool isRunning(void);

    /**
     * @brief Retourne le nombre de cycles (depuis le dernier RAZ ou le BOOT)
     * @return Nombre de cycles
     * */
    unsigned long cycles(void);

    /**
     * @brief Active les esclaves souhaités (Bit0 = Esclave @1, Bit1 = Esclave @2, ... Bit14 = Esclave @14)
     * @param[in] ens Bits d'activation des esclaves
     * */
    void config_slaves(unsigned short ens);

    /**
     * @brief Commande les esclaves souhaités (Bit0 = Esclave @1, Bit1 = Esclave @2, ... Bit14 = Esclave @14)
     * @param[in] cmds Bits de commande des esclaves
     * */
    void set_commands(unsigned short cmds);
    
    /**
     * @brief Set ON/OFF the pump
     * @param[in] on true for ON
     * */
    void set_pump(bool on);

    /**
     * @brief Set ON/OFF the given oya
     * @param[in] addr Address of the oya [2-14]
     * @param[in] on true for ON
     * */
    void set_oya(char addr,bool on);

    /**
     * @brief Active ou désactive les traces
     * @param[in] flgEnabled true pour activer les traces
     * */
    void setTrace(bool flgEnabled);

    /**
     * @brief Retourne la liste des esclaves
     * @return Voir SlavesList
     * */
    SlavesList &getSlavesList();
};

#endif
