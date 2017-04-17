#include <stdio.h>
#include <stdlib.h>
#include "constante.h"
#include <SDL/SDL.h>
#include "jeu.h"
#include "listes_chainees/bomberman_fct_listes_ch.h"




int jouerPartie(SDL_Surface* ecran)
{
	SDL_Event event;
	SDL_Surface *mur = NULL, *brique = NULL, *bombe = NULL, *flamme = NULL;
	SDL_Surface *perso[4] = {NULL};
	Perso joueur[4];
	int carte[NB_CASES][NB_CASES] = {0}, continuer = 1, i = 0, nbJoueurs = 2;
	Uint32 tempsActuel = 0;
	Liste *bombesExplosees = initialiserListe();
	Maillon *aSupp = NULL;
		
	// INITIALISATIONS
	
	initSurfaces(&mur, &brique, &bombe, &flamme, perso);
	
		// Modèle : void initJoueur(Perso *joueur, SDL_Surface *skinInitial, int posX, int posY)
	initJoueur(&joueur[0], perso[BAS], CASE, CASE);
	initJoueur(&joueur[1], perso[BAS], CASE, 13*CASE);

	
	for(i=0; i<4; i++)
	{
		initialiserTouches(&joueur[i]);
	}
	
		// Lecture du niveau
	lireFichier(carte);
	
	// BOUCLE DES EVENEMENTS
	
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

					// TOUCHES JOUEUR 0
					
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
						if(joueur[0].bombesRestantes && !joueur[0].bombePosee)
							// Si il lui reste une bombe ET qu'il est pas déjà en train d'appuyer sur la touche
						{
							joueur[0].bombePosee = 1; 
								// On indique qu'il est en train d'appuyer sur la touche
							poserBombe(&joueur[0], carte);
						}
						break;
						
					// TOUCHES JOUEUR 1
					
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
						
					case SDLK_RCTRL: // La touche de pose de bombe est relachée
						joueur[0].bombePosee = 0;
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
		
		// DEPLACEMENT DES JOUEURS
		
		for(i=0; i<nbJoueurs; i++)
		{
			deplacerJoueur(&joueur[i]);
		}
		
		// TRAITEMENT DES BOMBES
		
		// Vérification des bombes qui attendent d'exploser
		
		for(i=0; i<nbJoueurs; i++)
		{
			if(joueur[i].listeBombes->taille && verifierDelai(joueur[i].listeBombes->premier->instant, DELAI_BOMBE))
				// On vérifie la première bombe de la liste des bombes du joueur
			{
				exploserBombe(carte, &joueur[i], bombesExplosees);
			}
		}
		
		// Vérification du délai d'affichage des flammes
		
		if(bombesExplosees->taille && verifierDelai(bombesExplosees->premier->instant, DELAI_BOMBE+DELAI_FLAMMES))
		{
			afficherExplosion(carte, bombesExplosees->premier, VIDE);
				// Effacement de l'explosion de la bombe (en remplaçant les FLAMME par VIDE)
			supprimerBombe(bombesExplosees, 0);
		}
		
		
		
		
		// MAJ DE L'ECRAN, COLLAGE DES SURFACES
		
		SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255, 255, 255)); // Fond de la fenêtre : blanc
		blitterSurfaces(ecran, carte, mur, brique, bombe, flamme);
		blitterPerso(ecran, joueur, nbJoueurs);
		SDL_Flip(ecran);
	}
	
	// LIBERATION DES SURFACES
	
	SDL_FreeSurface(mur);
	SDL_FreeSurface(brique);
	SDL_FreeSurface(bombe);
	SDL_FreeSurface(flamme);
	for(i = 0; i < 4; i++)
	{
		SDL_FreeSurface(perso[i]);
		// persoActuel est libéré dans cette boucle car il est égal à une des 4 surfaces libérées ici
	}
	
	
	return 0; // Pour l'instant
}


void blitterSurfaces(SDL_Surface *ecran, int carte[][NB_CASES], SDL_Surface *mur, 
					SDL_Surface *brique, SDL_Surface *bombe, SDL_Surface *flamme)
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
				case FLAMME:
					SDL_BlitSurface(flamme, NULL, ecran, &position);
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
	
	// Déplacement du joueur si une touche a été appuyée
	if(joueur->touche[i])
	{
		switch(i)
		{
			case HAUT:
				joueur->position.y -= VITESSE;
				break;
			case BAS:
				joueur->position.y += VITESSE;
				break;
			case GAUCHE:
				joueur->position.x -= VITESSE;
				break;
			case DROITE:
				joueur->position.x += VITESSE;
				break;
		}
	}
	
	return;
}



void poserBombe(Perso *joueur, int carte[][NB_CASES])
{
	Position repereBombe;
	int bombeActuelle = joueur->totalBombes - joueur->bombesRestantes, instantBombe = 0;
	
	repereBombe.x = (joueur->position.x + T_PERSO/2)/CASE;
	repereBombe.y = (joueur->position.y + T_PERSO - 1)/CASE;
	// Indique où est-ce que la bombe doit être posée dans la carte de jeu
	// repereBombe pile entre les 2 pieds du personnage, c'est le repère pour poser la bombe
	
	carte[repereBombe.y][repereBombe.x] = BOMBE;
	
	instantBombe = (int)SDL_GetTicks();
	ajouterBombeFin(joueur->listeBombes, instantBombe, repereBombe, NULL);
		// Ajoute l'instant auquel la bombe a été posée, ainsi que ses positions dans la carte
	joueur->bombesRestantes--;
	
	// Affichage pour voir si tout fonctionne
	
	afficherCarte(carte);
	afficherListe(joueur->listeBombes); // Test pr voir si ça marche (à supp)
	printf("\nBombes restantes : %d\n", joueur->bombesRestantes);
	
	return;
}

// Renvoie 1 si le délai est passé
int verifierDelai(int instant, int delai)
{
	int instantActuel = (int)SDL_GetTicks();
	
	if(instantActuel-instant >= delai)
	{
		return 1;
	}
	
	return 0;
}


void exploserBombe(int carte[][NB_CASES], Perso *joueur, Liste *bombesExplosees)
{
	Maillon *bombeExplosee = NULL;
	Position posBombe;
	int portee[4] = {0}, instantBombe = 0;
	
	posBombe.x = joueur->listeBombes->premier->position.x;
	posBombe.y = joueur->listeBombes->premier->position.y;
	instantBombe = joueur->listeBombes->premier->instant;
		// Pour simplifier la lecture du code
	
	determinerPortee(carte, joueur->puissanceBombe, posBombe, portee);
	
	ajouterBombeFin(bombesExplosees, instantBombe, posBombe, portee);
		/* On ajoute une bombe explosée pour ensuite traiter le délai de
			présence des flammes sur la carte */
	
	supprimerBombe(joueur->listeBombes, 0);
		// Supprime la 1ere bombe de la liste, celle qui doit exploser
	joueur->bombesRestantes++;
	
	bombeExplosee = recupererElement(bombesExplosees, bombesExplosees->taille - 1);
		// On récupère la bombe qui vient d'exploser : le dernier élément de bombesExplosees
	afficherExplosion(carte, bombeExplosee, FLAMME);
		// On affiche son explosion
	
	return;
}


void determinerPortee(int carte[][NB_CASES], int puissanceBombe, Position posBombe, int portee[])
{
	int i = 0, bool[4] = {1, 1, 1, 1};
		// bool sert à indiquer le moment où on arrête de vérifier la portée de la bombe à une direction donnée 
	
	// INITIALISATION (on sait jamais)
	
	for(i=0; i<4; i++)
	{
		portee[i] = 0;
	}
	
	printf("\nPosBombe : x = %d, y = %d\n", posBombe.x, posBombe.y);
	
	// DETERMINATION PORTEE
	
	for(i=1; i<=puissanceBombe; i++)
	{
		// EN HAUT
		if(posBombe.y >= i && (carte[posBombe.y - i][posBombe.x] == VIDE ||
								carte[posBombe.y - i][posBombe.x] == FLAMME ||
								carte[posBombe.y - i][posBombe.x] == BOMBE) && bool[HAUT])
		{
			portee[HAUT]++;
		}
		else
		{
			bool[HAUT] = 0; // On arrête de vérifier cette direction
		}
		
		// EN BAS
		if(posBombe.y + i <= NB_CASES-1 && (carte[posBombe.y + i][posBombe.x] == VIDE ||
											carte[posBombe.y + i][posBombe.x] == FLAMME ||
											carte[posBombe.y + i][posBombe.x] == BOMBE) && bool[BAS])
		{
			portee[BAS]++;
		}
		else
		{
			bool[BAS] = 0; // On arrête de vérifier cette direction
		}
		
		
		// A GAUCHE
		if(posBombe.x >= i && (carte[posBombe.y][posBombe.x - i] == VIDE ||
								carte[posBombe.y][posBombe.x - i] == FLAMME ||
								carte[posBombe.y][posBombe.x - i] == BOMBE) && bool[GAUCHE])
		{
			portee[GAUCHE]++;
		}
		else
		{
			bool[GAUCHE] = 0; // On arrête de vérifier cette direction
		}
		
		// A DROITE
		if(posBombe.x + i <= NB_CASES-1 && (carte[posBombe.y][posBombe.x + i] == VIDE ||
											carte[posBombe.y][posBombe.x + i] == FLAMME ||
											carte[posBombe.y][posBombe.x + i] == BOMBE) && bool[DROITE])
		{
			portee[DROITE]++;
		}
		else
		{
			bool[DROITE] = 0; // On arrête de vérifier cette direction
		}
	}
	
	return;
}


void afficherExplosion(int carte[][NB_CASES], Maillon *bombe, int icone)
{
	// icone vaudra FLAMME si on veut afficher l'explosion, VIDE si on veut l'effacer
	int i = 0, j = 0;
	
	carte[bombe->position.y][bombe->position.x] = icone;
	
	for(i=0; i<4; i++)
	{
		for(j=1; j<=bombe->portee[i]; j++)
		{
			switch(i)
			{
				case HAUT:
					carte[bombe->position.y - j][bombe->position.x] = icone;
					break;
					
				case BAS:
					carte[bombe->position.y + j][bombe->position.x] = icone;
					break;
					
				case GAUCHE:
					carte[bombe->position.y][bombe->position.x - j] = icone;
					break;
					
				case DROITE:
					carte[bombe->position.y][bombe->position.x + j] = icone;
					break;
			}
		}
	}
	
	return;
}
