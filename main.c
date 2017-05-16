/* 16/5/17
 * 
 * Fait : 
 * Définition de la hitbox pour chaque joueur
 * Maintenant la pose de bombe et le déplacement du joueur se font grâce à la hitbox
 * Fait l'image bomberman bas avec les bonnes dimensions
 * 
 * A faire : 
 * - Si une flamme touche un joueur le perso meurt, si ça touche
 * un item ça le brûle
 * J'ai déjà défini comment faire pour la mort du perso dans avancement.txt
 * - Refaire les images (persos, items, flammes etc) avec les bonnes 
 * dimensions et la transparence (pour la transparence, leur donner un fond jaune)
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
  
