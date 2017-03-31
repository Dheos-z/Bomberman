#include <stdio.h>
#include <stdlib.h>
#include "constante.h"
#include <SDL/SDL.h>
#include "fonction.h"
#include "jeu.h"




int jouerPartie(SDL_Surface* ecran)
{
	SDL_Event event;
	SDL_Surface *mur = NULL, *brique = NULL, *bombe = NULL;
	SDL_Surface *perso[4] = {NULL};
	Perso joueur[4];
	int carte[NB_CASES][NB_CASES] = {0}, continuer = 1, i = 0, nbJoueurs = 2;
		
	// INITIALISATIONS DES SURFACES ET DES POSITIONS
	
	mur = SDL_LoadBMP("MUR.bmp");
	brique = SDL_LoadBMP("BRIQUE.bmp");
	bombe = SDL_LoadBMP("BOMBE.bmp");
	
	perso[BAS] = SDL_LoadBMP("BAS.bmp");
	perso[HAUT] = SDL_LoadBMP("HAUT.bmp");
	perso[GAUCHE] = SDL_LoadBMP("GAUCHE.bmp");
	perso[DROITE] = SDL_LoadBMP("DROITE.bmp");
	
	// Joueur 0
	
	joueur[0].persoActuel = perso[BAS]; // On initialise le perso vers le bas
	joueur[0].position.x = CASE;
	joueur[0].position.y = CASE;
	joueur[0].totalBombes = 10;
	joueur[0].bombesRestantes = joueur[0].totalBombes;
	
	// Joueur 1
	
	joueur[1].persoActuel = perso[BAS];
	joueur[1].position.x = CASE;
	joueur[1].position.y = 13*CASE;
	joueur[1].totalBombes = 10;
	joueur[1].bombesRestantes = joueur[1].totalBombes;

	
	for(i=0; i<4; i++)
	{
		initialiserTouches(&joueur[i]);
	}
	
	// Lecture du niveau
	
	lireFichier(carte);
	afficherCarte(carte);
	
	
	// Boucle des événements
	
	while(continuer)
	{
		SDL_PollEvent(&event);
		
		
		
		switch(event.type)
		{
			case SDL_QUIT: // Si on appuie sur la croix
				continuer = 0;
				break;
				
			// APPUI SUR UNE TOUCHE
				
        	case SDL_KEYDOWN:
				
				switch(event.key.keysym.sym)
				{
					case SDLK_ESCAPE:
						continuer = 0; // La touche ECHAP permet de quitter le jeu
						break;

					// TOUCHES PERSO 0
					
					case SDLK_UP:
						joueur[0].touche[HAUT] = 1;
						break;

					case SDLK_DOWN:
						joueur[0].touche[BAS] = 1;
						break;

					case SDLK_LEFT:
						joueur[0].touche[GAUCHE] = 1;
						break;

					case SDLK_RIGHT:
						joueur[0].touche[DROITE] = 1;
						break;
						
					case SDLK_RCTRL: // Poser une bombe
						if(joueur[0].bombesRestantes)
						{
							poserBombe(joueur[0], carte);
							joueur[0].bombesRestantes--;
							afficherCarte(carte);
						}
						break;
						
					// TOUCHES PERSO 1
					
					case SDLK_z:
						joueur[1].touche[HAUT] = 1;
						break;

					case SDLK_s:
						joueur[1].touche[BAS] = 1;
						break;

					case SDLK_q:
						joueur[1].touche[GAUCHE] = 1;
						break;

					case SDLK_d:
						joueur[1].touche[DROITE] = 1;
						break;
				}	
				break;	
				
			// RELACHEMENT D'UNE TOUCHE
				
        	case SDL_KEYUP:
				
				switch(event.key.keysym.sym)
				{
					// TOUCHES PERSO 0
					
					case SDLK_UP:
						joueur[0].touche[HAUT] = 0;
						break;

					case SDLK_DOWN:
						joueur[0].touche[BAS] = 0;
						break;

					case SDLK_LEFT:
						joueur[0].touche[GAUCHE] = 0;
						break;

					case SDLK_RIGHT:
						joueur[0].touche[DROITE] = 0;
						break;
						
					// TOUCHES PERSO 1
					
					case SDLK_z:
						joueur[1].touche[HAUT] = 0;
						break;

					case SDLK_s:
						joueur[1].touche[BAS] = 0;
						break;

					case SDLK_q:
						joueur[1].touche[GAUCHE] = 0;
						break;

					case SDLK_d:
						joueur[1].touche[DROITE] = 0;
						break;
				}
				break;		
		}
		
		// Déplacement des joueurs
		
		for(i=0; i<nbJoueurs; i++)
		{
			deplacerJoueur(&joueur[i]);
		}
		
		// Mise à jour de l'écran
		
		SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255, 255, 255)); // Fond de la fenêtre : blanc
		blitterSurfaces(ecran, carte, mur, brique, bombe);
		blitterPerso(ecran, joueur, nbJoueurs);
		SDL_Flip(ecran);
	}
	
	// Libération des surfaces
	
	SDL_FreeSurface(mur);
	SDL_FreeSurface(brique);
	SDL_FreeSurface(bombe);
	for(i = 0; i < 4; i++)
	{
		SDL_FreeSurface(perso[i]);
		// persoActuel est libéré dans cette boucle car il est égal à une des 4 surfaces libérées ici
	}
	
	
	return 0; // Pour l'instant
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


void blitterSurfaces(SDL_Surface *ecran, int carte[][NB_CASES], SDL_Surface *mur, 
					SDL_Surface *brique, SDL_Surface *bombe)
{
	int i = 0, j = 0;
	SDL_Rect position;
	
	for(i = 0; i < NB_CASES; i++)
	{
		position.y = i*CASE;
		
		for(j = 0; j < NB_CASES; j++)
		{
			position.x = j*CASE;
			
			switch(carte[i][j])
			{
				case MUR:
					SDL_BlitSurface(mur, NULL, ecran, &position);
					break;
				
				case BRIQUE:
					SDL_BlitSurface(brique, NULL, ecran, &position);
					break;
					
				case BOMBE:
					SDL_BlitSurface(bombe, NULL, ecran, &position);
					break;
			}
		}
	}
	
	return;
}


void blitterPerso(SDL_Surface *ecran, Perso joueur[], int nbJoueurs)
{
	int i = 0;
	
	for(i=0; i<nbJoueurs; i++)
	{
		SDL_BlitSurface(joueur[i].persoActuel, NULL, ecran, &joueur[i].position);
	}
	
	return;
}


void afficherCarte(int carte[][NB_CASES])
{
	int i=0, j=0;
	
	for(i=0; i<NB_CASES; i++)
	{
		for(j=0; j<NB_CASES; j++)
		{
			printf("%d", carte[i][j]);
		}
		printf("\n");
	}
	
	return;
}


void deplacerJoueur(Perso *joueur)
{
	int i=0;
	
	// Recherche de la touche appuyée (HAUT, BAS, GAUCHE, DROITE, ou rien)
	while(i<4 && joueur->touche[i] == 0)
	{
		i++;
	}
	//printf("i vaut %d\n", i);
	//printf("La touche vaut %d\n", joueur.touche[i]);
	
	// Déplacement du joueur si une touche a été appuyée
	if(joueur->touche[i])
	{
		switch(i)
		{
			case HAUT:
				joueur->position.y -= 5;
				break;
			case BAS:
				joueur->position.y += 5;
				break;
			case GAUCHE:
				joueur->position.x -= 5;
				break;
			case DROITE:
				joueur->position.x += 5;
				//printf("\nDeplacement du joueur\n"); 
				break;
		}
	}
	
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



void poserBombe(Perso joueur, int carte[][NB_CASES])
{
	Position repereBombe;
	
	repereBombe.x = (joueur.position.x + T_PERSO/2)/CASE;
	repereBombe.y = (joueur.position.y + T_PERSO - 1)/CASE;
	// Indique où est-ce que la bombe doit être posée dans la carte de jeu
	// repereBombe pile entre les 2 pieds du personnage, c'est le repère pour poser la bombe
	
	carte[repereBombe.y][repereBombe.x] = BOMBE;
	
	return;
}
