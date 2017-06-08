#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include "menu.h"


int menujeu(SDL_Surface *ecran)
{
  SDL_Surface *deux = SDL_LoadBMP("images/2J.bmp"), *trois = SDL_LoadBMP("images/3J.bmp"), *quatre = SDL_LoadBMP("images/4J.bmp"),
			  *deuxSelect = SDL_LoadBMP("images/2J_SELECTION.bmp"), *troisSelect = SDL_LoadBMP("images/3J_SELECTION.bmp"), *quatreSelect = SDL_LoadBMP("images/4J_SELECTION.bmp"),
			  *quitter = SDL_LoadBMP("images/QUIT.bmp"), *quitterSelect = SDL_LoadBMP("images/QUIT_SELECTION.bmp"), *menu = SDL_LoadBMP("images/menu.bmp"), *selection = SDL_LoadBMP("images/2J.bmp");
  SDL_Rect positionMenu, positionDeux, positionTrois, positionQuatre, positionQuit, positionSelect;
  SDL_Event event;
  Uint8 *touche = SDL_GetKeyState(NULL); // Tableau de booléens qui représente chaque touche et indique si la touche est appuyée
  int choix = 2, continuer = 1;


  positionMenu.x = 0;
  positionMenu.y = 0;

  positionDeux.x = (menu->w-deux->w)/2;
  positionDeux.y = 200;
  
  positionTrois.x = positionDeux.x;
  positionTrois.y = positionDeux.y + deux->h + 20;
  
  positionQuatre.x = positionDeux.x;
  positionQuatre.y = positionTrois.y + trois->h + 20;
  
  positionQuit.x = positionDeux.x;
  positionQuit.y = positionQuatre.y + quatre->h + 20;


  // Choix du mode

  while(continuer)
  {
    SDL_WaitEvent(&event);
    
    
	if(touche[SDLK_ESCAPE] || event.type == SDL_QUIT)
	{
		continuer = 0;
		choix = 5;
	}
	else if(touche[SDLK_DOWN])
	{
		choix++;
	}
	else if(touche[SDLK_UP])
	{
		choix--;
	}
	else if(touche[SDLK_SPACE])
	{
		continuer = 0;
	}
	
	
	if(choix > 5)
	{
		choix = 2;
	}
	else if(choix < 2)
	{
		choix = 5;
	}
	
	
	switch(choix)
	{
		case 2:
			selection = deuxSelect;
			positionSelect = positionDeux;
			break;
		case 3:
			selection = troisSelect;
			positionSelect = positionTrois;
			break;
		case 4: 
			selection = quatreSelect;
			positionSelect = positionQuatre;
			break;
		case 5:
			selection = quitterSelect;
			positionSelect = positionQuit;
			break;
	}


  
	// Collage des surfaces et màj de l'écran

	SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255, 255, 255)); // Fond de la fenêtre : blanc
	SDL_BlitSurface(menu, NULL, ecran, &positionMenu);
	SDL_BlitSurface(deux, NULL, ecran, &positionDeux);
	SDL_BlitSurface(trois, NULL, ecran, &positionTrois);
	SDL_BlitSurface(quatre, NULL, ecran, &positionQuatre);
	SDL_BlitSurface(quitter, NULL, ecran, &positionQuit);
	SDL_BlitSurface(selection, NULL, ecran, &positionSelect);
	
	SDL_Flip(ecran);
  }
  
  // Libération des surfaces
  
  SDL_FreeSurface(menu);
  SDL_FreeSurface(deux);
  SDL_FreeSurface(trois);
  SDL_FreeSurface(quatre);
  SDL_FreeSurface(quitter);
  
  return choix;
}




  

