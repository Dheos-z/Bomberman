#ifndef CONSTANTE_H
#define CONSTANTE_H


#include<SDL/SDL.h>

// STRUCTURE 

struct Objet
{
  SDL_Surface *surface;
  SDL_Rect *position;
};

typedef struct Objet Objet;

// DIMENSIONS DE LA FENETRE

#define LARGEUR 675
#DEFINE HAUTEUR 675

// CONSTANTES VALEURES DES CASES

#define VIDE 0
#define BRIQUE 2
#define MUR 1
#define MAX 100
#define PERSO 3
#define BOMBE 4

// CONSTANTES PIXEL

#define CASE 45
#define T_TAB 15

// CONSTANTES DIRECTION

#define HAUT 1
#define BAS 2
#define DROITE 3
#define GAUCHE 4




#endif 
