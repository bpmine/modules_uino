/**
 * @file master.h
 * @brief Interface - Gestion de la communication I²C avec l'esclave nano
*/
#ifndef MASTER_HEADER_INCLUDED
#define MASTER_HEADER_INCLUDED

/**
 * @struct T_OUT
 * @brief Etat du semuino à appliquer sur l'esclave nano (commandes)
*/
typedef struct
{
  unsigned char ctrl;        ///< Contenu du registre de contrôle
  unsigned char level;       ///< Niveau RGB [0..255]
  unsigned char modeRGB1;    ///< Mode du bandeau RGB 1 [0..15]
  unsigned char modeRGB2;    ///< Mode du bandeau RGB 2 [0..15]
  unsigned char modeRGB3;    ///< Mode du bandeau RGB 3 [0..15] -- Non utilise car indisponible
  unsigned char leds;        ///< Etat de la LED verte
} T_OUT;

typedef struct
{
 unsigned char h1; 
 unsigned char h2;
 unsigned char h3;
 unsigned char inputs;
} T_IN;


extern void master_init(void);
extern void master_loop(void);

extern void master_get_in_values(T_IN *pIN);
extern void master_set_out_values(T_OUT *pOUT);

extern unsigned char master_read_eep(unsigned char addr);
extern void master_write_eep(int addr,unsigned char val);

#endif
