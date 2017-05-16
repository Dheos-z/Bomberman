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

#define VIDE 0 // FAIRE DES ENUM DES QUE J'AI LE TEMPS
#define MUR 1
#define BRIQUE 2
#define PERSO 3
#define BOMBE 4
#define FLAMME 5
#define ITEM_FLAMME 6
#define ITEM_BOMBE 7
#define ITEM_ROLLER 8
#define ITEM_PIED 9

#define PROBA_FLAMME 2 // 1 chance sur PROBA_FLAMME d'obtenir un item flamme
#define PROBA_BOMBE 2

// DIRECTIONS

#define HAUT 0
#define BAS 1
#define GAUCHE 2
#define DROITE 3

// AUTRES

#define VITESSE 5
#define MAX 100
#define DELAI_BOMBE 2000 // En millisecondes
#define DELAI_FLAMMES 800


#endif 
