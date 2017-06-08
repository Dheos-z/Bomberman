#ifndef FONCTIONS_JEU_H
#define FONCTIONS_JEU_H


void blitterSurfaces(SDL_Surface *ecran, int carte[][NB_CASES], SDL_Surface *mur, 
					SDL_Surface *brique, SDL_Surface *bombe, SDL_Surface *flamme, SDL_Surface *itemFlamme, 
					SDL_Surface *itemBombe, SDL_Surface *itemPied, SDL_Surface *itemRoller);
void blitterPerso(SDL_Surface *ecran, ListePerso *joueurs);
void afficherCarte(int carte[][NB_CASES]);
int chercherDirection(int touche[]);
void deplacerJoueur(Perso *joueur, int direction);
void poserBombe(Perso *joueur, Liste *bombesPosees, int carte[][NB_CASES], ListePerso *joueurs);
void exploserBombe(int carte[][NB_CASES], Maillon *bombe, Liste *bombesPosees, Liste *bombesExplosees);
void determinerPortee(int carte[][NB_CASES], Maillon *bombeExplosee, Liste *bombesPosees);
void afficherExplosion(int carte[][NB_CASES], Maillon *bombe, int icone);
void casserEntite(Maillon *bombe, int carte[][NB_CASES], Liste *items);
void entrainerExplosion(Liste *liste, Position posFlamme);
void recupererItem(Position posItem, int carte[][NB_CASES], Perso *joueur);


#endif
