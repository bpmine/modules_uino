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

### Schéma et conception

[Schéma et PCB de la carte](board\semuino)

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

La version plus élaborée de la carte semuino comporte en plus un ESP01 et une extension pour connecter un petit écran OLED (type SSD1306 128x64).

![GitHub Logo](images/semuino_esp01_wifi.png)

Dans cette version, l'arduino Nano devient un simple esclave I²C parmi les autres (RTC et écran). C'est le module ESP01 qui gère tout.
Ce dernier peut aussi fonctionner de façon autonome comme pour la configuration précédente.
Mais il peut communiquer en Wifi et disposer de plus de mémoire pour réaliser plus de choses.

L'ensemble logique de la carte est alimenté en 3.3V (Nano, RTC, ESP01, DHT22, ...). Pour le Nano, il est nécessaire d'ajouter une diode pour permettre l'utilisation simultanée de l'alimentation principale avec celle de l'USB.
Le 3.3V de la carte est appliqué à l'entrée d'alimentation 5V du nano. En effet, la broche 3.3V du Nano est une sortie reliée au régulateur intégré de la puce USB. Elle ne sera pas utilisée.

### Composants

En cours de construction...

### Les programmes

Il y'a plusieurs types de programmes différents pour l'arduino Nano:
Nom | Description | Lien
--- | --- | ---
semuino_test|Pour tester la carte semuino | 
semuino|Le programme standalone standard pour le Nano |
semuino_nano_slave|Le programme esclave I²C pour le nano |
semuino_nano_master_test|Un programme de test pour remplacer un maître I²C avec un Nano |

Le Nano est quasiment plein niveau RAM. Il n'est pas possible de gérer les trois RGB en même temps. Le RB3 a pour l'instant été retiré.

Pour l'ESP01, il n'y a qu'un seul programme:
Nom | Description | Lien
--- | --- | ---
semuino_wifi|Programme SEMUINO complet pour ESP01 | 

## Protocole d'échange I²C (Esclave Nano)

L'I²C permet à l'ESP01 d'accéder à l'écran, au RTC et au Nano. Cette partie décrit la gestion de l'esclave par le programme du Nano.

Adresse de l'esclave: 0xA

Pour écrire une valeur, on envoie le numéro de registre (1 octet) puis la donnée à écrire.

Registre | Nom | R/W  | Taille | Description
--- | --- | --- | ---
1 | Commandes | Ecriture | 1 octet | Contient les commandes pour agir sur les LEDs
2 | Level | Ecriture | 1 octet | Défini le niveau d'éclairage des LEDs RGB (entre 1 et 255)
3 | Modes RGB A | Ecriture | 1 octet | Contient les modes des bandeaux RGB 1 (4 bits de poids faible) et RGB 2 (4 bits de poids fort)
4 | Modes RGB B | Ecriture | 1 octet | Les 4 bits de poids faible correspondent au mode du bandeau RGB 3
5 | WRITE EEPROM | Ecriture | 2 octets | Le premier octet contient l'adresse EEPROM où écrire et le second contient la donnée à écrire

Pour lire une valeur, on envoie le numéro de registre (1 octet) puis on demande la lecture des données.

Registre | Nom | R/W  | Taille | Description
--- | --- | --- | ---
10 | Inputs | Lecture | 1 octet | Retourne l'état des entrées (bit 1: bouton de select)
11 | TEMP | Lecture | 1 octet | Retourne la température ambiante (en °C)
12 | HUM | Lecture | 1 octet | Retourne le taux d'humidité ambiante (en %)
13 | H1 | Lecture | 1 octet | Retourne la valeur du capteur d'humidité du sol n° 1 (0..255)
14 | H2 | Lecture | 1 octet | Retourne la valeur du capteur d'humidité du sol n° 2 (0..255)
15 | H3 | Lecture | 1 octet | Retourne la valeur du capteur d'humidité du sol n° 3 (0..255)
16 | READ EEPROM | Lecture | 1 octet | Il faut d'abord écrire 1 octet d'adresse, avant de lire l'octet contenant la valeur.

## Spécification du Webservice ESP01

### Information

La requète suivante permet de récupérer les informations d'identification du service.

GET /semuino/info

```

{
  "type":"semuino_info",
  "version":"V1.0",
  "description":"API de commande du semuino / Gestion du meuble à semi.",
  "github":"https://github.com/bpmine/modules_uino/tree/master/semuino"
}

```

### Choix du mode manuel/auto

GET /semuino/mode
POST /semuino/mode

```

{
  "type":"semuino_mode",
  "mode":"manual"
}

```

Les deux modes possibles sont `manual` et `auto`.
En mode manuel, l'utilisateur pilote directement le semuino en postant des commandes par `/semuino/cmds`.
En mode automatique, il modifie la configuration si nécessaire avec `/semuino/set`.

### Commandes des LEDs

Seulement en mode manuel. Pour envoyer des commandes.

POST /semuino/cmds

```

{
  "type":"semuino_cmds",

  "cmd5v":true,
  "rgb1":true,
  "rgb2":true,
  "rgb3":true,
  "modergb1":2,
  "modergb2":1,
  "modergb3":1,

  "cmdp1":true,
  "cmdp2":false,
  "cmdp3":false
}

```

Champ | Plage | Description
--- | --- | ---
cmd5v | false..true | Active ou non l'alimentation 5V des LEDs RGB
rgb1 | false..true | Active le bandeau RGB1
rgb2 | false..true | Active le bandeau RGB2
rgb3 | false..true | Active le bandeau RGB3
modergb1 | 0..15 | Défini le mode de fonctionnement du bandeau RGB 1
modergb2 | 0..15 | Défini le mode de fonctionnement du bandeau RGB 2
modergb3 | 0..15 | Défini le mode de fonctionnement du bandeau RGB 3
cmdp1 | false..true | Active le bandeau de LED 12V n°1
cmdp2 | false..true | Active le bandeau de LED 12V n°2
cmdp3 | false..true | Active le bandeau de LED 12V n°3


### Lecture des valeurs des capteurs

*GET /semuino/sensors*

```

{  
  "type":"semuino_sensors",

  "date":"12/12/2023",
  "time":"10:45:01",
  "temp":25,
  "hum":70,
  "hum1":100,
  "hum2":111,
  "hum3":45
}

```

Champ | Plage | Description
--- | --- | ---
date | dd/mm/yyyy | Date du module RTC
time | HH:MM:SS | Heure du module RTC
temp | -40..100 | Température ambiante (en °C)
hum | 0..100 | Taux d'humidité ambiant (en %)
hum1 | 0..255 | Valeur lue sur le capteur d'humidité 1
hum2 | 0..255 | Valeur lue sur le capteur d'humidité 2
hum3 | 0..255 | Valeur lue sur le capteur d'humidité 3

### Configuration du mode automatique

*GET /semuino/set*
*POST /semuino/set*

```

{
  "type":"semuino_set",

  "date":"14/11/2023",
  "time":"12:12:02",

  "sunrise":"07:00",
  "sunset":"22:00",

  "modergb1":2,
  "modergb2":1,
  "modergb3":1
}

```

# Bilan et configuration finale

En cours de construction...

# Version précédente du meuble à semis V0 (Hiver 2021)

Une première version du meuble a été mise en ligne en 2021.

Pour accéder à l'ancienne version de cette page, cliquer [Ici](./readme_v0.md).
Voir le tag __ semuino_prototype_2021 __ du git pour avoir l'intégralité des sources de cette version.


