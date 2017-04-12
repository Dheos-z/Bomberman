/* 12/04/17
 * 
 * Suppression des fichiers indésirables (si le commit a fonctionné)
 * Ajout de flamme.bmp
 * 
 * A faire : 
 * - Animation de la bombe qui explose
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
  
