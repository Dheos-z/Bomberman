#!/bin/bash

# Permet de compiler tout le projet Bomberman

PRG="jeu"

gcc *.c listes_chainees/fct_listes_chainees.c -o $PRG -lSDL

echo "Le programme a été compilé sous le nom de $PRG"
