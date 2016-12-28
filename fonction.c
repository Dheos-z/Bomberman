#include<stdio.h>
#include<stdlib.h>
#include"constante.h"
#include<time.h>
#include"fonction.h"
#include<SDL/SDL.h>



void remplissage(int map[T_TAB][T_TAB])
{
  int i,j;
  int alea = 0;
  srand(time(NULL));
  int compteurM = 0;
  int compteurV = 0;
  int compteurB = 0;


// ON REMPLI LES BORDURES DE LA MAP (MUR)


  for(i = 0; i<15; i++)
  {
    map[i][0] = MUR;
    map[i][T_TAB - 1] = MUR;
    map[0][i] = MUR;
    map[T_TAB - 1][i] = MUR;
  }

// TOUT LE RESTE SERA ICI EGALE A ZERO (POUR LE MOMENT)  

  for(i = 1; i<(T_TAB - 1); i++)
  {
    for(j = 1; j<(T_TAB - 1); j++)
    {
      map[i][j] = VIDE;
    }
  }



// UNE CASE SUR DEUX ON REMPLACE LES 0 PAR DES 1 POUR METTRE DES OBSTACLES

  for(i = 2; i<(T_TAB - 1); i++)
  {
    for(j = 2; j<(T_TAB - 1); j++)
    {
      if(i%2 == 0 && j%2 == 0)
      {
        map[i][j] = MUR;
      }
    }
  }

// ON REMPLIT ICI LES BRIQUES INDISPENSABLES
  
  for(j = 4; j<11; j++)
  {
    map[1][j] = BRIQUE;
    map[13][j] = BRIQUE;
  }

  for(i = 4; i<11; i++)
  {
    map[i][1] = BRIQUE;
    map[i][13] = BRIQUE;
  }
  for(i = 3; i<13; i++)
  {
    for(j = 2; j<13; j++)
    {
      map[3][j] = BRIQUE;
      map[11][j] = BRIQUE;
      map[j][3] = BRIQUE;
      map[j][11] = BRIQUE;
    }
  }

  for(i = 2; i<13; i++)
  {
    if(map[2][i] == 0)
    {
      map[2][i] = BRIQUE;
      map[12][i] = BRIQUE;
      map[i][2] = BRIQUE;
      map[i][12] = BRIQUE;
    }
  }



// ICI NOUS ALLONS REMPLIR LE CENTRE DE LA MAP DE FACON ALEATOIRE

  for(i = 4; i<10; i++)
  {
    for(j = 4; j<10; j++)
    {
      if (map[i][j] == 0)
      {
        alea = rand() % 8;
        if(alea !=0)
        {
          if(map[i][j] == 0)
          {
              map[i][j] = BRIQUE;  // SI LA VALEURE ALEATOIRE VAUT 1, ON REMPLACE PAR BRIQUE
          }
        }
      }
    }
  }

// INITIALISATION DU PERSONNAGE

for(i = 0; i<T_TAB; i++)
{
  for(j = 0; j<T_TAB; j++)
  {
    if(map[i][j] == MUR)
    {
      compteurM++;
    }
    else if(map[i][j] == BRIQUE)
    {
      compteurB++;
    }
  }
}


}




void affichage(int map[T_TAB][T_TAB])
{
  int i,j;

  // AFFICHAGE DE LA MAP

  for(i = 0; i<T_TAB; i++)
  {
    for(j = 0; j<T_TAB; j++)
    {
      if(map[i][j] == VIDE)
      {
        printf("  ");
      }
      else
      {
        printf("%d ", map[i][j]);
      }
    }
    printf("\n");
  }
}








