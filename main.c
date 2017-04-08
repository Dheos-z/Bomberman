/* 08/04/17
 * A faire : initialisations.c et initialisations.h pour les initialisations en tout genre
 * 
 * Ajout de verifierBombe().
 * A faire : exploserBombe()
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
  
