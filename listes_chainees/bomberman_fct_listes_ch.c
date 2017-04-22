
/* Dans le cadre de notre jeu, les fonctions qu'on utilise sont
 * initialiserListe(), afficherListe(), ajouterBombeFin() et supprimerBombe().
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

	if(liste == NULL) // Si les allocations n'ont pas abouti
	{
		exit(EXIT_FAILURE);
	}

	liste->premier = NULL;
	liste->taille = 0;

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
Maillon *ajouterBombeFin(Liste *liste, int instantBombe, Position posBombe, int puissanceBombe, Perso *joueur)
{
	Maillon *courant = liste->premier, *dernier = NULL, *nouveau = malloc(sizeof(Maillon));
	int i = 0;

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
	nouveau->joueur = joueur;
	
	nouveau->suivant = NULL;
	liste->taille++;
	
	// Initialisations
	for(i=0; i<4; i++) 
	{
		nouveau->brique[i].bool = 0;
		nouveau->brique[i].position.x = 0;
		nouveau->brique[i].position.y = 0;
		nouveau->portee[i] = 0;
	}

	return nouveau;
}


// Ajoute un maillon au début
void ajouterMaillonDebut(Liste *liste, int nombre)
{
    Maillon *nouveau = malloc(sizeof(Maillon));

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


int supprimerBombe(Liste *liste, int rang)
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
	
	return 1;
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
