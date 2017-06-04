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
	
	char bool; // Quand l'entité est une brique : booléenne qui indique si la bombe explosée a cassé une brique (voir liste bombesExplosees)
				// On l'utilise aussi dans verifierCollision() pour indiquer le joueur est sur quelle entité et
				// à quelle position se trouve l'entité
	
};
typedef struct Entite Entite;

typedef struct Perso Perso;
struct Perso
{
	int numero;
	SDL_Surface *persoActuel; // Le skin du perso selon la direction dans laquelle il va
	SDL_Rect position; // Position en pixels du coin en haut à gauche de la surface du personnage
	SDL_Rect hitbox; // Position en pixels du coin en haut à gauche de la hitbox, et également ses dimensions (w et h)
	Position coin[4]; // Position sur la carte de chaque coin de la hitbox (utile quand on vérifiera si un joueur touche des flammes ou un item)
	
	int touche[4]; // Tableau de booléennes qui indiquent si la touche correspondante est appuyée
	int bombePosee; // Booléenne qui indique si la touche de pose de bombe est actuellement appuyée
	int assisSurBombe; // Vaut 1 lorsque le joueur a posé une bombe et qu'il est 
					   // dessus (il peut avancer dessus, mais ne peut pas poser de bombe)
	
	int vitesse;
	int totalBombes;
	int bombesRestantes;
	int puissanceBombe;
	
	Perso *suivant;
};



#include "listes_chainees/bomberman_fct_listes_ch.h"


// FONCTIONS

int jouerPartie(SDL_Surface* ecran);
void blitterSurfaces(SDL_Surface *ecran, int carte[][NB_CASES], SDL_Surface *mur, 
					SDL_Surface *brique, SDL_Surface *bombe, SDL_Surface *flamme, SDL_Surface *itemFlamme, 
					SDL_Surface *itemBombe, SDL_Surface *itemPied, SDL_Surface *itemRoller);
void blitterPerso(SDL_Surface *ecran, ListePerso *joueurs);
void afficherCarte(int carte[][NB_CASES]);
int chercherDirection(int touche[]);
int verifierDeplacement(Perso *joueur, int direction, int carte[][NB_CASES]);
void deplacerJoueur(Perso *joueur, int direction);
void poserBombe(Perso *joueur, Liste *bombesPosees, int carte[][NB_CASES], ListePerso *joueurs);
int verifierDelai(int instant, int delai);
void exploserBombe(int carte[][NB_CASES], Maillon *bombe, Liste *bombesPosees, Liste *bombesExplosees);
void determinerPortee(int carte[][NB_CASES], Maillon *bombeExplosee, Liste *bombesPosees);
int verifierFlamme(int carte[][NB_CASES], Position posFlamme);
void afficherExplosion(int carte[][NB_CASES], Maillon *bombe, int icone);
void casserEntite(Maillon *bombe, int carte[][NB_CASES], Liste *items);
void entrainerExplosion(Liste *liste, Position posFlamme);
Entite verifierCollision(Position coin[], int carte[][NB_CASES]);
void recupererItem(Position posItem, int carte[][NB_CASES], Perso *joueur);
int* verifierTouches(Perso *idJoueur[], ListePerso *joueurs, Uint8 *touche, SDL_Surface *perso[]);


#endif
