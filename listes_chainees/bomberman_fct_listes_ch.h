#ifndef FCT_LISTES_CHAINEES_H
#define FCT_LISTES_CHAINEES_H


typedef struct Maillon Maillon;
struct Maillon
{
	int instant; // Instant auquel la bombe a été posée (en millisecondes)
	Position position; // Position en x et y de la bombe sur la carte
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
void ajouterBombeFin(Liste *liste, int instantBombe, int posX, int posY);
void ajouterMaillonDebut(Liste *liste, int nombre);
void ajouterMaillonMilieu(Liste *liste, int nombre, int indice);
int supprimerBombe(Liste *liste, int rang);

#endif
