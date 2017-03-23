#!/bin/bash

# Permet de compiler tout le projet Bomberman

PRG="jeu"

gcc main.c jeu.c -o $PRG -lSDL

echo "Le programme a été compilé sous le nom de $PRG"
