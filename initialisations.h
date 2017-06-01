#ifndef INITIALISATIONS_H
#define INITIALISATIONS_H


void initSurfaces(SDL_Surface **mur, SDL_Surface **brique, SDL_Surface **bombe, SDL_Surface **flamme, 
					SDL_Surface **itemBombe, SDL_Surface **itemFlamme, SDL_Surface **itemPied, SDL_Surface **itemRoller, SDL_Surface **perso);
void initJoueur(Perso *joueur, SDL_Surface *skinInitial, int posX, int posY, char pseudo[]);
void initialiserTouches(Perso *joueur);
int lireFichier(int carte[][NB_CASES]);

#endif
