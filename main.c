/* 31/5/17
 * 
 * Fait : 
 * Création d'une liste chaînée de persos à la place du tableau statique
 * Modification de toutes les fonctions pour que ça marche bien
 * 
 * A faire : 
 * - Si une flamme touche un joueur le perso meurt (voir verifierCollision() )
 * - enlever l'ombre moche du sprite bomberman bas
 * 
 * PROBLEME : erreur de segmentation depuis que j'ai créé des pointeurs
 * qui contiennent les adresses de chaque joueur (idJoueur[2]), et que je modifie les
 * touches de chaque joueur grâce à ces pointeurs. Trouver le problème
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
  
