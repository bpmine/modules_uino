/**
 * @file manager.h
 * @brief EN TETE - Programme de gestion des OYAs
 * */
#ifndef MANAGER_HEADER_INCLUDED
#define MANAGER_HEADER_INCLUDED

/**
 * @brief Modes d'affichage
 * @{
 * */
#define MODE_AFF_IDLE     (0)     ///< Maitre allume mais RS485 et Wifi Off
#define MODE_AFF_DISPLAY  (1)     ///< Affichage des etats des OYAs (RS485 on)
#define MODE_AFF_REMOTE   (2)     ///< Contrôle à distance en cours (Affiche l'etat des OYAs)
#define MODE_AFF_AUTO     (3)     ///< Mode auto en action
/**
 * @}
 **/

extern int mode_aff;

extern void manager_init(void);
extern void manager_run(void);

#endif
