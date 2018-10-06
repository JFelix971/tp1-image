#include "image.h" 
#include<stdio.h>
#include<stdlib.h>

//------------------------------------------------------------------------------
// Code source pour le projet d'UE035
// description : (les fonctions sont définit dans image.h)
//
// les pointeurs images dans ce code sont de type : struct fichierimage *
//
// fonction struct fichierimage * charger(char *)
// permet de charger une image presente sur le disque en memoire vive, la fonction
// renvoie un pointeur de type : struct fichierimage *
//
// fonction int enregistrer(struct fichierimage *,char *)
// permet d'enregistrer une image sur le disque sous le nom donné en arg2, cette 
// image est contenue dans une pointeur de type : struct fichierimage * fournit en arg1
//
// fonction struct fichierimage * nouveau(int,int)
// permet de creer une image en memoire de largeur arg1 et de hauteur arg2, la fonction 
// retourne un pointeur de type : struct fichierimage *
//------------------------------------------------------------------------------
 

void img_nvgris(struct fichierimage *fichier)
{
	int nvgris=0;
	int i, j;
	for(j=0;j<fichier->entetebmp.hauteur;j++)
	{
		for(i=0;i<fichier->entetebmp.largeur;i++)
		{
			nvgris = (fichier->image[i][j].r + fichier->image[i][j].g + fichier->image[i][j].b)/3;
			fichier->image[i][j].r = nvgris;
			fichier->image[i][j].g = nvgris;
			fichier->image[i][j].b = nvgris;
		}
	nvgris=0;
	}
	enregistrer("image_nvgris.bmp",fichier);
	supprimer(fichier);	
}

void img_binaire(struct fichierimage *fichier)
{
	int i, j;
	int seuil=0,nbpix=0;
	
	for(j=0; j<fichier->entetebmp.hauteur; j++)
	{
		for(i=0; i<fichier->entetebmp.largeur; i++)
		{
			seuil = (fichier->image[i][j].r + fichier->image[i][j].g)/2 - fichier->image [i][j].b;
			if( seuil > 0 )
			{
				fichier->image[i][j].r = seuil;
				fichier->image[i][j].g = seuil;
				fichier->image[i][j].b = seuil;
			}
			else
			{
				fichier->image[i][j].r = 0;
				fichier->image[i][j].g = 0;
				fichier->image[i][j].b = 0;

			}

			seuil=0;
		}
	}
	seuil = 75;
	for(j=0; j<fichier->entetebmp.hauteur; j++)
	{
		for(i=0; i<fichier->entetebmp.largeur; i++)
		{
			if( fichier->image[i][j].r > seuil )
			{
				fichier->image[i][j].r = 255;
				fichier->image[i][j].g = 255;
				fichier->image[i][j].b = 255;
			}
			else
			{
				fichier->image[i][j].r = 0;
				fichier->image[i][j].g = 0;
				fichier->image[i][j].b = 0;

			}
		}
	}
	enregistrer("image_binaire.bmp",fichier);
	supprimer(fichier);
}

void erosion (struct fichierimage * fichier)
{
	int i, j ;
	struct fichierimage *buff;
	buff=nouveau(fichier->entetebmp.hauteur,fichier->entetebmp.largeur); 

	for(j=1; j<fichier->entetebmp.hauteur-1; j++)
	{
		for(i=1; i<fichier->entetebmp.largeur-1; i++)
		{
			if( fichier->image[i][j].r > 0)
			{
				//On utilise un filtre manuel pour eroder l image , on verifie chaque voisin
				if( (fichier->image[i-1][j-1].r || fichier->image[i-1][j].r || fichier->image[i-1][j+1].r ||
				     fichier->image[i][j-1].r || fichier->image[i][j+1].r || fichier->image[i+1][j-1].r ||
				     fichier->image[i+1][j].r || fichier->image[i+1][j+1].r ) == 0) 
				{
					buff->image[i][j].r = 0;
					buff->image[i][j].g = 0;
					buff->image[i][j].b = 0;
				}

			}
		}
	}

	enregistrer("image_erode.bmp",buff);
	supprimer(buff);	
}

void dilatation (struct fichierimage * fichier)
{
	int i, j ;
	struct fichierimage *buff;
	buff=nouveau(fichier->entetebmp.hauteur,fichier->entetebmp.largeur); 

	for(i=1; i<fichier->entetebmp.hauteur-1; i++)
	{
		for(j=1; j<fichier->entetebmp.largeur-1; j++)
		{
			if( fichier->image[i][j].r > 0)
			{
				//On utilise un filtre manuel pour eroder l image , on verifie chaque voisin
				if( (fichier->image[i-1][j-1].r || fichier->image[i-1][j].r || fichier->image[i-1][j+1].r ||
				     fichier->image[i][j-1].r || fichier->image[i][j+1].r || fichier->image[i+1][j-1].r ||
				     fichier->image[i+1][j].r || fichier->image[i+1][j+1].r ) > 0) 
				{
					buff->image[i][j].r = 255;
					buff->image[i][j].g = 255;
					buff->image[i][j].b = 255;
				}
				else
				{

					buff->image[i][j].r=0;
					buff->image[i][j].g=0;
					buff->image[i][j].b=0;
				}
			}
		}
	}

	enregistrer("image_dilate.bmp",fichier);

	free(fichier);	
}

int minVoisin(int *tab)
{
	int i, min=0;
	for (i=0;i<9;i++)
	{
		if (tab[i] < min && tab[i] > 0 || min ==0)
		{
			min = tab[i];
		}

	}

	return min;
}

void etiquetage(struct fichierimage* fichier)
{
	int i, j, t;
	int etiquette = 1 ;
	int test = 1;
	int voisins[9]={0,0,0,0,0,0,0,0,0};
	int minvois = 0; 
	int T[fichier->entetebmp.largeur][fichier->entetebmp.hauteur];

	struct fichierimage *buff;
	buff=nouveau(fichier->entetebmp.largeur,fichier->entetebmp.hauteur); 

	for(j=0;j<fichier->entetebmp.hauteur;j++)
		for(i=0;i<fichier->entetebmp.largeur;i++)
			T[i][j]=0;

	while( test == 1)
	{
		test = 0;
		for(j=1; j<fichier->entetebmp.hauteur-1; j++)
		{
			for(i=1; i<fichier->entetebmp.largeur-1; i++)
			{
				if(fichier->image[i][j].r > 0)
				{	 
					voisins[0]=T[i-1][j-1];
					voisins[1]=T[i-1][j];
					voisins[2]=T[i-1][j+1];						     
					voisins[3]=T[i][j-1];
					voisins[4]=T[i][j+1];
					voisins[5]=T[i+1][j-1];
					voisins[6]=T[i+1][j];
					voisins[7]=T[i+1][j+1];
					voisins[8]=T[i][j] == 0 ? etiquette : T[i][j];
					minvois=T[i][j]=minVoisin(voisins); printf(" plus petit voisin de [%d,%d] est %d \n",i,j,minvois);
					etiquette += T[i][j] == etiquette; 
				}//finsi
			}//fin pour2
		}//finpour1

		//second parours bas en haut, droite gauche
		for(j=fichier->entetebmp.hauteur-1; j>1; j--)
		{
			for(i=fichier->entetebmp.largeur-1; i>1; i--)
			{
				if(fichier->image[i][j].r > 0)
				{	 
					voisins[0]=T[i-1][j-1];
					voisins[1]=T[i-1][j];
					voisins[2]=T[i-1][j+1];						     
					voisins[3]=T[i][j-1];
					voisins[4]=T[i][j+1];
					voisins[5]=T[i+1][j-1];
					voisins[6]=T[i+1][j];
					voisins[7]=T[i+1][j+1];
					voisins[8]=T[i][j] == 0 ? etiquette : T[i][j];
					minvois=T[i][j]=minVoisin(voisins); printf(" plus petit voisin de [%d,%d] est %d \n",i,j,minvois);
					etiquette += T[i][j] == etiquette; 
				}//finsi
			}//fin pour2
		}//finpour1
	}//fintantque

	for(j=0;j<fichier->entetebmp.hauteur;j++)
		for(i=0;i<fichier->entetebmp.largeur;i++)
		{
			
				srand(T[i][j]);
				buff->image[i][j].r=rand()%256;
				buff->image[i][j].g=rand()%256;
				buff->image[i][j].b=rand()%256;
		}
	enregistrer("image_etique.bmp",buff);
	supprimer(buff);
}

int main()
{
// variable permettant le parcours d'une image
int i,j;

// exemple de déclaration d'un pointeur image
struct fichierimage *fichier=NULL;

// exemple pour effectuer un copier coller

//Traitement image niv gris
fichier=charger("image.bmp");
img_nvgris(fichier);

//Traitement image couleur a binaire
fichier=charger("image.bmp");
img_binaire(fichier);

//Traitement image binaire to erode
/*fichier=charger("image_binaire.bmp");
erosion(fichier);*/

/*fichier =  charger("image_binaire.bmp");
dilatation(fichier);*/

fichier=charger("image_binaire.bmp");
etiquetage(fichier);

}


