#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include "menu.h"


void menujeu(void)
{
  SDL_Surface *un = NULL, *deux = NULL, *quitter = NULL, *ecran = NULL, *menu = NULL;
  SDL_Rect positionMenu, positionUn, positionDeux, positionQuit;
  SDL_Event event;
  int continuer = 1, choixmenu = 0;

  // LANCER LA SDL

  SDL_Init(SDL_INIT_VIDEO);

  positionMenu.x = 0;
  positionMenu.y = 0;

  positionUn.x = 200;
  positionUn.y = 225;

  positionDeux.x = 200;
  positionDeux.y = 300;
  
  positionQuit.x = 200;
  positionQuit.y = 375;

  //INITIALISATION DE LA FENETRE

  ecran = SDL_SetVideoMode(675, 675, 32, SDL_HWSURFACE);
  SDL_WM_SetCaption("Bomberman", NULL);

  menu = SDL_LoadBMP("menu.bmp");
  un = SDL_LoadBMP("1J_SELECTION.bmp");
  deux = SDL_LoadBMP("2J.bmp");
  quitter = SDL_LoadBMP("QUIT.bmp");

  // BOUCLE D'EVENEMENT 

  while(continuer)
  {
    SDL_WaitEvent(&event);

    switch (event.type)
    {
      case SDL_QUIT:
        continuer = 0;
        break;
      case SDL_KEYDOWN:
      switch(event.key.keysym.sym)
      {
        case SDLK_DOWN:
            choixmenu ++; 
            break;

        case SDLK_UP: 
            choixmenu--;
            break;
      }
    }
    
    
	// HISTOIRE DE SAVOIR QUEL RUBRIQUE DOIT ETRE ALLUMÉ

    if(choixmenu == 0)
    {
      un = SDL_LoadBMP("1J_SELECTION.bmp");
      deux = SDL_LoadBMP("2J.bmp");
      quitter = SDL_LoadBMP("QUIT.bmp");
    }
    else if(choixmenu == 1)
    {
      deux = SDL_LoadBMP("2J_SELECTION.bmp");
      un = SDL_LoadBMP("1J.bmp");
      quitter = SDL_LoadBMP("QUIT.bmp");
    }
    else if(choixmenu == 2)
    {
      quitter = SDL_LoadBMP("QUIT_SELECTION.bmp");
      un = SDL_LoadBMP("1J.bmp");
      deux = SDL_LoadBMP("2J.bmp");
    }

    if(choixmenu < 0)
    {
      choixmenu = 2;
    }
    else if(choixmenu > 2)
    {
      choixmenu = 0;
    }


  
	// BLITSURFACE

	SDL_BlitSurface(menu, NULL, ecran, &positionMenu);
	SDL_BlitSurface(un, NULL, ecran, &positionUn);
	SDL_BlitSurface(deux, NULL, ecran, &positionDeux);
	SDL_BlitSurface(quitter, NULL, ecran, &positionQuit);


	// MAJ DE L'ECRAN
	
	SDL_Flip(ecran);
  }
  
  return;
}




  

