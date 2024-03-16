/**
 * @file i2c_cmn.h
 * @brief Interface commune entre le maître et l'esclave I²C
 * @warning S'assurer que ce fichier est identique côté esclave nano et côté maître
*/
#ifndef I2C_CMN_HEADER_INCLUDED
#define I2C_CMN_HEADER_INCLUDED

#define ADDR_SLAVE      (0xA)     ///< Adresse de l'esclave semuino (le nano)

#define REG_CTRL        (1)       ///< Id du registre de contrôle
#define REG_LEVEL       (2)       ///< Id du registre contenant le niveau d'éclairage
#define REG_MODE_RGB_A  (3)       ///< Id du registre contenant le mode d'éclairage du bandeau RGB 1 et du bandeau RGB 2
#define REG_MODE_RGB_B  (4)       ///< Id du registre contenant le mode d'éclairage du bandeau RGB 3
#define REG_EEP_WRITE   (5)       ///< Id du registre pour écrire dans l'EEPROM

#define REG_INPUTS      (10)      ///< Id du registre ou lire l'état des entrées
#define REG_TEMP        (11)      ///< Id du registre ou lire la température dans le boîtier (indisponible)
#define REG_HUM         (12)      ///< Id du registre ou lire l'humidité dans le boîtier (indisponible)
#define REG_H1          (13)      ///< Id du registre ou lire l'humidité du bas
#define REG_H2          (14)      ///< Id du registre ou lire l'humidité du haut
#define REG_H3          (15)      ///< Id du registre ou lire l'humidité du dessus
#define REG_EEP_READ    (16)      ///< Id du registre pour lire dans l'EEPROM

#define REG_ALIVE       (20)      ///< Id du alive

#define CTRL_5V         (0x01)    ///< Bit de contrôle commande de l'alimentation 5V (1 pour ON)
#define CTRL_P1         (0x02)    ///< Bit de contrôle commande des LEDs 12V P1 (1 pour ON)
#define CTRL_P2         (0x04)    ///< Bit de contrôle commande des LEDs 12V P2 (1 pour ON)
#define CTRL_P3         (0x08)    ///< Bit de contrôle commande des LEDs 12V P3 (1 pour ON)
#define CTRL_RGB1       (0x10)    ///< Bit de contrôle commande des LEDs RGB1 (1 pour ON)
#define CTRL_RGB2       (0x20)    ///< Bit de contrôle commande des LEDs RGB2 (2 pour ON)
#define CTRL_RGB3       (0x40)    ///< Bit de contrôle commande des LEDs RGB3 (3 pour ON)
#define CTRL_RESERVED   (0x80)    ///< Non utilisé

#define INP_SELECTOR    (0x01)    ///< Bit de masque de l'appui sur le selecteur
#define INP_SEL_LONG    (0x02)    ///< Bit de masque de l'appui long sur le selecteur

#define OUP_GREEN       (0x01)    ///< Bit de masque de la LED verte allumée
#define OUP_GREEN_BLINK (0x02)    ///< Bit de masque de la LED verte clignotante

#define RGB_MOD_OFF     (1)       ///< Mode RGB OFF
#define RGB_MOD_GROW    (2)       ///< Mode RGB en rouge croissance horticole
#define RGB_MOD_WHITE   (3)       ///< Mode RGB en blanc
#define RGB_MOD_MIXED   (4)       ///< Mode RGB avec un mix Blanc/Rouge horticole

#endif
