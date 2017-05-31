/* 31/5/17
 * 
 * Fait : 
 * Sprites du personnage bas, gauche et droite.
 * Collisions (juste les murs et les briques)
 * 
 * A faire : 
 * - Si une flamme touche un joueur le perso meurt, si ça touche
 * un item ça le brûle
 * Sprite bomberman haut + enlever l'ombre moche du sprite bomberman bas
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
  
