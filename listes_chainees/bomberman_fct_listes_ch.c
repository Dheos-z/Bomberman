
/* Dans le cadre de notre jeu, les fonctions qu'on utilise sont
 * initialiserListe(), afficherListe(), ajouterElementFin() et supprimerElement().
 * On a modifié les fonctions de base pour qu'elles correspondent bien à ce
 * qu'on veut faire dans le jeu.
 */

#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include "../constante.h"
#include "../jeu.h" // bomberman_fct_listes_ch.h déjà inclus dans jeu.h donc pas besoin de l'inclure ici


// Crée une liste chainée vide
Liste* initialiserListe()
{
	Liste* liste = malloc(sizeof(Liste));

	verifierErreurAllocation((void*)liste);

	liste->premier = NULL;
	liste->taille = 0;
	liste->nbItemsSurTerrain = 0;

	return liste;
}


// Affiche les instants auxquels chaque bombe a été posée, ainsi que les positions
void afficherListe(Liste *liste)
{
	Maillon *courant = liste->premier;
	
	printf("Taille de la liste : %d\n", liste->taille);

	while(courant != NULL)
	{
		printf("\nInstant : %d\nPositions : (%d, %d)\n", courant->instant, courant->position.x, courant->position.y);

		courant = courant->suivant; // On parcoure la liste
	}
	printf("\n");

	return;
}


// Ajouter une bombe à la fin de la liste
Maillon *ajouterElementFin(Liste *liste, int instantBombe, Position posBombe, int puissanceBombe, Perso *joueur)
{
	Maillon *courant = liste->premier, *dernier = NULL, *nouveau = malloc(sizeof(Maillon));
	int i = 0;
	
	verifierErreurAllocation((void*)nouveau);

	if(!liste->taille) // Si la liste est vide
	{
		liste->premier = nouveau;
	}
	else // Si elle contient au moins 1 élément on peut parcourir les éléments
	{
		while(courant != NULL)
		{
			dernier = courant;
			courant = courant->suivant;
		}
		// A ce stade, dernier est le dernier élément. Il pointe vers NULL. On peut accéder aux données du dernier élément
		dernier->suivant = nouveau;
	}
	
	nouveau->instant = instantBombe;
	nouveau->position.x = posBombe.x;
	nouveau->position.y = posBombe.y;
	nouveau->puissance = puissanceBombe;
	if(joueur != NULL)
	{
		nouveau->joueur = joueur;
	}
	
	nouveau->suivant = NULL;
	liste->taille++;
	
	// Initialisations
	for(i=0; i<4; i++) 
	{
		nouveau->entite[i].bool = 0;
		nouveau->entite[i].position.x = 0;
		nouveau->entite[i].position.y = 0;
		nouveau->portee[i] = 0;
	}

	return nouveau;
}


// Ajoute un maillon au début
void ajouterMaillonDebut(Liste *liste, int nombre)
{
    Maillon *nouveau = malloc(sizeof(Maillon));
    
    verifierErreurAllocation((void*)nouveau);

    nouveau->suivant = liste->premier;
    nouveau->instant = nombre;
    liste->premier = nouveau;
    liste->taille++;

    return;
}


// Ajoute un maillon en milieu de liste (l'indice commence à 0)
void ajouterMaillonMilieu(Liste *liste, int nombre, int indice)
{
    int i = 0;
    Maillon *precedentFinal = liste->premier, *nouveau = malloc(sizeof(Maillon)), *suivantFinal = NULL;
    
    verifierErreurAllocation((void*)nouveau);

    if(liste->taille < 2 || indice >= liste->taille || indice < 1)
    {
		printf("Erreur : taille de la liste trop petite (< 2) ou bien l'indice demandé est trop grand ou négatif\n");
    }
    else
    {
        for(i = 0; i < indice-1; i++)
        {
            precedentFinal = precedentFinal->suivant;
        }
        // Ici, precedentFinal vaut la case qui précèdera le nouvel élément

        suivantFinal = precedentFinal->suivant;
        nouveau->suivant = suivantFinal;
        nouveau->instant = nombre;
        precedentFinal->suivant = nouveau;
        
        liste->taille++;
    }
    
    return;
}


int supprimerElement(Liste *liste, int rang)
{
	int i = 0;
	Maillon *precedentFinal = liste->premier, *suivantFinal = NULL, *maillonAsupprimer = NULL;
	
	if(liste->taille < 1 || rang < 0 || rang >= liste->taille)
	{
		printf("Erreur de saisie du rang, ou bien la liste est vide\n");
	}
	else
	{
		if(rang)
		{
			for(i=0; i<rang-1; i++)
			{
				precedentFinal = precedentFinal->suivant;
			}
			// Ici, precedentFinal vaut la case qui précèdera le nouvel élément
			
			maillonAsupprimer = precedentFinal->suivant;
			suivantFinal = maillonAsupprimer->suivant;
			
			precedentFinal->suivant = suivantFinal;
			free(maillonAsupprimer);
		}
		else // Si on veut supprimer le maillon au rang 0
		{
			maillonAsupprimer = precedentFinal;
			liste->premier = precedentFinal->suivant;
			free(maillonAsupprimer);
		}
		liste->taille--;
		
		return 0;
	}
	
	return 1; // Si il y a eu une erreur
}


Maillon *recupererElement(Liste *liste, int rang)
{
	int i = 0;
	Maillon *courant = liste->premier;
	
	if(courant == NULL || rang < 0 || rang >= liste->taille)
	{
		printf("Erreur : la liste ne contient élément ou le rang est négatif/superieur a la taille de la liste\n");
	}
	else
	{
		for(i=0; i<rang; i++)
		{
			courant = courant->suivant;
		}
	}
	
	return courant;
}


void deplacerElement(Liste *liste, int rangDepart, int rangArrivee)
{
	int i=0, rangSup = rangDepart;
	Maillon *courant=liste->premier, *m = NULL, *mPlus1 = NULL, *n = NULL, *nPlus1 = NULL;
	
	
	if(rangDepart < 0 || rangArrivee < 0 || rangDepart >= liste->taille || rangArrivee >= liste->taille || liste->taille < 2 || rangDepart == rangArrivee)
	{
		printf("Erreur : operation impossible\n");
		return;
	}
	
	// Définition du plus grand rang
	if(rangDepart < rangArrivee)
	{
		rangSup = rangArrivee;
	}
	
	// Mémorisation des éléments indispensables
	
	while(i <= rangSup)
	{
		if(!rangArrivee && i == rangArrivee)
		{
			m = liste->premier;
			
			// Cas particulier où rangArr = 0 et rangDep = 1
			if(i == rangDepart-1)
			{
				n = courant->suivant;
				nPlus1 = courant->suivant->suivant;
			}
		}
		else if(i == rangArrivee-1 && i != rangDepart)
		{
			m = courant->suivant;
		}
		
		else if(i == rangArrivee && rangDepart < rangArrivee)
		{
			mPlus1 = courant->suivant;
		}
		
		else if(!rangDepart && i == rangDepart)
		{
			n = courant;
			nPlus1 = courant->suivant;
		}
		
		else if(rangDepart && i == rangDepart-1)
		{
			n = courant->suivant;
			nPlus1 = courant->suivant->suivant;
		}
		
		i++;
		courant = courant->suivant;
	}
	
	// Parcours et modification de l'ordre de la liste
		
	i=0;
	courant=liste->premier;
		
	while(i <= rangSup)
	{
		if(rangArrivee && i == rangArrivee-1 && rangDepart) // rangArrivee doit être non nul
		{
			courant->suivant = n;
			if(rangDepart > rangArrivee)
			{
				courant->suivant->suivant = m;
				rangDepart++;
			}
			else if(rangDepart < rangArrivee)
			{
				courant->suivant->suivant = mPlus1;
			}
		}
		
		else if(rangDepart && i == rangDepart-1 && i != rangArrivee) // rangDepart doit être non nul
		{
			courant->suivant = nPlus1;
		}
		
		// Si rangArrivee ou rangDepart est nul : petites manips
		else if(!rangDepart && i == rangDepart)
		{
			liste->premier = nPlus1;
			courant = liste->premier; // Actualisation du premier
			if(i == rangArrivee-1) // Cas exceptionnel où rangDep = 0 ET rangArr = 1
			{
				courant->suivant = n;
				courant->suivant->suivant = mPlus1;
			}
		}
		
		else if(!rangArrivee && i == rangArrivee)
		{
			liste->premier = n;
			liste->premier->suivant = m;
			courant = liste->premier; // Actualisation du premier
			rangDepart++;
		}
		
		i++;
		courant = courant->suivant;
	}
	
	return;
}


// Recherche un élément dans la liste à partir de ses positions
Maillon *chercherElement(Position pos, Liste *liste, int *rang)
{
	Maillon *courant = NULL;
	int i = 0;
	
	if(liste->taille)
	{
		courant = liste->premier;
	
		while(i < liste->taille && (courant->position.x != pos.x || courant->position.y != pos.y))
		{
			courant = courant->suivant;
			i++;
		}
	}
	
	if(rang != NULL)
	{
		*rang = i;
	}

	return courant;
}




// FONCTIONS LISTES CHAINEES DE PERSONNAGES


// Crée une liste chainée vide
ListePerso* initialiserListePerso()
{
	ListePerso* liste = malloc(sizeof(ListePerso));
	
	verifierErreurAllocation((void*)liste);

	if(liste == NULL) // Si les allocations n'ont pas abouti
	{
		exit(EXIT_FAILURE);
	}

	liste->premier = NULL;
	liste->taille = 0;

	return liste;
}


// Ajouter un perso à la fin de la liste
Perso *ajouterPersoFin(ListePerso *liste)
{
	Perso *courant = liste->premier, *dernier = NULL, *nouveau = malloc(sizeof(Perso));
	
	verifierErreurAllocation((void*)nouveau);

	if(!liste->taille) // Si la liste est vide
	{
		liste->premier = nouveau;
	}
	else // Si elle contient au moins 1 élément on peut parcourir les éléments
	{
		while(courant != NULL)
		{
			dernier = courant;
			courant = courant->suivant;
		}
		// A ce stade, dernier est le dernier élément. Il pointe vers NULL. On peut accéder aux données du dernier élément
		dernier->suivant = nouveau;
	}
	
	nouveau->suivant = NULL;
	liste->taille++;

	return nouveau;
}



void supprimerPerso(ListePerso *liste, Perso *joueur)
{
	Perso *courant = liste->premier, *joueurASupp = NULL;
	int bool = 0;
	
	
	if(courant == joueur) // Si le joueur à supprimer est le 1er de la liste
	{
		joueurASupp = courant;
		liste->premier = courant->suivant;
	}
	else // Si le joueur à supprimer n'est pas le 1er de la liste
	{
		while(courant != NULL && bool != 1)
		{
			if(courant->suivant == joueur)
			{
				bool = 1;
				joueurASupp = courant->suivant;
				courant->suivant = courant->suivant->suivant;
			}
			
			if(!bool)
			{
				courant = courant->suivant;
			}
		}	
	}
	
	
	free(joueurASupp);
	liste->taille--;
	
	return;
}


// Affiche la liste des prénoms de chaque perso
void afficherListePerso(ListePerso *liste)
{
	Perso *courant = liste->premier;
	
	printf("Taille de la liste : %d\n", liste->taille);

	while(courant != NULL)
	{
		printf("Joueur %d\n", courant->numero);
		courant = courant->suivant; // On parcoure la liste
	}
	printf("\n");

	return;
}


void verifierErreurAllocation(void *pointeur)
{
	if(pointeur == NULL)
	{
		exit(EXIT_FAILURE);
	}
	
	return;
}

