/* 21/04/17
 * 
 * Fait : 
 * La liste des bombes est maintenant indépendante des joueurs, dans bombesPosees.
 * Les bombes peuvent exploser les briques
 * 
 * A faire : 
 * - Effets de l'explosion : si du feu touche une bombe ça la fait exploser,
 * si du feu touche une brique ça la casse, si ça touche un joueur le perso meurt, si ça touche
 * un item ça le brûle
 * 
 * On en est où : finir de rédiger avancement.txt et description_fonctions.txt
 * Faire en sorte que lorsqu'une bombe touche une autre bombe, ça la fait exploser aussi.
 * 
 * Zak
 */


#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include "menu.h" 
#include "constante.h"
#include "jeu.h"


// ECRAN TITRE

int main(void)
{
    SDL_Surface *ecran = NULL;
    SDL_Event event;
    
    // INITIALISATIONS
    
    SDL_Init(SDL_INIT_VIDEO); // Initialise la SDL en mode vidéo
    ecran = SDL_SetVideoMode(LARGEUR, HAUTEUR, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    SDL_WM_SetCaption("Bomberman", NULL);
    
    
    jouerPartie(ecran);
    
    SDL_Quit();
    
    return EXIT_SUCCESS;
}
  
