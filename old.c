#include<stdio.h>
#include<stdlib.h>
#include"constante.h"
#include<SDL/SDL.h>

void remplissage(int map[T_TAB][T_TAB]);
void affichage(int map[T_TAB][T_TAB]);

int main(void)
{
  SDL_Surface *mur= NULL, *ecran = NULL, *vide = NULL, *brique = NULL,
  *bomberHaut = NULL, *bomberBas = NULL, *bomberDroite = NULL,
  *bomberGauche = NULL;
  SDL_Event event;
  int map[T_TAB][T_TAB];
  SDL_Rect position[T_TAB][T_TAB];
  SDL_Rect bomber;
  int continuer = 1;
  int i,j;
  int direction = BAS;

  remplissage(map);
  affichage(map); 
  SDL_Init(SDL_INIT_VIDEO);

  ecran = SDL_SetVideoMode(675, 675, 32, SDL_HWSURFACE);
  SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 16, 88, 48));
  SDL_WM_SetCaption("Bomber-Man", NULL);
  
  brique = SDL_LoadBMP("BRIQUE.bmp");
  mur = SDL_LoadBMP("MUR.bmp");
  vide = SDL_LoadBMP("VIDE.bmp");
  bomberBas = SDL_LoadBMP("BAS.bmp");
  bomberHaut = SDL_LoadBMP("HAUT.bmp");
  bomberDroite = SDL_LoadBMP("DROITE.bmp");
  bomberGauche = SDL_LoadBMP("GAUCHE.bmp");

  for(i = 0; i<T_TAB; i++)
  {
    for(j = 0; j<T_TAB; j++)
    {

        position[i][j].x = i*CASE;
        position[i][j].y = j*CASE;  
    }
  }

  bomber.x = CASE;
  bomber.y = CASE;
  



// ECRAN PAUSE

SDL_EnableKeyRepeat(1,1);

while (continuer)
{

  SDL_WaitEvent(&event);
  switch(event.type)
  {
    case SDL_QUIT:
    continuer = 0;
    break;

    case SDL_KEYDOWN:
      switch(event.key.keysym.sym)
      {

        case SDLK_RIGHT:
        bomber.x++;
        direction = DROITE;
        break;

        case SDLK_LEFT:
        bomber.x--;
        direction = GAUCHE;
        break;

        case SDLK_UP:
        bomber.y--;
        direction = HAUT;
        break;

        case SDLK_DOWN:
        bomber.y++;
        direction = BAS;
      }


  }

  

  for(i = 0; i<T_TAB; i++)
  {
    for(j = 0; j<T_TAB; j++)
    {
      switch(direction)
      {
        case HAUT:      
        SDL_SetColorKey(bomberHaut, SDL_SRCCOLORKEY,
        SDL_MapRGB(bomberHaut->format, 255, 255, 0));
        SDL_BlitSurface(bomberHaut, NULL, ecran, &(bomber));
        break;

        case BAS:
        SDL_SetColorKey(bomberBas, SDL_SRCCOLORKEY,
        SDL_MapRGB(bomberBas-> format, 255, 255, 0));
        SDL_BlitSurface(bomberBas, NULL, ecran, &(bomber));
        break;

        case DROITE: 
        SDL_SetColorKey(bomberDroite, SDL_SRCCOLORKEY,
        SDL_MapRGB(bomberDroite-> format, 255, 255, 0));
        SDL_BlitSurface(bomberDroite, NULL, ecran, &(bomber));
        break;

        case GAUCHE:
        SDL_SetColorKey(bomberGauche, SDL_SRCCOLORKEY,
        SDL_MapRGB(bomberGauche->format, 255, 255, 0));
        SDL_BlitSurface(bomberGauche, NULL, ecran, &(bomber));
        break;
                

      }

        if(map[i][j] == MUR)
        {
          SDL_BlitSurface(mur, NULL, ecran, &(position[i][j]));
        }
        else if(map[i][j] == BRIQUE)
        {      
          SDL_BlitSurface(brique, NULL, ecran, &(position[i][j]));
        }
        else if(map[i][j] == VIDE)
        {
          SDL_BlitSurface(vide, NULL, ecran, &(position[i][j]));
        }
    }
  }
  
  

  SDL_Flip(ecran);
}

SDL_Flip(ecran);


return EXIT_SUCCESS;  
}
