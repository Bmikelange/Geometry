#include <GL/gl.h>
#include <math.h>
#include "o_objet.h"
#include "u_table.h"
#include "t_geometrie.h"

struct bezier_next
{
  Table_quadruplet courbe;
  Table_quadruplet vecteur;
  Table_quadruplet ControlePoint;
  Table_quadruplet ControleTangente;
  int nb_points;
  Booleen AfficheManettes;
  Booleen RempliCourbe;
} ; 


void calcul_tangente(struct bezier_next * o)
{
  for(int i=0;i<o->ControlePoint.nb;i++)
  {
    o->ControleTangente.table[i].x=(o->ControlePoint.table[i].x+o->vecteur.table[i].x);
    o->ControleTangente.table[i].y=(o->ControlePoint.table[i].y+o->vecteur.table[i].y);
    o->ControleTangente.table[i].z=(o->ControlePoint.table[i].z+o->vecteur.table[i].z);
    o->ControleTangente.table[i].h=o->ControlePoint.table[i].h;
  }
}

void calcul_vecteur(struct bezier_next * o)
{
  free(o->vecteur.table);
  ALLOUER(o->vecteur.table,o->ControlePoint.nb);
  o->vecteur.nb=o->ControlePoint.nb;
  for(int i=0;i<o->ControlePoint.nb;i++)
  {
    o->vecteur.table[i].x=(o->ControleTangente.table[i].x-o->ControlePoint.table[i].x);
    o->vecteur.table[i].y=(o->ControleTangente.table[i].y-o->ControlePoint.table[i].y);
    o->vecteur.table[i].z=(o->ControleTangente.table[i].z-o->ControlePoint.table[i].z);
    o->vecteur.table[i].h=o->ControleTangente.table[i].h;
  }
}


void creation_table(int i,int j, Table_quadruplet temporaire,struct bezier_next * o,int t)
{
    float facteur= (float)t/(float)o->nb_points;
    temporaire.table[0].x=o->ControlePoint.table[i].x*o->ControlePoint.table[i].h;
    temporaire.table[0].y=o->ControlePoint.table[i].y*o->ControlePoint.table[i].h;
    temporaire.table[0].z=o->ControlePoint.table[i].z*o->ControlePoint.table[i].h;
    temporaire.table[0].h=o->ControlePoint.table[i].h; 

    temporaire.table[1].x=o->ControleTangente.table[i].x*o->ControleTangente.table[i].h;
    temporaire.table[1].y=o->ControleTangente.table[i].y*o->ControleTangente.table[i].h;
    temporaire.table[1].z=o->ControleTangente.table[i].z*o->ControleTangente.table[i].h;
    temporaire.table[1].h=o->ControleTangente.table[i].h; 

    temporaire.table[2].x=(o->ControlePoint.table[j].x-o->vecteur.table[j].x)*o->ControleTangente.table[j].h;
    temporaire.table[2].y=(o->ControlePoint.table[j].y-o->vecteur.table[j].y)*o->ControleTangente.table[j].h;
    temporaire.table[2].z=(o->ControlePoint.table[j].z-o->vecteur.table[j].z)*o->ControleTangente.table[j].h;
    temporaire.table[2].h=o->ControleTangente.table[j].h; 

    temporaire.table[3].x=o->ControlePoint.table[j].x*o->ControlePoint.table[j].h;
    temporaire.table[3].y=o->ControlePoint.table[j].y*o->ControlePoint.table[j].h;
    temporaire.table[3].z=o->ControlePoint.table[j].z*o->ControlePoint.table[j].h;
    temporaire.table[3].h=o->ControlePoint.table[j].h; 

    for(int n=1;n<temporaire.nb;n++)
    {
      for(int z=0;z<temporaire.nb-n;z++)
      {
      temporaire.table[z].x=(1-facteur)*temporaire.table[z].x+facteur*temporaire.table[z+1].x;
      temporaire.table[z].y=(1-facteur)*temporaire.table[z].y+facteur*temporaire.table[z+1].y;
      temporaire.table[z].z=(1-facteur)*temporaire.table[z].z+facteur*temporaire.table[z+1].z;
      temporaire.table[z].h=(1-facteur)*temporaire.table[z].h+facteur*temporaire.table[z+1].h;
      }
    }
    o->courbe.table[i*o->nb_points+t].x=temporaire.table[0].x/temporaire.table[0].h;
    o->courbe.table[i*o->nb_points+t].y=temporaire.table[0].y/temporaire.table[0].h;
    o->courbe.table[i*o->nb_points+t].z=temporaire.table[0].z/temporaire.table[0].h;
    o->courbe.table[i*o->nb_points+t].h=temporaire.table[0].h;
    printf("%f %f %f \n",o->courbe.table[i*o->nb_points+t].x,o->courbe.table[i*o->nb_points+t].y,o->courbe.table[i*o->nb_points+t].z);
}

void calculus(struct bezier_next *o)
{
  free(o->courbe.table);
  int value=o->ControlePoint.nb-1;
  if(o->RempliCourbe==1)
  {
    value=o->ControlePoint.nb;
  }
  ALLOUER(o->courbe.table,o->nb_points*value+1);
  o->courbe.nb=o->nb_points*value+1;
  Table_quadruplet temporaire;
  ALLOUER(temporaire.table,4);
  temporaire.nb=4;
  for(int i=0;i<o->ControlePoint.nb-1;i++)
  {
    for(int t=0;t<o->nb_points;t++)
    {
      creation_table(i,i+1,temporaire,o,t);
    }
  }
  
  o->courbe.table[o->courbe.nb-1]=o->ControlePoint.table[o->ControlePoint.nb-1];

  if(o->RempliCourbe==1)
  { 
    for(int t=0;t<o->nb_points;t++)
    {
      creation_table(o->ControlePoint.nb-1,0,temporaire,o,t);
    }
    o->courbe.table[o->courbe.nb-1]=o->ControlePoint.table[0];
  }
  free(temporaire.table);
}

static void create(struct bezier_next *o)
{
  free(o->vecteur.table);
  ALLOUER(o->vecteur.table,o->ControlePoint.nb);
  o->vecteur.nb=o->ControlePoint.nb;
  for(int i=0;i<o->ControlePoint.nb-1;i++)
  {
    o->vecteur.table[i].x=(o->ControlePoint.table[i+1].x-o->ControlePoint.table[i].x)/2;
    o->vecteur.table[i].y=(o->ControlePoint.table[i+1].y-o->ControlePoint.table[i].y)/2;
    o->vecteur.table[i].z=(o->ControlePoint.table[i+1].z-o->ControlePoint.table[i].z)/2;
    o->vecteur.table[i].h=o->ControlePoint.table[i].h;
  }
  o->vecteur.table[o->ControlePoint.nb-1].x=(o->ControlePoint.table[0].x-o->ControlePoint.table[o->ControlePoint.nb-1].x)/2;
  o->vecteur.table[o->ControlePoint.nb-1].y=(o->ControlePoint.table[0].y-o->ControlePoint.table[o->ControlePoint.nb-1].y)/2;
  o->vecteur.table[o->ControlePoint.nb-1].z=(o->ControlePoint.table[0].z-o->ControlePoint.table[o->ControlePoint.nb-1].z)/2;
  o->vecteur.table[o->ControlePoint.nb-1].h=o->ControlePoint.table[o->ControlePoint.nb-1].h;

  calcul_tangente(o);
  
}


static void changement(struct bezier_next *o)
{ 
  if(CREATION(o))
    create(o);
  if ( ! (UN_CHAMP_CHANGE(o)||CREATION(o)) )
    return ;
  
  if (CHAMP_CHANGE(o,nb_points) || CHAMP_CHANGE(o,ControlePoint) || CHAMP_CHANGE(o,RempliCourbe) || CHAMP_CHANGE(o,ControleTangente))
  {
    if(CHAMP_CHANGE(o,ControlePoint))
      calcul_tangente(o);
    if(CHAMP_CHANGE(o,ControleTangente))
      calcul_vecteur(o);
    calculus(o);
  }
}


static void affiche_bezier(struct bezier_next *o)
{
  int j;
  NOTE_CHAMP_CHANGE(o,ControleTangente);
  glColor3d(1,0,0);
  glBegin(GL_LINES) ;
  for(j=0  ; j<o->courbe.nb-1 ; j++)
  {
    glVertex3f(o->courbe.table[j].x,o->courbe.table[j].y,o->courbe.table[j].z); 
    glVertex3f(o->courbe.table[j+1].x,o->courbe.table[j+1].y,o->courbe.table[j+1].z); 
  }
  glEnd();

  glColor3d(0,1,0);
  if(o->AfficheManettes)
  {
    for(j=0  ; j<o->ControlePoint.nb ; j++)
    {
      glBegin(GL_LINES) ;
      glVertex3f(o->ControlePoint.table[j].x-o->vecteur.table[j].x,o->ControlePoint.table[j].y-o->vecteur.table[j].y,o->ControlePoint.table[j].z-o->vecteur.table[j].z);
      glVertex3f(o->ControlePoint.table[j].x,o->ControlePoint.table[j].y,o->ControlePoint.table[j].z);
      glVertex3f(o->ControlePoint.table[j].x,o->ControlePoint.table[j].y,o->ControlePoint.table[j].z);
      glVertex3f(o->ControleTangente.table[j].x,o->ControleTangente.table[j].y,o->ControleTangente.table[j].z);
      
      glEnd();
    }
  }
  
}




CLASSE(bezier_next, struct bezier_next,      
       CHAMP(courbe, L_table_point P_table_quadruplet Sauve)
       CHAMP(vecteur, L_table_point P_table_quadruplet Sauve)
       CHAMP(nb_points, LABEL("Nombre de points") 
       L_entier  Edite Sauve DEFAUT("10") )  
       CHAMP(ControlePoint, L_table_point P_table_quadruplet Extrait Sauve Obligatoire)
       CHAMP(ControleTangente, L_table_point P_table_quadruplet Extrait Sauve Obligatoire)
       CHAMP(AfficheManettes, LABEL("Affiche les Manettes") L_booleen Edite Sauve DEFAUT("0"))
       CHAMP(RempliCourbe, LABEL("rempli la courbe")  L_booleen Edite Sauve DEFAUT("0"))
       
       CHANGEMENT(changement)
       CHAMP_VIRTUEL(L_affiche_gl(affiche_bezier))
       
       MENU("Exemples_MG/bezier_next")
       EVENEMENT("Ctrl+BZ")
       )
       
