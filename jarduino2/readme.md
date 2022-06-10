# JARDUINO 2

## MODBUS

Le jarduino implémente un serveur/esclave MODBUS.

Seules les functions suivantes sont implémentées:

- 01 READ COIL STATUS
- 02 READ INPUT STATUS
- 03 READ HOLDING REGISTERS
- 04 READ INPUT REGISTERS
- 15 WRITE MULTIPLE COILS
- 16 WRITE MULTIPLE REGISTERS

### Table des COILs (Lecture/Ecriture)

Adresse | Description
--- | ---
10 | Forçage POMPE 1
11 | Activation POMPE 1
12 | Mode automatique POMPE 1
13 | Mode Distant POMPE 1
--- | ---
14 | Forçage POMPE 2
15 | Activation POMPE 2
16 | Mode automatique POMPE 2
17 | Mode Distant POMPE 2

### Table des INPUT REGISTERs (Lecture)

Adresse | Description
--- | ---
0 | Niveau de batterie en dxV
1 | Niveau du panneau solaire en dxV
2 | Temperature en °C
3 | Humidité en %
