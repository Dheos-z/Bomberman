#!/bin/bash

# Permet de compiler tout le projet Bomberman
# Lorsqu'on crée un nouveau fichier source, pas besoin de
# le rajouter ici !

PRG="jeu"

gcc *.c listes_chainees/*.c -o $PRG -lSDL

echo "Le programme a été compilé sous le nom de $PRG"
