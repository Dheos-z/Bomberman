#include <stdio.h>
#include <stdlib.h>
#include "constante.h"
#include <SDL/SDL.h>
#include "jeu.h"
#include "listes_chainees/bomberman_fct_listes_ch.h"
#include <time.h>
#include "initialisations.h"
#include "fonctions_verifier.h"
#include "fonctions_jeu.h"



int jouerPartie(SDL_Surface* ecran, int nbJoueurs)
{
	SDL_Event event;
	SDL_Surface *mur = NULL, *brique = NULL, *bombe = NULL, *flamme = NULL, *itemBombe = NULL, *itemFlamme = NULL,
				*itemPied = NULL, *itemRoller = NULL, *perso[4] = {NULL};
	Perso *joueur = NULL, *idJoueur[4] = {NULL}, *joueurASupp = NULL; 
		// joueur va parcourir la liste, idJoueur contient l'adresse de chaque joueur (indispensable quand on teste l'appui des touches)
	int carte[NB_CASES][NB_CASES] = {0}, continuer = 1, i = 0, direction = 0, *bombeAPoser = NULL, retourMenu = 0;
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
	
	if(nbJoueurs > 2)
	{
		idJoueur[2] = ajouterPersoFin(joueurs);
		initJoueur(idJoueur[2], perso[BAS], 13*CASE, 13*CASE, 3);	
	}

	if(nbJoueurs > 3)
	{
		idJoueur[3] = ajouterPersoFin(joueurs);
		initJoueur(idJoueur[3], perso[BAS], 13*CASE, CASE, 4);
	}

	
	
		// Lecture du niveau
		
	lireFichier(carte);
	
	
		// Génération d'items
		
	genererItems(items, carte);
	
	
	
	
	// BOUCLE DES EVENEMENTS
	
	while(continuer)
	{
		SDL_PollEvent(&event); // Activation des événements en mode continu
		
		
		// GESTION DES TOUCHES
		
		if(event.type == SDL_QUIT) // Fin du programme
		{
			continuer = 0;
		}
		else if(touche[SDLK_ESCAPE]) // Retour au menu
		{
			continuer = 0;
			retourMenu = 1;
		}
			
		
		bombeAPoser = (int*)verifierTouches(idJoueur, joueurs, touche, perso); // Renvoie le tableau qui indique si chaque joueur essaye de poser une bombe
		
		
		
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
		
		if(bombesPosees->taille && verifierDelai(bombesPosees->premier->instant, DELAI_BOMBE)) // Si le délai d'attente de la première bombe de la liste est dépassé
		{
			exploserBombe(carte, bombesPosees->premier, bombesPosees, bombesExplosees);
		}
		
		// Vérification du délai d'affichage des flammes, cassage des briques
		
		if(bombesExplosees->taille && verifierDelai(bombesExplosees->premier->instant, DELAI_BOMBE+DELAI_FLAMMES)) // Si le délai d'explosion de la première bombe de la liste est dépassé
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
					idJoueur[joueurASupp->numero-1] = NULL; // idJoueur ne pointe plus vers le joueur mort
					supprimerPerso(joueurs, joueurASupp);
				}
				else if(entite.bool == 2) // Si le joueur a chopé un item
				{
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
			retourMenu = 1;
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
	
	
	return retourMenu;
}






