# Modules de commande des pompes en Wifi (jard_wemos)

Cette partie comporte la description de modules Wifi permettant de commander les pompes de transfert entre cuves.

Plusieurs cuves contiennent de l'eau de récupération des pluies.

Les cuves principales sont toujours en exédant d'eau, alors que les cuves proches des coins jardin sont en déficit d'eau.

Le rôle de ce système est de permettre un transfert d'eau automatique ou piloté à distance.

## Schéma d'ensemble des cuves


## Architecture du système



## Description des composants du système
### Module de pompage

Le module de pompage comporte:
- 1 alimentation 12V;
- 1 sortie pompe de 12V/5A;
- 3 entrées de type interrupteur (actif à l'état bas) pour la détection du niveau de cuve.

### Module d'affichage

Le module d'affichage comporte un écran, OLED.

Il permet de visualiser l'état des cuves et l'activité de pompage en temps réel.

### Serveur de contrôle

Le serveur comporte un broker MQTT pour centraliser les connexions des différents modules du système.

Un serveur reddis stocke tous les états, les réglages et les commandes des différentes applications.

Les applications sont les suivantes:
- Interraction avec les modules de pompage
- Publication des états sur les écrans
- Webservice pour exposer les services à des clients
- Automatisme de contrôle
- Logger

