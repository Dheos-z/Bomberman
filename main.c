/* 21/04/17
 * 
 * Fait : 
 * Rien
 * 
 * A faire : 
 * - Effets de l'explosion : si du feu touche une bombe ça la fait exploser,
 * si du feu touche une brique ça la casse, si ça touche un joueur le perso meurt, si ça touche
 * un item ça le brûle
 * 
 * On en est où : erreur de segmentation, je crois que c'est dans determinerPortee().
 * Enlever le tableau portee et incrementer direct le tableau dans bombeExplosee.
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
  
