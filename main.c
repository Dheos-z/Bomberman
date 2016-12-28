/* Puisqu'on travaille sur le jeu en lui-même, j'ai fait en sorte qu'à l'exécution on arrive direct sur la map de jeu (plutôt que passer par le menu à chaque fois)
 * But : ouvrir une partie depuis le main, en essayant d'utiliser les fonctions existantes,
 * pourquoi pas ouvrir le niveau depuis un fichier texte
 */


#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include "menu.h" 



int main(void)
{
    SDL_Surface *ecran = NULL;
    
    // INITIALISATIONS
    
    SDL_Init(SDL_INIT_VIDEO); // Initialise la SDL en mode vidéo
    ecran = SDL_SetVideoMode(LARGEUR, HAUTEUR, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    SDL_WM_SetCaption("Bomberman", NULL);
    
    
    
    
    
    return EXIT_SUCCESS;
}
  
