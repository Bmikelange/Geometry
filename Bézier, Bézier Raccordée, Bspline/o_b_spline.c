#include <GL/gl.h>
#include <math.h>
#include "o_objet.h"
#include "u_table.h"
#include "t_geometrie.h"

struct B_spline
{
  Table_quadruplet courbe;
  Table_quadruplet ControlePoint;
  int nb_points;
  int degre;
}; 


int returnR(float u,float noeud[],int nbCP)
{
	int r=0;
	for(int i=0;i<nbCP;i++)
  {
    if(noeud[i]<=u && noeud[i+1]>=u)
    {
      r=i;
      return r;
    }
  }
	return r;
}

void calculus(struct B_spline *o)
{
  if(o->degre>o->ControlePoint.nb)
    o->degre=o->ControlePoint.nb;
  int k = o->degre+1;
  int n = o->ControlePoint.nb;
  int taille_noeuds = k + n + 1;
  float noeuds[taille_noeuds];
  for(int i=0;i<k;i++)
  {
    noeuds[i] = 0.0;
  }
  for(int i=n+1;i<taille_noeuds;i++)
  {
    noeuds[i]=1.0;
  }
  for(int i=k;i<=n;i++)
  {
    noeuds[i]= ((float)i-(float)k+1.0f)/((float)n-(float)k+2.0f);
  }

  printf("succes creation noeuds \n");
	
  free(o->courbe.table);
  printf("je veux allouer: %d \n",o->nb_points);
  ALLOUER(o->courbe.table,o->nb_points);
  o->courbe.nb= o->nb_points;
  printf("succes allocation \n");
  
  int count =2;
  o->courbe.table[0] = o->ControlePoint.table[0];
  o->courbe.table[o->courbe.nb-1] = o->ControlePoint.table[n-1];
  for(int t=1;t<o->courbe.nb-1;t++)
  {
    count++;
    Table_quadruplet temporaire;
    ALLOUER(temporaire.table,n+1);
    temporaire.nb=n+1;
    float u = (float) t/(float) (o->courbe.nb);
    int r=returnR(u,noeuds,taille_noeuds);
    printf("r : %d\n",r);
    for (int i = 0; i < n; i++)
    {
      temporaire.table[i].x=o->ControlePoint.table[i].x*o->ControlePoint.table[i].h;
      temporaire.table[i].y=o->ControlePoint.table[i].y*o->ControlePoint.table[i].h;
      temporaire.table[i].z=o->ControlePoint.table[i].z*o->ControlePoint.table[i].h;
      temporaire.table[i].h=o->ControlePoint.table[i].h;  
    }
    temporaire.table[n].x=o->ControlePoint.table[n-1].x*o->ControlePoint.table[n-1].h;
    temporaire.table[n].y=o->ControlePoint.table[n-1].y*o->ControlePoint.table[n-1].h;
    temporaire.table[n].z=o->ControlePoint.table[n-1].z*o->ControlePoint.table[n-1].h;
    temporaire.table[n].h=o->ControlePoint.table[n-1].h;
    for(int j=1;j<k;j++)
    {
      for(int i=r;i>=r-k+1+j;i--)
      {
        float firstfact = (u-noeuds[i])/(noeuds[i+k-j]-noeuds[i]);
        float secondfact = (noeuds[i+k-j]-u)/(noeuds[i+k-j]-noeuds[i]);
        temporaire.table[i].x = temporaire.table[i].x*firstfact + temporaire.table[i-1].x*secondfact;
        temporaire.table[i].y = temporaire.table[i].y*firstfact + temporaire.table[i-1].y*secondfact;
        temporaire.table[i].z = temporaire.table[i].z*firstfact + temporaire.table[i-1].z*secondfact;
        temporaire.table[i].h = temporaire.table[i].h*firstfact + temporaire.table[i-1].h*secondfact;
      }
    }
    o->courbe.table[t].x = temporaire.table[r].x / temporaire.table[r].h;
    o->courbe.table[t].y = temporaire.table[r].y / temporaire.table[r].h;
    o->courbe.table[t].z = temporaire.table[r].z / temporaire.table[r].h;
    o->courbe.table[t].h = temporaire.table[r].h;
    printf("x %f,y %f,z %f,h %f\n",o->courbe.table[t].x,o->courbe.table[t].y,o->courbe.table[t].z,o->courbe.table[t].h);
    free(temporaire.table);
  }
  printf("nbPoint : %d , count : %d \n",o->courbe.nb,count);
  printf("fin\n");
}

static void changement(struct B_spline *o)
{ 
  
  if ( ! (UN_CHAMP_CHANGE(o)||CREATION(o)) )
    return ;
    
  if (CHAMP_CHANGE(o,nb_points) || CHAMP_CHANGE(o,ControlePoint) || CHAMP_CHANGE(o,degre))
  {
    calculus(o);
    printf("dans changement\n");
  }
  else
    printf(" sans changement\n");
}

static void affiche_B_spline(struct B_spline *o)
{
  int j;
  glBegin(GL_LINES) ;
  for(j=0  ; j< o->courbe.nb -1 ; j++)
  {
    glVertex3f(o->courbe.table[j].x,o->courbe.table[j].y,o->courbe.table[j].z); 
    glVertex3f(o->courbe.table[j+1].x,o->courbe.table[j+1].y,o->courbe.table[j+1].z); 
  }
  glEnd();
}

CLASSE(B_spline, struct B_spline,      
       CHAMP(courbe, L_table_point P_table_quadruplet Sauve)
       CHAMP(nb_points, LABEL("Nombre de points")
       L_entier  Edite Sauve DEFAUT("10") )  
       CHAMP(degre, LABEL("degré B_spline")
       L_entier  Edite Sauve DEFAUT("1") )
       CHAMP(ControlePoint, L_table_point P_table_quadruplet Sauve Extrait)
       
       CHANGEMENT(changement)
       CHAMP_VIRTUEL(L_affiche_gl(affiche_B_spline))
       
       MENU("Exemples_MG/B_spline")
       EVENEMENT("Ctrl+BS")
       )
