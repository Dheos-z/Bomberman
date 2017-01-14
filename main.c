/* 14/01/17
 * J'ai trouvé sur Openclassrooms un nouveau système pour blitter les surfaces aux bonnes positions :
 * déclarer 1 surface de chaque objet est suffisant. J'ai fait en sorte que le niveau
 * soit lu dans un fichier carte.txt. Je suis en train de structurer le déroulement du programme
 * dans les différents fichiers source pour qu'ils fonctionnent ensemble.
 * Seuls les fichiers main.c et jeu.c fonctionnent ensemble pour l'instant
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
  
