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

	// Haut-Gauche, Haut-Droite, Bas-Droite et Bas-Gauche (pour les coins de la hitbox)
#define HG 0
#define HD 1
#define BD 2
#define BG 3

// AUTRES

#define VITESSE 2
#define MAX 100
#define DELAI_BOMBE 2000 // En millisecondes
#define DELAI_FLAMMES 800
#define NB_ITEM_FLAMME 3
#define NB_ITEM_BOMBE 3
#define NB_ITEM_ROLLER 1
#define NB_ITEM_PIED 0
#define NB_ITEMS NB_ITEM_BOMBE+NB_ITEM_FLAMME+NB_ITEM_PIED+NB_ITEM_ROLLER

#endif 
