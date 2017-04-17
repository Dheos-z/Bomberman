#ifndef FCT_LISTES_CHAINEES_H
#define FCT_LISTES_CHAINEES_H


typedef struct Maillon Maillon;
struct Maillon
{
	int instant;
	Position position; // Position en x et y du maillon sur la carte
	int portee[4]; /* Portée de la bombe (lorsque le maillon est une bombe) dans
						les 4 directions (à définir au moment où elle explose) */
	Entite brique;
	Maillon *suivant;
};

typedef struct Liste Liste;
struct Liste
{
    int taille;
	Maillon *premier;
};

Liste* initialiserListe();
void afficherListe(Liste *liste);
void ajouterBombeFin(Liste *liste, int instantBombe, Position posBombe);
void ajouterMaillonDebut(Liste *liste, int nombre);
void ajouterMaillonMilieu(Liste *liste, int nombre, int indice);
int supprimerBombe(Liste *liste, int rang);
Maillon *recupererElement(Liste *liste, int rang);

#endif
