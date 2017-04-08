#!/bin/bash

# Permet de compiler tout le projet Bomberman

PRG="jeu"

gcc *.c listes_chainees/bomberman_fct_listes_ch.c -o $PRG -lSDL

echo "Le programme a été compilé sous le nom de $PRG"
