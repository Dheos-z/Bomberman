#ifndef CONSTANTE_H
#define CONSTANTE_H


#include <SDL/SDL.h>

// STRUCTURES

struct Objet
{
  SDL_Surface *surface;
  SDL_Rect *position;
};

typedef struct Objet Objet;

// DIMENSIONS

#define CASE 45 // Taille d'une case en pixels (A NE PAS MODIFIER)
#define NB_CASES 15 // Nombre de cases de côté (A NE PAS MODIFIER)

#define LARGEUR CASE*NB_CASES // Tailles de la fenêtre en pixels
#define HAUTEUR CASE*NB_CASES

// VALEURS DES OBJETS

#define VIDE 0
#define MUR 1
#define BRIQUE 2
#define PERSO 3
#define BOMBE 4
#define MAX 100

// DIRECTIONS

#define HAUT 0
#define BAS 1
#define DROITE 2
#define GAUCHE 3




#endif 
