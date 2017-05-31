#include <stdio.h>
#include <stdlib.h>
#include "constante.h"
#include <SDL/SDL.h>
#include "jeu.h"
#include "listes_chainees/bomberman_fct_listes_ch.h"
#include <time.h>
#include "initialisations.h"



int jouerPartie(SDL_Surface* ecran)
{
	SDL_Event event;
	SDL_Surface *mur = NULL, *brique = NULL, *bombe = NULL, *flamme = NULL, *itemBombe = NULL, *itemFlamme = NULL,
				*itemPied = NULL, *itemRoller = NULL;
	SDL_Surface *perso[4] = {NULL};
	Perso joueur[4];
	int carte[NB_CASES][NB_CASES] = {0}, continuer = 1, i = 0, nbJoueurs = 2, murACasser[4] = {0}, direction = 0;
	Uint32 tempsActuel = 0;
	Liste *bombesPosees = initialiserListe(), *bombesExplosees = initialiserListe(), *items = initialiserListe();
		
	// INITIALISATIONS
	
		// Surfaces
	initSurfaces(&mur, &brique, &bombe, &flamme, &itemBombe, &itemFlamme, &itemPied, &itemRoller, perso);
	
		// Modèle : void initJoueur(Perso *joueur, SDL_Surface *skinInitial, int posX, int posY)
	initJoueur(&joueur[0], perso[BAS], CASE, CASE);
	initJoueur(&joueur[1], perso[BAS], CASE, 13*CASE);

	for(i=0; i<4; i++)
	{
		initialiserTouches(&joueur[i]);
	}
	
		// Lecture du niveau
	lireFichier(carte);
	
		// Génération d'items
	genererItems(items, carte);
	
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
						joueur[0].persoActuel = perso[HAUT];
						break;

					case SDLK_DOWN:
						joueur[0].touche[BAS] = 1;
						joueur[0].persoActuel = perso[BAS];
						break;

					case SDLK_LEFT:
						joueur[0].touche[GAUCHE] = 1;
						joueur[0].persoActuel = perso[GAUCHE];
						break;

					case SDLK_RIGHT:
						joueur[0].touche[DROITE] = 1;
						joueur[0].persoActuel = perso[DROITE];
						break;
						
					case SDLK_RCTRL: // Poser une bombe
						if(joueur[0].bombesRestantes && !joueur[0].bombePosee)
							// Si il lui reste une bombe ET qu'il est pas déjà en train d'appuyer sur la touche
						{
							joueur[0].bombePosee = 1;
								// Booléenne qui indique qu'il est en train d'appuyer sur la touche
							poserBombe(&joueur[0], bombesPosees, carte);
						}
						break;
						
					// TOUCHES JOUEUR 1
					
					case SDLK_z:
						joueur[1].touche[HAUT] = 1;
						joueur[1].persoActuel = perso[HAUT];
						break;

					case SDLK_s:
						joueur[1].touche[BAS] = 1;
						joueur[1].persoActuel = perso[BAS];
						break;

					case SDLK_q:
						joueur[1].touche[GAUCHE] = 1;
						joueur[1].persoActuel = perso[GAUCHE];
						break;

					case SDLK_d:
						joueur[1].touche[DROITE] = 1;
						joueur[1].persoActuel = perso[DROITE];
						break;
						
					case SDLK_v:
						if(joueur[1].bombesRestantes && !joueur[1].bombePosee)
							// Si il lui reste une bombe ET qu'il est pas déjà en train d'appuyer sur la touche
						{
							joueur[1].bombePosee = 1;
								// Booléenne qui indique qu'il est en train d'appuyer sur la touche
							poserBombe(&joueur[1], bombesPosees, carte);
						}
						break;
				}	
				break;
				
			// RELACHEMENT D'UNE TOUCHE
				
        	case SDL_KEYUP:
				
				switch(event.key.keysym.sym)
				{
					// TOUCHES JOUEUR 0
					
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
						
					// TOUCHES JOUEUR 1
					
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
						
					case SDLK_v:
						joueur[1].bombePosee = 0;
						break;
				}
				break;		
		}
		
		// DEPLACEMENT DES JOUEURS
		
		for(i=0; i<nbJoueurs; i++)
		{
			direction = chercherDirection(joueur[i].touche);
			
			if(direction < 4 && verifierDeplacement(joueur[i], direction, carte) == VIDE) // Si une direction a été détectée
			{
				deplacerJoueur(&joueur[i], direction);
			}
			
		}
		
		// TRAITEMENT DES BOMBES
		
		// Vérification des bombes qui attendent d'exploser
		
		if(bombesPosees->taille && verifierDelai(bombesPosees->premier->instant, DELAI_BOMBE))
			// On vérifie la première bombe de la liste des bombes posées
		{
			exploserBombe(carte, bombesPosees->premier, bombesPosees, bombesExplosees);
		}
		
		// Vérification du délai d'affichage des flammes, cassage des briques
		
		if(bombesExplosees->taille && verifierDelai(bombesExplosees->premier->instant, DELAI_BOMBE+DELAI_FLAMMES)
		{
			afficherExplosion(carte, bombesExplosees->premier, VIDE);
				// Effacement de l'explosion de la 1ere bombe de la liste (en remplaçant les FLAMME par VIDE)
			casserEntite(bombesExplosees->premier, carte, items);
			supprimerElement(bombesExplosees, 0);
		}
		
		// VERIFICATION DE LA MORT DU JOUEUR ET DE LA PRISE D'UN ITEM
		
		if(items->nbItemsSurTerrain || bombesExplosees->taille)
		{
			
		}
		
		
		
		
		// MAJ DE L'ECRAN, COLLAGE DES SURFACES
		
		SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255, 255, 255)); // Fond de la fenêtre : blanc
		blitterSurfaces(ecran, carte, mur, brique, bombe, flamme, itemFlamme, itemBombe, itemPied, itemRoller);
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



int verifierDeplacement(Perso joueur, int direction, int carte[][NB_CASES])
{
	Position coin[2];
	
	// Définition des 2 coins
	
	switch(direction)
	{
		case HAUT:
			
			if(joueur.hitbox.y >= joueur.vitesse) // Si il ne cherche pas  sortir de la map
			{
				coin[0].x = (joueur.hitbox.x)/CASE;
				coin[0].y = (joueur.hitbox.y-joueur.vitesse)/CASE;
				coin[1].x = (joueur.hitbox.x + joueur.hitbox.w)/CASE;
				coin[1].y = (joueur.hitbox.y-joueur.vitesse)/CASE;
			}

			
			break;
			
		case BAS:
		
			if(joueur.hitbox.y + joueur.hitbox.h + joueur.vitesse <= NB_CASES * CASE) // Si il ne cherche pas  sortir de la map
			{
				coin[0].x = (joueur.hitbox.x)/CASE;
				coin[0].y = (joueur.hitbox.y+joueur.hitbox.h+joueur.vitesse)/CASE;
				coin[1].x = (joueur.hitbox.x + joueur.hitbox.w)/CASE;
				coin[1].y = (joueur.hitbox.y+joueur.hitbox.h+joueur.vitesse)/CASE;
			}

			break;
			
		case GAUCHE:
		
			if(joueur.hitbox.x >= joueur.vitesse) // Si il ne cherche pas  sortir de la map
			{
				coin[0].x = (joueur.hitbox.x-joueur.vitesse)/CASE;
				coin[0].y = (joueur.hitbox.y)/CASE;
				coin[1].x = (joueur.hitbox.x-joueur.vitesse)/CASE;
				coin[1].y = (joueur.hitbox.y+joueur.hitbox.h)/CASE;
			}

			break;
			
		case DROITE:
		
			if(joueur.hitbox.x + joueur.hitbox.w + joueur.vitesse <= NB_CASES * CASE) // Si il ne cherche pas  sortir de la map
			{
				coin[0].x = (joueur.hitbox.x+joueur.hitbox.w+joueur.vitesse)/CASE;
				coin[0].y = (joueur.hitbox.y)/CASE;
				coin[1].x = (joueur.hitbox.x+joueur.hitbox.w+joueur.vitesse)/CASE;
				coin[1].y = (joueur.hitbox.y+joueur.hitbox.h)/CASE;
			}

			break;
	}
	
	// Vérification des collisions
	
	if(carte[coin[0].y][coin[0].x] == MUR || carte[coin[0].y][coin[0].x] == BRIQUE ||
		carte[coin[1].y][coin[1].x] == MUR || carte[coin[1].y][coin[1].x] == BRIQUE)
	{
		return 1; // Le joueur ne peut pas avancer
	}
	
	return VIDE;
}



void deplacerJoueur(Perso *joueur, int direction)
{
	int i = 0, vertical = 0, horizontal = 0;
	
	// Déplacement du joueur si une touche a été appuyée
	
	switch(direction)
	{
		case HAUT:
			joueur->hitbox.y -= joueur->vitesse;
			vertical = -1;
			break;
		case BAS:
			joueur->hitbox.y += joueur->vitesse;
			vertical = 1;
			break;
		case GAUCHE:
			joueur->hitbox.x -= joueur->vitesse;
			horizontal = -1;
			break;
		case DROITE:
			joueur->hitbox.x += joueur->vitesse;
			horizontal = 1;
			break;
	}
		
	// Redéfinition de la position de l'image du joueur grâce à la hitbox
	joueur->position.x = joueur->hitbox.x - 4;
	joueur->position.y = joueur->hitbox.y + joueur->hitbox.h - joueur->persoActuel->h + 2;
	
	// Redéfinition des positions sur la carte des coins
	
	
	for(i=0; i<4; i++)
	{
		joueur->coin[i].x += horizontal*joueur->vitesse;
		joueur->coin[i].y += vertical*joueur->vitesse;
	}
	
	return;
}



void poserBombe(Perso *joueur, Liste *bombesPosees, int carte[][NB_CASES])
{
	Position repereBombe;
	int instantBombe = 0;
	
	repereBombe.x = (joueur->hitbox.x + (joueur->hitbox.w)/2)/CASE;
	repereBombe.y = (joueur->hitbox.y + joueur->hitbox.h - 10)/CASE;
	// Indique où est-ce que la bombe doit être posée dans la carte de jeu
	// repereBombe pile entre les 2 genoux du personnage, c'est le repère pour poser la bombe
	
	carte[repereBombe.y][repereBombe.x] = BOMBE;
	
	instantBombe = (int)SDL_GetTicks();
	ajouterElementFin(bombesPosees, instantBombe, repereBombe, joueur->puissanceBombe, joueur);
		// Ajoute l'instant auquel la bombe a été posée, ainsi que ses positions dans la carte
	joueur->bombesRestantes--;
	
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
		/* ICI J'AI FAIT UN TRUC DE OUF:
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



/* Vérifie le parcours de la flamme de la bombe
 * Retourne 1 : la flamme peut continuer à parcourir la map
 * Retourne 2 : la flamme a rencontré une brique
 * Retourne 0 : elle doit s'arrêter (rencontre un mur)
 */
int verifierFlamme(int carte[][NB_CASES], Position posFlamme)
{
	int valeur = 0;
	
	if(posFlamme.y >= 0 && posFlamme.y <= NB_CASES-1 &&
		posFlamme.x >= 0 && posFlamme.x <= NB_CASES-1)
	{
		if(carte[posFlamme.y][posFlamme.x] == VIDE ||
			carte[posFlamme.y][posFlamme.x] == FLAMME ||
			carte[posFlamme.y][posFlamme.x] == BOMBE)
		{
			valeur = 1; // Elle peut continuer à parcourir
		}
		else if(carte[posFlamme.y][posFlamme.x] == BRIQUE || carte[posFlamme.y][posFlamme.x] == ITEM_BOMBE
					|| carte[posFlamme.y][posFlamme.x] == ITEM_FLAMME || carte[posFlamme.y][posFlamme.x] == ITEM_PIED
					|| carte[posFlamme.y][posFlamme.x] == ITEM_ROLLER)
		{
			valeur = 2;
		}
		else
		{
			valeur = 0;
		}
	}
	
	return valeur;
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

// A TERMINER CETTE FONCTION


/* Return 1 : le joueur est mort cheh
 * Return 2 : le joueur a chopé un item
 * Return 0 : rien ne se passe
 */
int verifierCollision(Position coin[], int carte[][NB_CASES])
{
	int i=0;
	Position pos;
	
	for(i=0; i<4; i++)
	{
		if(carte[coin[i].y][coin[i].x] == FLAMME)
		{
			return 1;
		}
		else if(carte[coin[i].y][coin[i].x] == ITEM_BOMBE || carte[coin[i].y][coin[i].x] == ITEM_FLAMME ||
				carte[coin[i].y][coin[i].x] == ITEM_ROLLER || carte[coin[i].y][coin[i].x] == ITEM_ROLLER)
		{
			return 
		}
	}
}



