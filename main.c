/* 
 * SUPER BOMBERMAN
 * Développé par Zakaria YAKDI et Mustapha MASTOUR
 * 
 * Veuillez lire le README avant de jouer
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
	int choix = 0, retourMenu = 1;
    SDL_Surface *ecran = NULL;
    
    // INITIALISATIONS
    
    srand(time(NULL));
    
    SDL_Init(SDL_INIT_VIDEO); // Initialise la SDL en mode vidéo
    ecran = SDL_SetVideoMode(LARGEUR, HAUTEUR, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    SDL_WM_SetCaption("Bomberman", NULL);
    
    
    // MENU
    
    while(retourMenu)
    {
		choix = menujeu(ecran);
		
		if(choix != 5) // Si l'on ne cherche pas à qutitter le programme
		{
			retourMenu = jouerPartie(ecran, choix);
		}
		else
		{
			retourMenu = 0;
		}
	}

    
    SDL_Quit();
    
    return EXIT_SUCCESS;
}
  
