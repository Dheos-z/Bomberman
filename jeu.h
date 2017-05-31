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
	
	char bool; // Quand l'entité est une brique : booléenne qui indique si la bombe explosée a cassé une brique
	
};
typedef struct Entite Entite;

struct Perso
{
	SDL_Surface *persoActuel; // Le skin du perso selon la direction dans laquelle il va
	SDL_Rect position; // Position en pixels du coin en haut à gauche de la surface du personnage
	SDL_Rect hitbox; // Position en pixels du coin en haut à gauche de la hitbox, et également ses dimensions (w et h)
	Position coin[4]; // Position sur la carte de chaque coin de la hitbox (utile quand on vérifiera si un joueur touche des flammes ou un item)
	
	int touche[4]; // Tableau de booléennes qui indiquent si la touche correspondante est appuyée
	int bombePosee; // Booléenne qui indique si la touche de pose de bombe est actuellement appuyée
	
	int vitesse;
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
int chercherDirection(int touche[]);
int verifierCollision(Perso joueur, int direction, int carte[][NB_CASES]);
void deplacerJoueur(Perso *joueur, int direction);
void poserBombe(Perso *joueur, Liste *bombesPosees, int carte[][NB_CASES]);
int verifierDelai(int instant, int delai);
void exploserBombe(int carte[][NB_CASES], Maillon *bombe, Liste *bombesPosees, Liste *bombesExplosees);
void determinerPortee(int carte[][NB_CASES], Maillon *bombeExplosee, Liste *bombesPosees);
int verifierFlamme(int carte[][NB_CASES], Position posFlamme);
void afficherExplosion(int carte[][NB_CASES], Maillon *bombe, int icone);
void casserBrique(Maillon *bombe, int carte[][NB_CASES]);
void entrainerExplosion(Liste *liste, Position posFlamme);


#endif
