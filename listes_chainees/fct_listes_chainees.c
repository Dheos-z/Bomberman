#include <stdio.h>
#include <stdlib.h>
#include "fct_listes_chainees.h"

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


// Affiche la liste chaînée en entier
void afficherListe(Liste *liste)
{
	Maillon *courant = liste->premier;
	
	printf("Taille de la liste : %d\n", liste->taille);

	while(courant != NULL)
	{
		printf("%d / ", courant->donnee);

		courant = courant->suivant; // On parcoure la liste
	}
	printf("\n");

	return;
}


// Ajouter un maillon à la fin de la liste
void ajouterMaillonFin(Liste *liste, int nombre)
{
	Maillon *courant = liste->premier, *dernier = NULL, *nouveau = malloc(sizeof(Maillon));

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
	nouveau->donnee = nombre;
	nouveau->suivant = NULL;
	liste->taille++;

	return;
}


// Ajoute un maillon au début
void ajouterMaillonDebut(Liste *liste, int nombre)
{
    Maillon *nouveau = malloc(sizeof(Maillon));

    nouveau->suivant = liste->premier;
    nouveau->donnee = nombre;
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
        nouveau->donnee = nombre;
        precedentFinal->suivant = nouveau;
        
        liste->taille++;
    }
    
    return;
}


void supprimerMaillon(Liste *liste, int rang)
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
	}
	
	return;
}
