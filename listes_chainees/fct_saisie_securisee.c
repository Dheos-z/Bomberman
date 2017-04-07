#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "fct_saisie_securisee.h"


// Lit une chaine contenant au maximum nbCaractMax caractères
void lireChaine(char *chaine, int nbCaractMax)
{
    char *posEntree = NULL;

    fgets(chaine, nbCaractMax, stdin); // Fonction de saisie sécurisée

    if(strchr(chaine, '\n') != NULL) // Si on trouve le caractère \n
    {
        posEntree = strchr(chaine, '\n'); // On se place dessus
        *posEntree = '\0'; // Et on le remplace par \0
    }
    else // Si l'utilisateur a tapé trop de caractères, il faut vider le buffer
    {
        viderBuffer();
    }

    return;
}


void viderBuffer()
{
    char car = 0;

    while(car != '\n' && car != EOF)
    {
        car = getchar(); // Lit l'entrée standard stdin jusqu'à qu'elle soit vide
    }

    return;
}


long lireNombre(char chaine[], int nbCharMax)
{
	lireChaine(chaine, nbCharMax);
	return strtol(chaine, NULL, 10);
}
