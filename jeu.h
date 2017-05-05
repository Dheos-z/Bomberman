#ifndef JEU_H
#define JEU_H

// STRUCTURES

struct Position
{
	int x;
	int y;
};
typedef struct Position Position;

struct Entite
{
	Position position;
	char bool; // Booléenne qui indique si la bombe explosée a cassé une brique
};
typedef struct Entite Entite;

struct Perso
{
	SDL_Surface *persoActuel; // Le skin du perso selon la direction dans laquelle il va
	SDL_Rect position;
	int touche[4]; // Tableau de booléennes qui indiquent si la touche correspondante est appuyée
	int bombePosee; // Booléenne qui indique si la touche de pose de bombe
					// est actuellement appuyée
	int totalBombes;
	int bombesRestantes;
	int puissanceBombe;
};
typedef struct Perso Perso;

#include "listes_chainees/bomberman_fct_listes_ch.h"


// FONCTIONS

int jouerPartie(SDL_Surface* ecran);
void blitterSurfaces(SDL_Surface *ecran, int carte[][NB_CASES], SDL_Surface *mur, 
					SDL_Surface *brique, SDL_Surface *bombe, SDL_Surface *flamme, SDL_Surface *itemFlamme, 
					SDL_Surface *itemBombe, SDL_Surface *itemPied, SDL_Surface *itemRoller);
void blitterPerso(SDL_Surface *ecran, Perso joueur[], int nbJoueurs);
void afficherCarte(int carte[][NB_CASES]);
void deplacerJoueur(Perso *joueur, int carte[][NB_CASES]);
void poserBombe(Perso *joueur, Liste *bombesPosees, int carte[][NB_CASES]);
int verifierDelai(int instant, int delai);
void exploserBombe(int carte[][NB_CASES], Maillon *bombe, Liste *bombesPosees, Liste *bombesExplosees);
void determinerPortee(int carte[][NB_CASES], Maillon *bombeExplosee, Liste *bombesPosees);
void afficherExplosion(int carte[][NB_CASES], Maillon *bombe, int icone);
void casserBrique(Maillon *bombe, int carte[][NB_CASES]);
Maillon *chercherBombe(Position posBombe, Liste *bombesPosees);

#endif
