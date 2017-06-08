#include <stdio.h>
#include <stdlib.h>
#include "constante.h"
#include "jeu.h"
#include "listes_chainees/bomberman_fct_listes_ch.h"
#include "fonctions_verifier.h"



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
		
		if(touche[J1_POSER_BOMBE] && !idJoueur[0]->bombePosee && !idJoueur[0]->assisSurBombe && idJoueur[0]->bombesRestantes)
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
		
		if(touche[J2_POSER_BOMBE] && !idJoueur[1]->bombePosee && !idJoueur[1]->assisSurBombe && idJoueur[1]->bombesRestantes)
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
		
		if(touche[J3_POSER_BOMBE] && !idJoueur[2]->bombePosee && !idJoueur[2]->assisSurBombe && idJoueur[2]->bombesRestantes)
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
		
		if(touche[J4_POSER_BOMBE] && !idJoueur[3]->bombePosee && !idJoueur[3]->assisSurBombe && idJoueur[3]->bombesRestantes)
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
