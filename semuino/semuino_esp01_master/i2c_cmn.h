/**
 * @file i2c_cmn.h
 * @brief Interface commune entre le maître et l'esclave I²C
*/
#ifndef I2C_CMN_HEADER_INCLUDED
#define I2C_CMN_HEADER_INCLUDED

#define ADDR_SLAVE      (0xA)     ///< Adresse de l'esclave semuino (le nano)

#define REG_CTRL        (1)       ///< Id du registre de contrôle
#define REG_LEVEL       (2)       ///< Id du registre contenant le niveau d'éclairage
#define REG_MODE_RGB_A  (3)
#define REG_MODE_RGB_B  (4)
#define REG_EEP_WRITE   (5)

#define REG_INPUTS      (10)
#define REG_TEMP        (11)
#define REG_HUM         (12)
#define REG_H1          (13)
#define REG_H2          (14)
#define REG_H3          (15)
#define REG_EEP_READ    (16)

#define REG_ALIVE       (20)

#define CTRL_5V         (0x01)
#define CTRL_P1         (0x02)
#define CTRL_P2         (0x04)
#define CTRL_P3         (0x08)
#define CTRL_RGB1       (0x10)
#define CTRL_RGB2       (0x20)
#define CTRL_RGB3       (0x40)
#define CTRL_RESERVED   (0x80)

#define INP_SELECTOR    (0x01)
#define OUP_GREEN       (0x01)

#define RGB_MOD_OFF     (1)
#define RGB_MOD_GROW    (2)
#define RGB_MOD_WHITE   (3)
#define RGB_MOD_MIXED   (4)

#endif
