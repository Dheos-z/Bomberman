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
				*itemPied = NULL, *itemRoller = NULL, *perso[4] = {NULL};
	Perso *joueur = NULL, *idJoueur[4] = {NULL}, *joueurASupp = NULL; 
		// joueur va parcourir la liste, idJoueur contient l'adresse de chaque joueur (indispensable quand on teste l'appui des touches)
	int carte[NB_CASES][NB_CASES] = {0}, continuer = 1, i = 0, direction = 0, *bombeAPoser = NULL;
	Liste *bombesPosees = initialiserListe(), *bombesExplosees = initialiserListe(), *items = initialiserListe();
	ListePerso *joueurs = initialiserListePerso();
	Entite entite;
	Uint8 *touche = SDL_GetKeyState(NULL); // Tableau de booléens qui représente chaque touche et indique si la touche est appuyée
		
	// INITIALISATIONS
	
		// Surfaces
		
	initSurfaces(&mur, &brique, &bombe, &flamme, &itemBombe, &itemFlamme, &itemPied, &itemRoller, perso);
	
	
		// Joueurs
		// (Modèle : void initJoueur(Perso *joueur, SDL_Surface *skinInitial, int posX, int posY, char pseudo[]) )
		
	idJoueur[0] = ajouterPersoFin(joueurs);
	initJoueur(idJoueur[0], perso[BAS], CASE, CASE, 1);
	idJoueur[1] = ajouterPersoFin(joueurs);
	initJoueur(idJoueur[1], perso[BAS], CASE, 13*CASE, 2);
	idJoueur[2] = ajouterPersoFin(joueurs);
	initJoueur(idJoueur[2], perso[BAS], 13*CASE, 13*CASE, 3);
	idJoueur[3] = ajouterPersoFin(joueurs);
	initJoueur(idJoueur[3], perso[BAS], 13*CASE, CASE, 4);
	
	
		// Lecture du niveau
		
	lireFichier(carte);
	
	
		// Génération d'items
		
	genererItems(items, carte);
	
	// BOUCLE DES EVENEMENTS
	
	while(continuer)
	{
		SDL_PollEvent(&event); // Activation des événements en mode continu
		
		
		// GESTION DES TOUCHES
		
		if(touche[SDLK_ESCAPE])
			continuer = 0;
		
		bombeAPoser = verifierTouches(idJoueur, joueurs, touche, perso); // Renvoie le tableau qui indique si chaque joueur essaye de poser une bombe
		
		
		
		// POSE DE BOMBE
		
		for(i=0; i<joueurs->taille; i++)
		{
			if(bombeAPoser[i])
			{
				poserBombe(idJoueur[i], bombesPosees, carte, joueurs);
			}
		}
		

		
		// DEPLACEMENT DES JOUEURS
		
		joueur = joueurs->premier;
		
		while(joueur != NULL)
		{
			
			direction = chercherDirection(joueur->touche);
			
			if(direction < 4 && verifierDeplacement(joueur, direction, carte) == VIDE) // Si une direction a été détectée
			{
				deplacerJoueur(joueur, direction);
			}
			joueur = joueur->suivant;
		}
		
		
		
		// TRAITEMENT DES BOMBES
		
		
		// Vérification des bombes qui attendent d'exploser
		
		if(bombesPosees->taille && verifierDelai(bombesPosees->premier->instant, DELAI_BOMBE))
			// On vérifie la première bombe de la liste des bombes posées
		{
			exploserBombe(carte, bombesPosees->premier, bombesPosees, bombesExplosees);
		}
		
		// Vérification du délai d'affichage des flammes, cassage des briques
		
		if(bombesExplosees->taille && verifierDelai(bombesExplosees->premier->instant, DELAI_BOMBE+DELAI_FLAMMES))
		{
			afficherExplosion(carte, bombesExplosees->premier, VIDE);
				// Effacement de l'explosion de la 1ere bombe de la liste (en remplaçant les FLAMME par VIDE)
			casserEntite(bombesExplosees->premier, carte, items);
			supprimerElement(bombesExplosees, 0);
		}
		
		
		// VERIFICATION DE LA MORT DU JOUEUR ET DE LA PRISE D'UN ITEM
		
		
		if(items->nbItemsSurTerrain || bombesExplosees->taille)
		{
			joueur = joueurs->premier;
			
			
			while(joueur != NULL)
			{
				entite = verifierCollision(joueur->coin, carte);
				
				if(entite.bool == 1) // Si le joueur est sur une flamme
				{
					joueurASupp = joueur;
					joueur = joueur->suivant;
					idJoueur[joueurASupp->numero-1] = NULL;
					supprimerPerso(joueurs, joueurASupp);
				}
				else if(entite.bool == 2) // Si le joueur a chopé un item
				{
					printf("Oh! un item\n");
					recupererItem(entite.position, carte, joueur);
					joueur = joueur->suivant;
				}
				else
				{
					joueur = joueur->suivant;
				}
			}
		}
		
		
		
		// VERIFICATION DE LA FIN D'UNE PARTIE
		
		if(joueurs->taille == 1)
		{
			continuer = 0;
			printf("Fin de la partie !\nBravo Joueur %d !!\n", joueurs->premier->numero);
		}
		
		
		
		// MAJ DE L'ECRAN, COLLAGE DES SURFACES
		
		SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255, 255, 255)); // Fond de la fenêtre : blanc
		blitterSurfaces(ecran, carte, mur, brique, bombe, flamme, itemFlamme, itemBombe, itemPied, itemRoller);
		blitterPerso(ecran, joueurs);
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


// Ceci pourrait être fait plus proprement (avec moins de répétition) avec des coeff 1 ou -1
int verifierDeplacement(Perso *joueur, int direction, int carte[][NB_CASES])
{
	Position coinArriere[2], coinAvant[2], coinArriereJoueur[2], coinAvantJoueur[2];
	
	// Définition des 4 coins de la hitbox comme si il s'était déjà déplacé
	
	
	switch(direction)
	{
		case HAUT:
			
			if(joueur->hitbox.y >= joueur->vitesse) // Si il ne cherche pas  sortir de la map
			{
				coinAvantJoueur[0] = joueur->coin[0];
				coinAvantJoueur[1] = joueur->coin[1];
				coinArriereJoueur[0] = joueur->coin[3];
				coinArriereJoueur[1] = joueur->coin[2];
				
				coinAvant[0].x = (joueur->hitbox.x)/CASE;
				coinAvant[0].y = (joueur->hitbox.y-joueur->vitesse)/CASE;
				coinAvant[1].x = (joueur->hitbox.x + joueur->hitbox.w)/CASE;
				coinAvant[1].y = (joueur->hitbox.y-joueur->vitesse)/CASE;
				
				coinArriere[0].x = (joueur->hitbox.x)/CASE;
				coinArriere[0].y = (joueur->hitbox.y+joueur->hitbox.h-joueur->vitesse)/CASE;
				coinArriere[1].x = (joueur->hitbox.x + joueur->hitbox.w)/CASE;
				coinArriere[1].y = (joueur->hitbox.y+joueur->hitbox.h-joueur->vitesse)/CASE;
			}

			
			break;
			
		case BAS:
		
			if(joueur->hitbox.y + joueur->hitbox.h + joueur->vitesse <= NB_CASES * CASE) // Si il ne cherche pas  sortir de la map
			{
				coinAvantJoueur[0] = joueur->coin[3];
				coinAvantJoueur[1] = joueur->coin[2];
				coinArriereJoueur[0] = joueur->coin[0];
				coinArriereJoueur[1] = joueur->coin[1];
				
				coinAvant[0].x = (joueur->hitbox.x)/CASE;
				coinAvant[0].y = (joueur->hitbox.y+joueur->hitbox.h+joueur->vitesse)/CASE;
				coinAvant[1].x = (joueur->hitbox.x + joueur->hitbox.w)/CASE;
				coinAvant[1].y = (joueur->hitbox.y+joueur->hitbox.h+joueur->vitesse)/CASE;
				
				coinArriere[0].x = (joueur->hitbox.x)/CASE;
				coinArriere[0].y = (joueur->hitbox.y+joueur->vitesse)/CASE;
				coinArriere[1].x = (joueur->hitbox.x + joueur->hitbox.w)/CASE;
				coinArriere[1].y = (joueur->hitbox.y+joueur->vitesse)/CASE;
			}

			break;
			
		case GAUCHE:
		
			if(joueur->hitbox.x >= joueur->vitesse) // Si il ne cherche pas  sortir de la map
			{
				coinAvantJoueur[0] = joueur->coin[0];
				coinAvantJoueur[1] = joueur->coin[3];
				coinArriereJoueur[0] = joueur->coin[1];
				coinArriereJoueur[1] = joueur->coin[2];
				
				coinAvant[0].x = (joueur->hitbox.x-joueur->vitesse)/CASE;
				coinAvant[0].y = (joueur->hitbox.y)/CASE;
				coinAvant[1].x = (joueur->hitbox.x-joueur->vitesse)/CASE;
				coinAvant[1].y = (joueur->hitbox.y+joueur->hitbox.h)/CASE;
				
				coinArriere[0].x = (joueur->hitbox.x+joueur->hitbox.w-joueur->vitesse)/CASE;
				coinArriere[0].y = (joueur->hitbox.y)/CASE;
				coinArriere[1].x = (joueur->hitbox.x+joueur->hitbox.w-joueur->vitesse)/CASE;
				coinArriere[1].y = (joueur->hitbox.y+joueur->hitbox.h)/CASE;
			}

			break;
			
		case DROITE:
		
			if(joueur->hitbox.x + joueur->hitbox.w + joueur->vitesse <= NB_CASES * CASE) // Si il ne cherche pas  sortir de la map
			{
				coinAvantJoueur[0] = joueur->coin[1];
				coinAvantJoueur[1] = joueur->coin[2];
				coinArriereJoueur[0] = joueur->coin[0];
				coinArriereJoueur[1] = joueur->coin[3];
				
				coinAvant[0].x = (joueur->hitbox.x+joueur->hitbox.w+joueur->vitesse)/CASE;
				coinAvant[0].y = (joueur->hitbox.y)/CASE;
				coinAvant[1].x = (joueur->hitbox.x+joueur->hitbox.w+joueur->vitesse)/CASE;
				coinAvant[1].y = (joueur->hitbox.y+joueur->hitbox.h)/CASE;
				
				coinArriere[0].x = (joueur->hitbox.x+joueur->vitesse)/CASE;
				coinArriere[0].y = (joueur->hitbox.y)/CASE;
				coinArriere[1].x = (joueur->hitbox.x+joueur->vitesse)/CASE;
				coinArriere[1].y = (joueur->hitbox.y+joueur->hitbox.h)/CASE;
			}

			break;
	}
	
	// Vérification des collisions
	
	if(carte[coinAvant[0].y][coinAvant[0].x] == MUR || carte[coinAvant[0].y][coinAvant[0].x] == BRIQUE ||
		carte[coinAvant[1].y][coinAvant[1].x] == MUR || carte[coinAvant[1].y][coinAvant[1].x] == BRIQUE ||
		(joueur->assisSurBombe == 0 && (carte[coinAvant[0].y][coinAvant[0].x] == BOMBE || carte[coinAvant[1].y][coinAvant[1].x] == BOMBE)))
	{
		return 1; // Le joueur ne peut pas avancer
	}
	else if(joueur->assisSurBombe == 1)
	{
		if(carte[coinAvant[0].y][coinAvant[0].x] == VIDE
		    && carte[coinAvant[1].y][coinAvant[1].x] == VIDE && carte[coinArriere[0].y][coinArriere[0].x] == VIDE
		    && carte[coinArriere[1].y][coinArriere[1].x] == VIDE) // Si il n'est plus assis sur sa bombe
		{
			joueur->assisSurBombe = 0;
		}
		else if(carte[coinAvant[0].y][coinAvant[0].x] == BOMBE || carte[coinAvant[1].y][coinAvant[1].x] == BOMBE)
				// Si il est sur une bombe mais il cherche à avancer sur une autre bombe située à côté
		{
			if(coinAvant[0].x != coinAvantJoueur[0].x || coinAvant[0].y != coinAvantJoueur[0].y)
				// Si un des 2 coins avants n'est pas sur la même case que les coins avant initiaux du joueur,
				// c'est qu'il cherche à marcher sur une autre bombe
			{
				return 1;
			}
		}
		
	} 
	
	return VIDE;
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



/* bool = 1 : le joueur est mort cheh
 * bool = 2 : le joueur a chopé un item
 * bool = 0 : rien ne se passe
 */
Entite verifierCollision(Position coin[], int carte[][NB_CASES])
{
	Entite entite;
	int i=0;
	
	entite.bool = 0;
	
	for(i=0; i<4; i++)
	{
		if(carte[coin[i].y][coin[i].x] == FLAMME)
		{
			entite.bool = 1;
			entite.position.x = coin[i].x;
			entite.position.y = coin[i].y;
		}
		else if( (carte[coin[i].y][coin[i].x] == ITEM_BOMBE || carte[coin[i].y][coin[i].x] == ITEM_FLAMME ||
				carte[coin[i].y][coin[i].x] == ITEM_ROLLER || carte[coin[i].y][coin[i].x] == ITEM_ROLLER) && !entite.bool)
		{
			entite.bool = 2;
			entite.position.x = coin[i].x;
			entite.position.y = coin[i].y;
		}
		else if(carte[coin[i].y][coin[i].x] == BOMBE) // cette condition est utilisée dans poserBombe()
		{
			entite.bool = 3;
		}
	}
	
	return entite;
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



int* verifierTouches(Perso *idJoueur[], ListePerso *joueurs, Uint8 *touche, SDL_Surface *perso[])
{
	int i = 0, *bombeAPoser = malloc(joueurs->taille*sizeof(int));
	Perso *courant = joueurs->premier;
	
	verifierErreurAllocation(bombeAPoser);
	
	for(i=0; i<joueurs->taille; i++)
	{
		bombeAPoser[i] = 0;
	}
	
	
	// Joueur 1
	
	if(idJoueur[0] != NULL)
	{
		idJoueur[0]->touche[HAUT] = touche[J1_HAUT];
		idJoueur[0]->touche[BAS] = touche[J1_BAS];
		idJoueur[0]->touche[GAUCHE] = touche[J1_GAUCHE];
		idJoueur[0]->touche[DROITE] = touche[J1_DROITE];
		
		if(touche[J1_POSER_BOMBE] && !idJoueur[0]->bombePosee && !idJoueur[0]->assisSurBombe)
		{
			bombeAPoser[0] = 1;
		}
		idJoueur[0]->bombePosee = touche[J1_POSER_BOMBE];
	}
	
	// Joueur 2
	
	if(idJoueur[1] != NULL)
	{
		idJoueur[1]->touche[HAUT] = touche[J2_HAUT];
		idJoueur[1]->touche[BAS] = touche[J2_BAS];
		idJoueur[1]->touche[GAUCHE] = touche[J2_GAUCHE];
		idJoueur[1]->touche[DROITE] = touche[J2_DROITE];
		
		if(touche[J2_POSER_BOMBE] && !idJoueur[1]->bombePosee && !idJoueur[1]->assisSurBombe)
		{
			bombeAPoser[1] = 1;
		}
		idJoueur[1]->bombePosee = touche[J2_POSER_BOMBE];
	}
	
	// Joueur 3
	
	if(idJoueur[2] != NULL)
	{
		idJoueur[2]->touche[HAUT] = touche[J3_HAUT];
		idJoueur[2]->touche[BAS] = touche[J3_BAS];
		idJoueur[2]->touche[GAUCHE] = touche[J3_GAUCHE];
		idJoueur[2]->touche[DROITE] = touche[J3_DROITE];
		
		if(touche[J3_POSER_BOMBE] && !idJoueur[2]->bombePosee && !idJoueur[2]->assisSurBombe)
		{
			bombeAPoser[2] = 1;
		}
		idJoueur[2]->bombePosee = touche[J3_POSER_BOMBE];
	}
	
	// Joueur 4
	
	if(idJoueur[3] != NULL)
	{
		idJoueur[3]->touche[HAUT] = touche[J4_HAUT];
		idJoueur[3]->touche[BAS] = touche[J4_BAS];
		idJoueur[3]->touche[GAUCHE] = touche[J4_GAUCHE];
		idJoueur[3]->touche[DROITE] = touche[J4_DROITE];
		
		if(touche[J4_POSER_BOMBE] && !idJoueur[3]->bombePosee && !idJoueur[3]->assisSurBombe)
		{
			bombeAPoser[3] = 1;
		}
		idJoueur[3]->bombePosee = touche[J4_POSER_BOMBE];
	}
	
	
	// Orientation des joueurs
	
	while(courant != NULL)
	{
		for(i=0; i<4; i++)
		{
			if(courant->touche[i])
			{
				courant->persoActuel = perso[i];
			}
		}
		
		courant = courant->suivant;
	}
	
	return bombeAPoser;
}
