/* 2/6/17
 * 
 * Fait : 
 * Collision des bombes
 * 
 * A faire : 
 * - Problème : flamme ne veut pas s'afficher correctement
 * - Faire l'écran titre
 * - Faire la mort du personnage : problème : quand j'appelle supprimerPerso() le jeu freeze
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
  
