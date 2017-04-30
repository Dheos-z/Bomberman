/* 30/04/17
 * 
 * Fait : 
 * Que de la merde
 * 
 * A faire : 
 * - Effets de l'explosion : si du feu touche une bombe ça la fait exploser,
 * si du feu touche une brique ça la casse, si ça touche un joueur le perso meurt, si ça touche
 * un item ça le brûle
 * 
 * On en est où : finir de rédiger avancement.txt et description_fonctions.txt
 * Faire en sorte que lorsqu'une bombe touche une autre bombe, ça la fait exploser aussi.
 * 
 * Génération d'items quand on casse une brique : y a un truc qui
 * marche pas dans casserBrique(), changer de technique. Idée : 
 * dans la structure brique (Entité je crois) mettre l'item que contient la brique (c'est stylé ça)
 * 
 * J'arrive pas à charger les images item_bombe, item_flamme, item_roller et item_pied ça casse
 * les couilles
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
  
