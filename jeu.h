#ifndef JEU_H
#define JEU_H

// STRUCTURES

struct Perso
{
	SDL_Surface *persoActuel;
	SDL_Rect position;
	int touche[4];
	int bombePosee; // Booléenne qui indique si la touche de pose de bombe
					// est actuellement appuyée
	int totalBombes;
	int bombesRestantes;
	int instantBombe[MAX]; // Répertorie les instants aux quels chaque bombe a été posée
};
typedef struct Perso Perso;

struct Position
{
	int x;
	int y;
};
typedef struct Position Position;

// FONCTIONS

int jouerPartie(SDL_Surface* ecran);
int lireFichier(int carte[][NB_CASES]);
void blitterSurfaces(SDL_Surface *ecran, int carte[][NB_CASES], SDL_Surface *mur, SDL_Surface *brique, SDL_Surface *bombe);
void blitterPerso(SDL_Surface *ecran, Perso joueur[], int nbJoueurs);
void afficherCarte(int carte[][NB_CASES]);
void deplacerJoueur(Perso *joueur);
void initialiserTouches(Perso *joueur);
void poserBombe(Perso *joueur, int carte[][NB_CASES]);

#endif
