#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constante.h"
#include "jeu.h"
#include "initialisations.h"

void initSurfaces(SDL_Surface **mur, SDL_Surface **brique, SDL_Surface **bombe, SDL_Surface **flamme, 
					SDL_Surface **itemBombe, SDL_Surface **itemFlamme, SDL_Surface **itemPied, SDL_Surface **itemRoller, SDL_Surface **perso)
{
	int i=0;
	
	*mur = SDL_LoadBMP("images/MUR.bmp");
	*brique = SDL_LoadBMP("images/BRIQUE.bmp");
	*bombe = SDL_LoadBMP("images/BOMBE.bmp");
	*flamme = SDL_LoadBMP("images/flamme.bmp");
	*itemFlamme = SDL_LoadBMP("images/item_flamme.bmp");
	*itemRoller = SDL_LoadBMP("images/item_roller.bmp");
	*itemPied = SDL_LoadBMP("images/item_pied.bmp");
	*itemBombe = SDL_LoadBMP("images/item_bombe.bmp");
	
	perso[BAS] = SDL_LoadBMP("images/perso_bas.bmp");
	perso[HAUT] = SDL_LoadBMP("images/perso_haut.bmp");
	perso[GAUCHE] = SDL_LoadBMP("images/perso_gauche.bmp");
	perso[DROITE] = SDL_LoadBMP("images/perso_droite.bmp");
	
	// On indique que la couleur jaune doit être rendue transparente
	for(i=0; i<4; i++)
	{
		SDL_SetColorKey(perso[i], SDL_SRCCOLORKEY, SDL_MapRGB(perso[i]->format, 255, 255, 0));
	}
	
	return;
}


void initJoueur(Perso *joueur, SDL_Surface *skinInitial, int posX, int posY, char pseudo[])
{
	strcpy(joueur->pseudo, pseudo);
	joueur->persoActuel = skinInitial; // On initialise le perso vers une direction
	joueur->hitbox.x = posX; // Positions en pixels
	joueur->hitbox.y = posY;
	joueur->hitbox.w = joueur->persoActuel->w - 10;
	joueur->hitbox.h = joueur->hitbox.w + 5;

	joueur->position.x = joueur->hitbox.x - 4;
	joueur->position.y = joueur->hitbox.y + joueur->hitbox.h - joueur->persoActuel->h + 2;
	/*printf("Perso : pos(%d,%d) taille(%d,%d)\nHitbox : pos(%d,%d) taille(%d,%d)\n",
			joueur->position.x, joueur->position.y, joueur->persoActuel->w, joueur->persoActuel->h,
			joueur->hitbox.x, joueur->hitbox.y, joueur->hitbox.w, joueur->hitbox.h);*/
	
	joueur->bombePosee = 0;
	joueur->totalBombes = 10;
	joueur->bombesRestantes = joueur[0].totalBombes;
	joueur->puissanceBombe = 3;
	joueur->vitesse = VITESSE;
	
	joueur->coin[HG].x = joueur->hitbox.x;
	joueur->coin[HG].y = joueur->hitbox.y;
	joueur->coin[HD].x = joueur->hitbox.x + joueur->hitbox.w;
	joueur->coin[HD].y = joueur->hitbox.y;
	joueur->coin[BD].x = joueur->hitbox.x + joueur->hitbox.w;
	joueur->coin[BD].y = joueur->hitbox.y + joueur->hitbox.h;
	joueur->coin[BG].x = joueur->hitbox.x;
	joueur->coin[BG].y = joueur->hitbox.y + joueur->hitbox.h;
	
	initialiserTouches(joueur);
	
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


void genererItems(Liste *items, int carte[][NB_CASES])
{
	int i=0, itemCourant = ITEM_FLAMME, bool[4]={1}, k=1;
	Position pos;
	Maillon *item = NULL;
	
	
	for(i=0; i<NB_ITEMS; i++)
	{
		do
		{
			pos.x = rand()%(NB_CASES-1) + 1; // Génère une position entre 1 et NB_CASES-1
			pos.y = rand()%(NB_CASES-1) + 1;
			
		}while(carte[pos.y][pos.x] != BRIQUE || chercherElement(pos, items, NULL) != NULL);
		
		item = ajouterElementFin(items, 0, pos, 0, NULL);
		item->type = itemCourant;
		
		printf("(%d,%d)\n", pos.x, pos.y);
		
		// Système qui permet de définir quand est-ce qu'on doit changer d'item à placer
		if(i+1 == bool[0]*NB_ITEM_FLAMME + bool[1]*NB_ITEM_BOMBE + bool[2]*NB_ITEM_ROLLER + bool[3]*NB_ITEM_PIED)
		{
			itemCourant++;
			bool[k]=1;
			k++;
		}
	}
	
	
	return;
}

