#include <stdio.h>
#include <stdlib.h>
#include "constante.h"
#include "jeu.h"
#include "initialisations.h"

void initSurfaces(SDL_Surface **mur, SDL_Surface **brique, SDL_Surface **bombe, SDL_Surface **flamme, 
					SDL_Surface **itemBombe, SDL_Surface **itemFlamme, SDL_Surface **itemPied, SDL_Surface **itemRoller, SDL_Surface **perso)
{
	*mur = SDL_LoadBMP("images/MUR.bmp");
	*brique = SDL_LoadBMP("images/BRIQUE.bmp");
	*bombe = SDL_LoadBMP("images/BOMBE.bmp");
	*flamme = SDL_LoadBMP("images/flamme.bmp");
	*itemFlamme = SDL_LoadBMP("images/item_flamme.bmp");
	*itemRoller = SDL_LoadBMP("images/item_roller.bmp");
	*itemPied = SDL_LoadBMP("images/item_pied.bmp");
	*itemBombe = SDL_LoadBMP("images/item_bombe.bmp");
	
	perso[BAS] = SDL_LoadBMP("images/BAS.bmp");
	perso[HAUT] = SDL_LoadBMP("images/HAUT.bmp");
	perso[GAUCHE] = SDL_LoadBMP("images/GAUCHE.bmp");
	perso[DROITE] = SDL_LoadBMP("images/DROITE.bmp");
	
	return;
}


void initJoueur(Perso *joueur, SDL_Surface *skinInitial, int posX, int posY)
{
	joueur->persoActuel = skinInitial; // On initialise le perso vers une direction
	joueur->position.x = posX;
	joueur->position.y = posY;
		// Positions dans la carte
	
	joueur->bombePosee = 0;
	joueur->totalBombes = 10;
	joueur->bombesRestantes = joueur[0].totalBombes;
	joueur->puissanceBombe = 3;
	
	return;
}




void initialiserTouches(Perso *joueur)
{
	int i = 0;
	
	for(i=0; i<4; i++)
	{
		joueur->touche[i] = 0;
	}
	
	return;
}




int lireFichier(int carte[][NB_CASES])
{
	FILE *fichier = NULL;
	int i = 0, j = 0;
	
	fichier = fopen("carte.txt", "r"); // Ouverture du fichier représentant la carte en mode lecture seule
	
	if(fichier == NULL)
	{
		return 1; // Erreur lors de l'ouverture du fichier		
	}
	
	for(i = 0; i < NB_CASES; i++)
	{
		for(j = 0; j < NB_CASES; j++)
		{
			carte[i][j] = fgetc(fichier) - '0'; /* Remplissage des valeurs de la carte. 
												On retire '0' pour passer du char (lu dans le fichier) au int */
		}
		fgetc(fichier); // Pour lire le \n à la fin de chaque ligne
	}
	
	fclose(fichier); // La base
	
	return 0;
}
