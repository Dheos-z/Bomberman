/* 4/6/17
 * 
 * Fait : 
 * Nouveau système de touches
 * 
 * A faire : 
 * - Problème : flamme ne veut pas s'afficher correctement
 * - Faire l'écran titre
 * - Problème : je peux poser une bombe sur une autre
 * - Problème : quand au moins un joueur est mort, le jeu plante si j'essaye de poser un bombe avec le joueur 4
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
  
