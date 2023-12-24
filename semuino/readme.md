# SEMUINO - ECLAIRAGE ARMOIRE A SEMIS

Toutes les plantes ont besoin de lumière.

Les semis ont aussi besoin de lumière.

Pour une croissance optimale, il faut éclairer les semis pendant une certaine durée journalière et avec un spectre de lumière donné (couleurs).

C'est ce que réalise le meuble à semis sur la photo suivante.

![GitHub Logo](images/meuble.png)

## Description de l'éclairage souhaité

Pendant la phase de germinaison et de croissance, il est conseillé d'éclairer avec une teinte composée de 20% de bleu et 80% de rouge.
La lumière doit être répartie de façon homogène pour éviter que les pousses prenent une direction particulière ou/et ne mollissent.

Nous avons choisi une durée d'éclairage de 16 heures par jour.

Pour se rapprocher du cycle naturel, il y'a une montée de la luminosité pendant les 2 premières heures et une descente les 2 dernières heures de la journée.

Pendant la deuxième phase de croissance, la lumière est blanche. Mais les avis divergent. Le meuble permet donc de choisir plusieurs modes d'éclairage.

## Description du meuble

### Composition du meuble

Il est issu de la récupération d'un vieux meuble cassé et dont nous n'avions plus l'usage. 

Le tiroir du bas est utilisé pour mettre des semis.

Le tiroir du milieu est retourné et servira de plafond au premier étage (en bas).

Au dessus du tiroir du milieu pourront être posé des petits plants de plus grande taille. Un tiroir peut également être inséré au dessus pour permettre de poser d'autres semis.

Des bandeaux de LEDs sont collés au dessus et dans le tiroir du milieu. Ils éclairent de façon homogène les semis des deux étages intérieurs.

Il est prévu d'ajouter un étage au dessus du meuble toujours sur le même principe.

### Mode d'emploi

En cours de construction...
 
## Détail technique

### Schéma

[Schéma et PCB de la carte](.\board\semuino)

Le carte semuino est architecturée autour d'un arduino nano dans sa configuration réduite "standalone".

![GitHub Logo](images/semuino_standalone.png)

Elle comporte les éléments suivants:

- Module Nano;
- module RTC;
- module DHT22 pour mesurer l'humidité et la température ambiante;
- trois Sorties de puissance 12V pour alimenter des LEDs horticoles en mode ON/OFF;
- une alimentation 5V pilotable pour alimenter les bandeaux de LEDs LEDs RGB;
- trois sorties pour bandeaux de LEDs RGB;
- trois entrées pour mesure d'humidité du sol;
- un bouton de sélection de mode;
- une entrée/sortie disponible.

Dans cette configuration, le semuino fonctionne de manière autonome. Il se base sur la date et l'heure pour appliquer l'éclairage souhaité.
Il est également possible de prendre en compte la température et l'humidité (en fonction du programme présent dans le nano).
L'utilisateur peut changer le mode à l'aide d'un bouton de sélection.

La version plus élaborée de la carte semuino comporte en plus un ESP01 et une extension pour connecter un petit écran OLED.

![GitHub Logo](images/semuino_esp01_wifi.png)

Dans cette version, l'arduino Nano devient un simple esclave I²C parmi les autres (RTC et LCD). C'est le module ESP01 qui gère tout.
Il peut aussi fonctionner en "standalone" avec le même fonctionnement que pour la première configuration.
Mais il peut communiquer en Wifi et disposer de plus de mémoire pour réaliser plus de choses.

L'ensemble logique de la carte est alimenté en 3.3V (Nano, RTC, ESP01, DHT22, ...). Pour le Nano, il est nécessaire d'ajouter une diode pour permettre l'utilisation simultanée de l'alimentation principale avec celle de l'USB.
Le 3.3V de la carte est appliqué à l'entrée d'alimentation 5V du nano. En effet, la broche 3.3V du Nano est une sortie reliée au régulateur intégré de la puce USB. Elle ne sera pas utilisée.

### Composants

En cours de construction...

### Le programme

En cours de construction...

# Bilan et configuration finale

En cours de construction...

# Version précédente du meuble à semis V0 (Hiver 2021)

Une première version du meuble a été mise en ligne en 2021.

Pour accéder à l'ancienne version de cette page, cliquer [Ici](./readme_v0.md).
Voir le tag __ semuino_prototype_2021 __ du git pour avoir l'intégralité des sources de cette version.


