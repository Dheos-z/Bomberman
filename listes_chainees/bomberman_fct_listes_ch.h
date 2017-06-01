#ifndef FCT_LISTES_CHAINEES_H
#define FCT_LISTES_CHAINEES_H


typedef struct Maillon Maillon;
struct Maillon
{
	int instant;
	Position position; // Position en x et y du maillon sur la carte
	int puissance; // Indispensable qu'elle ait sa propre puissance
	Perso *joueur; // Le joueur qui a posé cette bombe
	int portee[4]; /* Portée de la bombe (lorsque le maillon est une bombe) dans
						les 4 directions (à définir au moment où elle explose) */
	Entite entite[4]; // Pour chaque direction, indique si l'entité (brique ou item) peut se casser et à quelle position
	Maillon *suivant;
	int type; // Dans le cas où l'élément est un item : type de l'item
};

typedef struct Liste Liste;
struct Liste
{
    int taille;
	Maillon *premier;
	int nbItemsSurTerrain; // On l'utilise dans le cas d'une liste d'items
};


typedef struct ListePerso ListePerso;
struct ListePerso
{
    int taille;
	Perso *premier;
};

// Liste chaînée de bombes/items

Liste* initialiserListe();
void afficherListe(Liste *liste);
Maillon *ajouterElementFin(Liste *liste, int instantBombe, Position posBombe, int puissanceBombe, Perso *joueur);
void ajouterMaillonDebut(Liste *liste, int nombre);
void ajouterMaillonMilieu(Liste *liste, int nombre, int indice);
int supprimerElement(Liste *liste, int rang);
Maillon *recupererElement(Liste *liste, int rang);
void deplacerElement(Liste *liste, int rangDepart, int rangArrivee);
Maillon *chercherElement(Position posBombe, Liste *liste, int *rang);

// Liste chaînée de personnages

ListePerso* initialiserListePerso();
Perso *ajouterPersoFin(ListePerso *liste);

#endif
