#!/bin/bash

# Permet de compiler tout le projet Bomberman
# Lorsqu'on crée un nouveau fichier source, pas besoin de
# le rajouter ici !

PRG="jeu"

gcc *.c listes_chainees/*.c -o $PRG -lSDL

echo "Si la compilation n'a généré aucune erreur,"
echo "lancez le programme en tapant ./$PRG"
