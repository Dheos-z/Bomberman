/* 5/5/17
 * 
 * Fait : 
 * Que de la merde
 * 
 * A faire : 
 * - Effets de l'explosion : si du feu touche une bombe ça la fait exploser,
 * si du feu touche une brique ça la casse, si ça touche un joueur le perso meurt, si ça touche
 * un item ça le brûle
 * 
 * Ce que j'étais en train de faire :
 * Dans determinerPortee(), lorsque qu'une flamme atteint une autre bombe, retrouver cette bombe
 * dans bombesPosees, changer son instant (SDL_GetTicks() + DELAI_BOMBE) pour qu'elle explose direct
 * quand on vérifie son délai, et modifier sa position dans bombesPosees : la mettre au rang 1 comme ça
 * la bombe qui a déjà explosée (et provoqué la nouvelle explosion)
 * partira du rang 0 de bombesPosees et c'est celle qui doit exploser (celle à qui on a modifié l'instant)
 * qui prendra la position 0 et elle sera vérifiée pour exploser.
 * Pour ça, faire une nouvelle fonction de liste chaînée qui déplace un élément
 * 
 * 
 * 
 * Zak
 */


#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include "menu.h" 
#include "constante.h"
#include "jeu.h"
#include <time.h>


// ECRAN TITRE

int main(void)
{
    SDL_Surface *ecran = NULL;
    SDL_Event event;
    
    // INITIALISATIONS
    
    srand(time(NULL));
    
    SDL_Init(SDL_INIT_VIDEO); // Initialise la SDL en mode vidéo
    ecran = SDL_SetVideoMode(LARGEUR, HAUTEUR, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    SDL_WM_SetCaption("Bomberman", NULL);
    
    
    jouerPartie(ecran);
    
    SDL_Quit();
    
    return EXIT_SUCCESS;
}
  
