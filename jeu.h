#ifndef JEU_H
#define JEU_H

// STRUCTURES

struct Perso
{
	SDL_Surface *persoActuel;
	SDL_Rect position;
	int touche[4];
};

typedef struct Perso Perso;

// FONCTIONS

void remplissage(int map[NB_CASES][NB_CASES]);
void affichage(int map[NB_CASES][NB_CASES]);
void initPerso(int map[NB_CASES][NB_CASES]);

int jouerPartie(SDL_Surface* ecran);
int lireFichier(int carte[][NB_CASES]);
void blitterSurfaces(SDL_Surface *ecran, int carte[][NB_CASES], SDL_Surface *mur, SDL_Surface *brique, SDL_Surface *bombe);
void blitterPerso(SDL_Surface *ecran, Perso joueur[], int nbJoueurs);
void afficherCarte(int carte[][NB_CASES]);
void deplacerJoueur(Perso *joueur);
void initialiserTouches(Perso *joueur);

#endif
