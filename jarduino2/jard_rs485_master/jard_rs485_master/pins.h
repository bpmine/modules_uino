/**
 * @file pins.h
 * @brief Definition des broches de la carte Master RS485
*/
#ifndef PINS_HEADER_INCLUDED
#define PINS_HEADER_INCLUDED

#define PIN_TX_EN         (6)     ///< Activation de la ligne RS485 (Pour emettre)
#define PIN_TEST_BTN      (2)     ///< Bouton de test (pull up interne)
#define PIN_PWR_ON        (3)     ///< Commande puissance sur réseau RS485 (esclaves)
#define PIN_DATA_LEDS     (4)     ///< Commande Data des LEDs
#define PIN_PWR_LEDS      (5)     ///< Alimentation des LEDs (via npn)
#define PIN_SD_CS         (10)

#endif
