# ParkAssist

Autore: Angelo Sorrentino

Descrizione: il progetto consiste in un sistema che favorisce la precisione durante i parcheggi in luogo chiuso.
Un sensore ad ultra suoni aiuta a pilotare un servo motore, sopra al quale è montato un diodo laser a linea. Quest'ultimo proietta la distanza tra il veicolo e la parete, rispetto ad una linea presa come riferimento (es: lo spigolo tra parete e soffitto).

Componenti HW:
- sensore ad ultra suoni (HC-SR04)
- servo motore (SG90)
- diodo laser a linea
- Arduino UNO R3

Link a repo: https://github.com/angsorre/ParkAssist

Licenza scelta: GNU General Public License v3.0

Data *indicativa* di presentazione: fine giugno 2025

## Schema
![schema](schema.png)


### Alimentazione
![wiring](wiring.png)
Salvato nel file [wiring](wiring.txt), importabile in [circuitjs](https://www.falstad.com/circuit/circuitjs.html).


### Installazione
Il laser va montato sul lato destro all'altezza del riferimento preso in considerazione. La distanza a cui si consiglia di montarlo è tra i 30-100cm, in maniera tale da ridurre la lunghezza della linea proiettata e da migliorare la precisione. Tale valore di distanza deve essere inserite nel parametro `mount_distance` prima di flashare il codice.

