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
  unsigned char modeRGB1;    ///< Mode du bandeau RGB 1 (bas) [0..15]
  unsigned char modeRGB2;    ///< Mode du bandeau RGB 2 (haut) [0..15]
  unsigned char modeRGB3;    ///< Mode du bandeau RGB 3 (dessus) [0..15] -- Non utilise car indisponible
  unsigned char voyants;     ///< Etat de la LED verte
} T_OUT;

/**
 * @struct T_IN
 * @brief Valeurs des entrées lues sur l'esclave nano
*/
typedef struct
{
 unsigned char h1;          ///< Humidite du sol (bas)
 unsigned char h2;          ///< Humidité du sol (haut)
 unsigned char h3;          ///< Humidité du sol (dessus)
 unsigned char inputs;      ///< Etat des entrees
} T_IN;


extern void master_init(void);
extern void master_loop(void);

extern void master_get_in_values(T_IN *pIN);
extern void master_set_out_values(T_OUT *pOUT);

extern unsigned char master_read_eep(unsigned char addr);
extern void master_write_eep(int addr,unsigned char val);

#endif
