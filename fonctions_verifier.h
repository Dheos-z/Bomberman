#ifndef FONCTIONS_VERIFIER_H
#define FONCTIONS_VERIFIER_H


int verifierDeplacement(Perso *joueur, int direction, int carte[][NB_CASES]);
int verifierDelai(int instant, int delai);
int verifierFlamme(int carte[][NB_CASES], Position posFlamme);
int* verifierTouches(Perso *idJoueur[], ListePerso *joueurs, Uint8 *touche, SDL_Surface *perso[]);
Entite verifierCollision(Position coin[], int carte[][NB_CASES]);


#endif
