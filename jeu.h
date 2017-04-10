#ifndef JEU_H
#define JEU_H

// STRUCTURES

struct Position
{
	int x;
	int y;
};
typedef struct Position Position;

#include "listes_chainees/bomberman_fct_listes_ch.h"

struct Perso
{
	SDL_Surface *persoActuel; // Le skin du perso selon la direction dans laquelle il va
	SDL_Rect position;
	int touche[4]; // Tableau de booléennes qui indiquent si la touche correspondante est appuyée
	int bombePosee; // Booléenne qui indique si la touche de pose de bombe
					// est actuellement appuyée
	int totalBombes;
	int bombesRestantes;
	Liste *listeBombes; // Répertorie les instants auxquels chaque bombe a été posée
};
typedef struct Perso Perso;


// FONCTIONS

int jouerPartie(SDL_Surface* ecran);
void blitterSurfaces(SDL_Surface *ecran, int carte[][NB_CASES], SDL_Surface *mur, SDL_Surface *brique, SDL_Surface *bombe);
void blitterPerso(SDL_Surface *ecran, Perso joueur[], int nbJoueurs);
void afficherCarte(int carte[][NB_CASES]);
void deplacerJoueur(Perso *joueur);
void poserBombe(Perso *joueur, int carte[][NB_CASES]);
void exploserBombe(int carte[][NB_CASES], Perso *joueur);

#endif
