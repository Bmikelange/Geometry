#include <GL/gl.h>
#include <math.h>
#include "o_objet.h"
#include "u_table.h"
#include "t_geometrie.h"

struct mon_bezier
{
  Table_quadruplet courbe;
  Table_quadruplet ControlePoint;
  int nb_points;
} ; 

void calculus(struct mon_bezier *o)
{
  Table_quadruplet temporaire;
  ALLOUER(temporaire.table,o->ControlePoint.nb);
  temporaire.nb=o->ControlePoint.nb;	
  free(o->courbe.table);
  ALLOUER(o->courbe.table,o->nb_points+1);
  o->courbe.nb=o->nb_points+1;
  for(int t=0;t<o->nb_points;t++)
  {
  	float facteur= (float)t/(float)o->nb_points;
  	for(int i=0;i<temporaire.nb;i++)
	  {
	    temporaire.table[i].x=o->ControlePoint.table[i].x*o->ControlePoint.table[i].h;
	    temporaire.table[i].y=o->ControlePoint.table[i].y*o->ControlePoint.table[i].h;
	    temporaire.table[i].z=o->ControlePoint.table[i].z*o->ControlePoint.table[i].h;
	    temporaire.table[i].h=o->ControlePoint.table[i].h;  
	  }
    for(int i=1;i<temporaire.nb;i++)
    {
      for(int z=0;z<temporaire.nb-i;z++)
      {
    	temporaire.table[z].x=(1-facteur)*temporaire.table[z].x+facteur*temporaire.table[z+1].x;
    	temporaire.table[z].y=(1-facteur)*temporaire.table[z].y+facteur*temporaire.table[z+1].y;
    	temporaire.table[z].z=(1-facteur)*temporaire.table[z].z+facteur*temporaire.table[z+1].z;
    	temporaire.table[z].h=(1-facteur)*temporaire.table[z].h+facteur*temporaire.table[z+1].h;
      }
  	}
    o->courbe.table[t].x=temporaire.table[0].x/temporaire.table[0].h;
    o->courbe.table[t].y=temporaire.table[0].y/temporaire.table[0].h;
  	o->courbe.table[t].z=temporaire.table[0].z/temporaire.table[0].h;
  	o->courbe.table[t].h=temporaire.table[0].h;
  }
    o->courbe.table[o->nb_points].x=o->ControlePoint.table[o->ControlePoint.nb-1].x;
    o->courbe.table[o->nb_points].y=o->ControlePoint.table[o->ControlePoint.nb-1].y;
    o->courbe.table[o->nb_points].z=o->ControlePoint.table[o->ControlePoint.nb-1].z;
    o->courbe.table[o->nb_points].h=o->ControlePoint.table[o->ControlePoint.nb-1].h;
    free(temporaire.table);
}

static void changement(struct mon_bezier *o)
{ 
  
  if ( ! (UN_CHAMP_CHANGE(o)||CREATION(o)) )
    return ;
    
  
  if (CHAMP_CHANGE(o,nb_points) || CHAMP_CHANGE(o,ControlePoint))
  {
    calculus(o);
    printf("dans changement\n");
  }
  else
    printf(" sans changement\n");
}


static void affiche_mon_bezier(struct mon_bezier *o)
{
  int j;
  
  glBegin(GL_LINES) ;
  for(j=0  ; j<o->courbe.nb-1 ; j++)
  {
    glVertex3f(o->courbe.table[j].x,o->courbe.table[j].y,o->courbe.table[j].z); 
    glVertex3f(o->courbe.table[j+1].x,o->courbe.table[j+1].y,o->courbe.table[j+1].z); 
  }
  glEnd();
}




CLASSE(mon_bezier, struct mon_bezier,      
       CHAMP(courbe, L_table_point P_table_quadruplet Sauve)
       CHAMP(nb_points, LABEL("Nombre de points") 
       L_entier  Edite Sauve DEFAUT("10") )  
       CHAMP(ControlePoint, L_table_point P_table_quadruplet Sauve Extrait)
       
       CHANGEMENT(changement)
       CHAMP_VIRTUEL(L_affiche_gl(affiche_mon_bezier))
       
       MENU("Exemples_MG/mon_bezier")
       EVENEMENT("Ctrl+BZ")
       )
       
