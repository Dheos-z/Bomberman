#include <stdio.h>
#include <stdlib.h>
#include "constante.h"
#include <SDL/SDL.h>
#include "fonction.h"
#include "jeu.h"




int jouerPartie(SDL_Surface* ecran)
{
	SDL_Event event;
	SDL_Surface *mur = NULL, *brique = NULL, *bombe = NULL;
	SDL_Surface *perso[4] = {NULL}, *persoActuel = NULL;
	int carte[NB_CASES][NB_CASES] = {0}, continuer = 1, i = 0;
	
	// Initialisation des surfaces
	
	mur = SDL_LoadBMP("MUR.bmp");
	brique = SDL_LoadBMP("BRIQUE.bmp");
	bombe = SDL_LoadBMP("BOMBE.bmp");
	
	perso[BAS] = SDL_LoadBMP("BAS.bmp");
	perso[HAUT] = SDL_LoadBMP("HAUT.bmp");
	perso[GAUCHE] = SDL_LoadBMP("GAUCHE.bmp");
	perso[DROITE] = SDL_LoadBMP("DROITE.bmp");
	persoActuel = perso[BAS]; // On initialise le perso vers le bas
	
	// Lecture du niveau
	
	lireFichier(carte);
	
	
	// Boucle des événements
	
	while(continuer)
	{
		SDL_PollEvent(&event);
		
		switch(event.type)
		{
			case SDL_QUIT: // Si on appuie sur la croix
				continuer = 0;
				break;
		}
		
		// Mise à jour de l'écran
		
		SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255, 255, 255)); // Fond de la fenêtre : blanc
		blitterSurfaces(ecran, carte, mur, brique, bombe, persoActuel);
		SDL_Flip(ecran);
	}
	
	// Libération des surfaces
	
	SDL_FreeSurface(mur);
	SDL_FreeSurface(brique);
	SDL_FreeSurface(bombe);
	for(i = 0; i < 4; i++)
	{
		SDL_FreeSurface(perso[i]);
		// persoActuel est libéré dans cette boucle car il est égal à une des 4 surfaces libérées ici
	}
	
	
	return 0; // Pour l'instant
}





int lireFichier(int carte[][NB_CASES])
{
	FILE *fichier = NULL;
	int i = 0, j = 0;
	
	fichier = fopen("carte.txt", "r"); // Ouverture du fichier représentant la carte en mode lecture seule
	
	if(fichier == NULL)
	{
		return 1; // Erreur lors de l'ouverture du fichier		
	}
	
	for(i = 0; i < NB_CASES; i++)
	{
		for(j = 0; j < NB_CASES; j++)
		{
			carte[i][j] = fgetc(fichier) - '0'; /* Remplissage des valeurs de la carte. 
												On retire '0' pour passer du char (lu dans le fichier) au int */
		}
		fgetc(fichier); // Pour lire le \n à la fin de chaque ligne
	}
	
	fclose(fichier); // La base
	
	return 0;
}


void blitterSurfaces(SDL_Surface *ecran, int carte[][NB_CASES], SDL_Surface *mur, 
					SDL_Surface *brique, SDL_Surface *bombe, SDL_Surface *persoActuel)
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
					
				case PERSO:
					SDL_BlitSurface(persoActuel, NULL, ecran, &position);
					break;
			}
		}
	}
	
	return;
}





/*

// NOMBRE DE MUR DANS LA MAP

int nombreMur(int map[NB_CASES][NB_CASES])
{
  int i,j;
  int cmpMur = 0;

  for(i = 0; i < NB_CASES; i++)
  {
    for(j = 0; j<NB_CASES; j++)
    {
      if(map[i][j] == MUR)
      {
        cmpMur++;
      }
    }
  }
    return cmpMur;
}

// NOMBRE DE BRIQUE DANS LA MAP

int nombreBrique(int map[NB_CASES][NB_CASES])
{
  int i,j;
  int cmpBrique = 0;

  for(i = 0; i < NB_CASES; i++)
  {
    for(j = 0; j < NB_CASES; j++)
    {
      if(map[i][j] == BRIQUE)
      {
        cmpBrique++;
      }
    }
  }

    return cmpBrique;
}





int main(void)
{
  SDL_Surface *ecran = NULL;
  SDL_Event event;
  SDL_Surface *mur = NULL;
  SDL_Surface *brique = NULL;
  SDL_Surface *bombe = NULL;
  SDL_Surface *vide = NULL;

   // perso
  SDL_Surface *bomberHaut = NULL;
  SDL_Surface *bomberBas = NULL;
  SDL_Surface *bomberGauche = NULL;
  SDL_Surface *bomberDroite = NULL;

  SDL_Rect *positionMur = NULL;
  SDL_Rect *positionBrique = NULL;
  SDL_Rect *positionVide = NULL;
  SDL_Rect bomber;
  SDL_Rect positionBombe;



  int map[NB_CASES][NB_CASES];
  int continuer = 1;
  int i,j,k;
  int direction = BAS;
  int compteurM, compteurB;
  int nbMur = 0;
  int nbBrique = 0;
  int nbVide = 0;
  int compteurV = 0;
  

  remplissage(map);
  compteurM = nombreMur(map);
  compteurB = nombreBrique(map);
  compteurV = NB_CASES * NB_CASES - (compteurM + compteurB);

  SDL_Init(SDL_INIT_VIDEO);
  ecran = SDL_SetVideoMode(675, 675, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
  SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 150, 0));
  SDL_WM_SetCaption("Super Bombermus", NULL);

  // ON DONNE LEURS VALEURS AUX SURFACES

  mur = SDL_LoadBMP("MUR.bmp");
  positionMur = (SDL_Rect*) malloc(compteurM * sizeof(SDL_Rect));

  brique = SDL_LoadBMP("BRIQUE.bmp");
  positionBrique = (SDL_Rect*) malloc(compteurB * sizeof(SDL_Rect));

  vide = SDL_LoadBMP("VIDE.bmp");
  positionVide = (SDL_Rect*) malloc(compteurV * sizeof(SDL_Rect));
  
  bomberBas = SDL_LoadBMP("BAS.bmp");
  bomberHaut = SDL_LoadBMP("HAUT.bmp");
  bomberGauche = SDL_LoadBMP("GAUCHE.bmp");
  bomberDroite = SDL_LoadBMP("DROITE.bmp");
  bombe = SDL_LoadBMP("BOMBE.bmp");

  // INITIALISATION DES POSITIONS
  
  for(i = 0; i<NB_CASES; i++)
  {
    for(j = 0; j<NB_CASES; j++)
    {
      if(map[i][j] == MUR)
      {
        positionMur[nbMur].x = i*CASE;
        positionMur[nbMur].y = j*CASE;
        nbMur++;
      }
      else if(map[i][j] == BRIQUE)
      {
        positionBrique[nbBrique].x = i*CASE;
        positionBrique[nbBrique].y = j*CASE;
        nbBrique++;
      }
      else if(map[i][j] == VIDE)
      {
        positionVide[nbVide].x = i*CASE;
        positionVide[nbVide].y = j*CASE;
        nbVide++;
      }
    }
  }

  bomber.x = CASE;
  bomber.y = CASE;
  
  positionBombe.x = 676;
  positionBombe.y = 676;

  nbMur = 0;
  nbBrique = 0;



  SDL_EnableKeyRepeat(10,10);

  // ECRAN PAUSE

  while(continuer)
  {
    SDL_WaitEvent(&event);
    switch(event.type)
    {
      case SDL_QUIT:
      continuer = 0;
      break;

      // MOUVEMENT DU PERSO 1

      case SDL_KEYDOWN:
        switch(event.key.keysym.sym)
        {
          case SDLK_RIGHT:
          if(map[(bomber.x)/CASE+1][(bomber.y)/CASE] == VIDE)
           // && map[(bomber.x)/CASE+1][(bomber.y)/CASE] != BRIQUE
          {
            bomber.x+=CASE;
            direction = DROITE;
          }
          break;

          case SDLK_LEFT:
         if(map[(bomber.x)/CASE-1][(bomber.y)/CASE] == VIDE)
           // && map[(bomber.x)/CASE-1][(bomber.y)/CASE] != BRIQUE
         {
            bomber.x-=CASE;
            direction = GAUCHE;
         }

          break;

          case SDLK_UP:
          if(map[(bomber.x)/CASE][(bomber.y)/CASE-1] == VIDE)
            // && map[(bomber.x)/CASE][(bomber.y)/CASE-1] != BRIQUE

          {
            bomber.y-=CASE;
            direction = HAUT;
          }
          break;

          case SDLK_DOWN:
          if(map[(bomber.x)/CASE][(bomber.y)/CASE+1] == VIDE)
           // && map[(bomber.x)/CASE][(bomber.y)/CASE+1] != BRIQUE
          {
            bomber.y+=CASE;
            direction = BAS;
          }
          break;

          case SDLK_SPACE:
          map[(positionBombe.x)/CASE][(positionBombe.y)/CASE] = VIDE;
          positionBombe.x = bomber.x;
          positionBombe.y = bomber.y;
          map[(positionBombe.x)/CASE][(positionBombe.y)/CASE] = BOMBE;
          break;

        }
    }


    // BLIT
    
    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 150, 0)); 

    for(i = 0; i<compteurM; i++)
    {
      SDL_BlitSurface(mur, NULL, ecran, &positionMur[i]);
    }

    for(i = 0; i<compteurB; i++)
    {
      SDL_BlitSurface(brique, NULL, ecran, &positionBrique[i]);
    }

    for(i = 0; i<compteurV; i++)
    {
      SDL_BlitSurface(vide, NULL, ecran, &positionVide[i]);
    }
    

   if(positionBombe.x != 0 && positionBombe.y != 0)
   {
      SDL_SetColorKey(bombe, SDL_SRCCOLORKEY,
      SDL_MapRGB(bombe->format, 255, 255, 0));
      SDL_BlitSurface(bombe, NULL, ecran, &positionBombe);
   }


    for(i = 0; i<NB_CASES; i++)
    {
      for(j = 0; j<NB_CASES; j++)
      {
        switch(direction)
        {
          case HAUT:
          SDL_SetColorKey(bomberHaut, SDL_SRCCOLORKEY,
          SDL_MapRGB(bomberHaut->format, 255, 255, 0));
          SDL_BlitSurface(bomberHaut, NULL, ecran, &bomber);
          break;
          
          case BAS:
          SDL_SetColorKey(bomberBas, SDL_SRCCOLORKEY,
          SDL_MapRGB(bomberBas->format, 255, 255, 0));
          SDL_BlitSurface(bomberBas, NULL, ecran, &bomber);
          break;

          case DROITE:
          SDL_SetColorKey(bomberDroite, SDL_SRCCOLORKEY,
          SDL_MapRGB(bomberDroite->format, 255, 255, 0));
          SDL_BlitSurface(bomberDroite, NULL, ecran, &bomber);
          break;

          case GAUCHE:
          SDL_SetColorKey(bomberGauche, SDL_SRCCOLORKEY,
          SDL_MapRGB(bomberGauche->format, 255, 255, 0));
          SDL_BlitSurface(bomberGauche, NULL, ecran, &bomber);
          break;
        }
      }
    

    SDL_Flip(ecran);

    }
  }

  
  free(positionMur);
  free(positionBrique);
  SDL_FreeSurface(brique);
  SDL_FreeSurface(mur);
  SDL_FreeSurface(vide);
  SDL_Quit();

  return EXIT_SUCCESS;
}



*/
  
  
