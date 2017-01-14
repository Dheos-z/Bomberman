#ifndef JEU_H
#define JEU_H

void remplissage(int map[NB_CASES][NB_CASES]);
void affichage(int map[NB_CASES][NB_CASES]);
void initPerso(int map[NB_CASES][NB_CASES]);

int jouerPartie(SDL_Surface* ecran);
int lireFichier(int carte[][NB_CASES]);
void blitterSurfaces(SDL_Surface *ecran, int carte[][NB_CASES], SDL_Surface *mur, SDL_Surface *brique, SDL_Surface *bombe, SDL_Surface *persoActuel);

#endif
