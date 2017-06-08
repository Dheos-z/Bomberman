#include <stdio.h>
#include <stdlib.h>
#include "constante.h"
#include "jeu.h"
#include "listes_chainees/bomberman_fct_listes_ch.h"
#include "fonctions_jeu.h"
#include "fonctions_verifier.h"


void blitterSurfaces(SDL_Surface *ecran, int carte[][NB_CASES], SDL_Surface *mur, 
					SDL_Surface *brique, SDL_Surface *bombe, SDL_Surface *flamme, SDL_Surface *itemFlamme, 
					SDL_Surface *itemBombe, SDL_Surface *itemPied, SDL_Surface *itemRoller)
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
					break;
					
				case ITEM_FLAMME:
					SDL_BlitSurface(itemFlamme, NULL, ecran, &position);
					break;	
								
				case ITEM_BOMBE:
					SDL_BlitSurface(itemBombe, NULL, ecran, &position);
					break;		
								
				case ITEM_ROLLER:
					SDL_BlitSurface(itemRoller, NULL, ecran, &position);
					break;
					
				case ITEM_PIED:
					SDL_BlitSurface(itemPied, NULL, ecran, &position);
					break;
			}
		}
	}
	
	return;
}


void blitterPerso(SDL_Surface *ecran, ListePerso *joueurs)
{
	Perso *joueur = joueurs->premier;
	
	while(joueur != NULL)
	{
		SDL_BlitSurface(joueur->persoActuel, NULL, ecran, &joueur->position);
		joueur = joueur->suivant;
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


int chercherDirection(int touche[]) // Permet de détecter la direction dans laquelle veut aller le joueur
{
	int i=0;
	
	// Recherche de la touche appuyée (HAUT, BAS, GAUCHE, DROITE, ou rien)
	while(i<4 && touche[i] == 0)
	{
		i++;
	}
	
	// Si aucune touche n'a été détectée, i vaudra 4
	return i;
}


void deplacerJoueur(Perso *joueur, int direction)
{
	int i = 0;
	
	// Déplacement du joueur si une touche a été appuyée
	
	switch(direction)
	{
		case HAUT:
			joueur->hitbox.y -= joueur->vitesse;
			break;
		case BAS:
			joueur->hitbox.y += joueur->vitesse;
			break;
		case GAUCHE:
			joueur->hitbox.x -= joueur->vitesse;
			break;
		case DROITE:
			joueur->hitbox.x += joueur->vitesse;
			break;
	}
		
	// Redéfinition de la position de l'image du joueur grâce à la hitbox
	joueur->position.x = joueur->hitbox.x - 4;
	joueur->position.y = joueur->hitbox.y + joueur->hitbox.h - joueur->persoActuel->h + 2;
	
	
	// Redéfinition des positions sur la carte des coins
	
	joueur->coin[HG].x = joueur->hitbox.x/CASE;
	joueur->coin[HG].y = joueur->hitbox.y/CASE;
	joueur->coin[HD].x = (joueur->hitbox.x + joueur->hitbox.w)/CASE;
	joueur->coin[HD].y = joueur->hitbox.y/CASE;
	joueur->coin[BD].x = (joueur->hitbox.x + joueur->hitbox.w)/CASE;
	joueur->coin[BD].y = (joueur->hitbox.y + joueur->hitbox.h)/CASE;
	joueur->coin[BG].x = joueur->hitbox.x/CASE;
	joueur->coin[BG].y = (joueur->hitbox.y + joueur->hitbox.h)/CASE;
	
	return;
}



void poserBombe(Perso *joueur, Liste *bombesPosees, int carte[][NB_CASES], ListePerso *joueurs)
{
	Position repereBombe;
	int instantBombe = 0;
	Entite entite;
	
	repereBombe.x = (joueur->hitbox.x + (joueur->hitbox.w)/2)/CASE;
	repereBombe.y = (joueur->hitbox.y + joueur->hitbox.h - 10)/CASE;
	// Indique où est-ce que la bombe doit être posée dans la carte de jeu
	// repereBombe pile entre les 2 genoux du personnage, c'est le repère pour poser la bombe
	
	carte[repereBombe.y][repereBombe.x] = BOMBE;
	
	instantBombe = (int)SDL_GetTicks();
	ajouterElementFin(bombesPosees, instantBombe, repereBombe, joueur->puissanceBombe, joueur);
		// Ajoute l'instant auquel la bombe a été posée, ainsi que ses positions dans la carte
	joueur->bombesRestantes--;
	
	// Finalement, non seulement on veut indiquer que le joueur qui vient de poser la bombe
	// est assis sur une bombe, mais on veut aussi vérifier si les autres joueurs sont assis
	// sur cette même bombe
	
	joueur = joueurs->premier;
	
	while(joueur != NULL)
	{
		entite = verifierCollision(joueur->coin, carte);
		
		if(entite.bool == 3) // Si le joueur considéré est présent sur la bombe
		{
			joueur->assisSurBombe = 1;
		}
		
		joueur = joueur->suivant;
	}
	
	return;
}


void exploserBombe(int carte[][NB_CASES], Maillon *bombe, Liste *bombesPosees, Liste *bombesExplosees)
{
	Maillon *bombeExplosee = NULL;
	
	bombeExplosee = ajouterElementFin(bombesExplosees, bombe->instant, bombe->position, bombe->puissance, bombe->joueur);
		/* On ajoute une bombe explosée pour ensuite pour ensuite exploiter
		 * ses données + on récupère cette bombe */
	
	determinerPortee(carte, bombeExplosee, bombesPosees);
	afficherExplosion(carte, bombeExplosee, FLAMME);
	
	supprimerElement(bombesPosees, 0);
		// Supprime la 1ere bombe de la liste des bombes en attente d'explosion (pcq elle a explosé)
	bombe->joueur->bombesRestantes++;
	
	return;
}


void determinerPortee(int carte[][NB_CASES], Maillon *bombeExplosee, Liste *bombesPosees)
{
	int i = 0, k=0, bool[4] = {1, 1, 1, 1}, flamme = 0;
		// bool sert à indiquer le moment où on arrête de vérifier la portée de la bombe à une direction donnée
	Position posBombe, posFlamme[4];
	
	
	posBombe.x = bombeExplosee->position.x;
	posBombe.y = bombeExplosee->position.y;
	
	for(i=1; i<=bombeExplosee->puissance; i++)
	{
		/* ICI J'AI FAIT UN TRUC DE GENIE:
		 * UNE SEULE BOUCLE DE 0 A 4, JE DEFINIS POSFLAMME, JE VERIFIE CE QU'IL Y A
		 * DANS LA FLAMME ET EN FONCTION JE FAIS CE QU'IL Y A A FAIRE
		 */
		 
		posFlamme[HAUT].x = posBombe.x;
		posFlamme[HAUT].y = posBombe.y - i;
		posFlamme[BAS].x = posBombe.x;
		posFlamme[BAS].y = posBombe.y + i;
		posFlamme[GAUCHE].x = posBombe.x - i;
		posFlamme[GAUCHE].y = posBombe.y;
		posFlamme[DROITE].x = posBombe.x + i;
		posFlamme[DROITE].y = posBombe.y;
		
		for(k=0; k<4; k++) // Chaque direction est vérifiée
		{
			if(bool[k]) // Si la flamme peut parcourir dans la direction k
			{
				flamme = verifierFlamme(carte, posFlamme[k]);
				
				if(flamme == 1) // La flamme peut continuer à parcourir
				{
					bombeExplosee->portee[k]++;
					
					if(carte[posFlamme[k].y][posFlamme[k].x] == BOMBE)
						// Si la flamme rencontre une bombe elle entraîne son explosion
					{
						entrainerExplosion(bombesPosees, posFlamme[k]);
					}
				}
				
				else if(flamme == 2) // La flamme rencontre une brique ou un item
				{
					bombeExplosee->entite[k].bool = 1;
					bombeExplosee->entite[k].position.x = posFlamme[k].x;
					bombeExplosee->entite[k].position.y = posFlamme[k].y;
					bool[k] = 0; // La flamme s'arrête de parcourir
				}
				
				else // Si la flamme rencontre autre chose (genre un mur)
				{
					bool[k] = 0; // La flamme s'arrête de parcourir
				}
			}
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


void casserEntite(Maillon *bombe, int carte[][NB_CASES], Liste *items)
{
	int i=0, rang = 0;
	Position posEntite;
	Maillon *item = NULL;
	
	for(i=0; i<4; i++)
	{
		if(bombe->entite[i].bool) // Si bool vaut 1, l'entité (brique ou item) peut se casser à la position trouvée
		{
			posEntite.x = bombe->entite[i].position.x;
			posEntite.y = bombe->entite[i].position.y;
			
			if(carte[posEntite.y][posEntite.x] == ITEM_BOMBE || carte[posEntite.y][posEntite.x] == ITEM_FLAMME || // Si la flamme doit brûler un item
				carte[posEntite.y][posEntite.x] == ITEM_PIED || carte[posEntite.y][posEntite.x] == ITEM_ROLLER)
			{
				carte[posEntite.y][posEntite.x] = VIDE;
				items->nbItemsSurTerrain--;
			}
			else if(carte[posEntite.y][posEntite.x] == BRIQUE) // Si elle doit casser une brique
			{
				item = chercherElement(posEntite, items, &rang);
				
				if(item != NULL) // Si la brique contient un item 
				{
					carte[posEntite.y][posEntite.x] = item->type;
					items->nbItemsSurTerrain++;
					supprimerElement(items, rang);
				}
				else // Si la brique ne contient rien
				{
					carte[posEntite.y][posEntite.x] = VIDE;
				}
			}
			
			bombe->entite[i].bool = 0;
		}
	}
	
	return;
}


void entrainerExplosion(Liste *liste, Position posFlamme)
{
	int rang = 0;
	Maillon *bombePassage = NULL;
	
	
	bombePassage = chercherElement(posFlamme, liste, &rang);
	bombePassage->instant = (int)0-DELAI_BOMBE;
		// On fait en sorte que son délai soit déjà passé pour qu'elle pète
		
	if(rang > 1)
	{
		deplacerElement(liste, rang, 1);
	}
	
	return;
}




void recupererItem(Position posItem, int carte[][NB_CASES], Perso *joueur)
{
	switch(carte[posItem.y][posItem.x])
	{
		case ITEM_FLAMME:
			joueur->puissanceBombe++;
			break;
			
		case ITEM_BOMBE:
			joueur->totalBombes++;
			joueur->bombesRestantes++;
			break;
			
		case ITEM_ROLLER:
			joueur->vitesse++;
			break;
			
		case ITEM_PIED:
			printf("Le pied ne fait rien pour l'instant\n");
			break;
	}
	
	carte[posItem.y][posItem.x] = VIDE;
	
	return;
}
