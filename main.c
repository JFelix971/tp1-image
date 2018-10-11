#include "image.h" 
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <SDL/SDL_image.h>
//------------------------------------------------------------------------------
// Code source pour le projet d'UE035
// description : (les fonctions sont d√©finit dans image.h)
//
// les pointeurs images dans ce code sont de type : struct fichierimage *
//
// fonction struct fichierimage * charger(char *)
// permet de charger une image presente sur le disque en memoire vive, la fonction
// renvoie un pointeur de type : struct fichierimage *
//
// fonction int enregistrer(struct fichierimage *,char *)
// permet d'enregistrer une image sur le disque sous le nom donn√© en arg2, cette 
// image est contenue dans une pointeur de type : struct fichierimage * fournit en arg1
//
// fonction struct fichierimage * nouveau(int,int)
// permet de creer une image en memoire de largeur arg1 et de hauteur arg2, la fonction 
// retourne un pointeur de type : struct fichierimage *
//------------------------------------------------------------------------------
void ouvrir_image(int longueur, int largeur, char nom_image[], char nom_image2[])
{

    SDL_Surface *ecran = NULL, *imageDeFond = NULL, *imageDeFond2 = NULL;

    SDL_Rect positionFond;
    SDL_Rect positionFond2;
    


    positionFond.x = 0;

    positionFond.y = 0;
    
    positionFond2.x = largeur;

    positionFond2.y = 0;


    SDL_Init(SDL_INIT_VIDEO);


    /* Chargement de l'icÙne AVANT SDL_SetVideoMode */

    ecran = SDL_SetVideoMode(largeur*2, longueur*2, 32, SDL_HWSURFACE);
    SDL_WM_SetCaption("Chargement d'images en SDL", NULL);


    imageDeFond = SDL_LoadBMP(nom_image);
    imageDeFond2 = SDL_LoadBMP(nom_image2);
    SDL_BlitSurface(imageDeFond, NULL, ecran, &positionFond);
    SDL_BlitSurface(imageDeFond2, NULL, ecran, &positionFond2);

    SDL_Flip(ecran);

    pause();


    SDL_FreeSurface(imageDeFond);
    SDL_FreeSurface(imageDeFond2);
    SDL_Quit();

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
	//ouvrir_image(fichier->entetebmp.hauteur,fichier->entetebmp.largeur,"image_nvgris.bmp");	
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
	//ouvrir_image(fichier->entetebmp.hauteur,fichier->entetebmp.largeur,"image_binaire.bmp");	
}

void erosion (struct fichierimage * fichier)
{
	int i, j, minvois=0;
	int voisins[9]={0,0,0,0,0,0,0,0,0};
	
	struct fichierimage *buff;
	buff=nouveau(fichier->entetebmp.largeur,fichier->entetebmp.hauteur); 

	for(j=1; j<fichier->entetebmp.hauteur-1; j++)
	{
		for(i=1; i<fichier->entetebmp.largeur-1; i++)
		{
			if( fichier->image[i][j].r > 0)
			{
				//buff->image[i][j].r=255;
				//buff->image[i][j].g=255;
				//buff->image[i][j].b=255;
				voisins[0]=fichier->image[i-1][j-1].r; 
				voisins[1]=fichier->image[i-1][j].r;
				voisins[2]=fichier->image[i-1][j+1].r;		     
				voisins[3]=fichier->image[i][j-1].r;
				voisins[4]=fichier->image[i][j+1].r;
				voisins[5]=fichier->image[i+1][j-1].r;
				voisins[6]=fichier->image[i+1][j].r;
				voisins[7]=fichier->image[i+1][j+1].r;
				voisins[8]=fichier->image[i][j].r;
				minvois=minVoisin(voisins);
				//On utilise un filtre manuel pour eroder l image , on verifie chaque voisin
				if(minvois==0) 
				{
					buff->image[i][j].r = 0;
					buff->image[i][j].g = 0;
					buff->image[i][j].b = 0;
					printf("pix off \n");
				}
				else
				{
					buff->image[i][j].r=255;
					buff->image[i][j].g=255;
					buff->image[i][j].b=255;	
				}

			}
		}
	}

	enregistrer("image_erode.bmp",buff);
	supprimer(buff);
	//ouvrir_image(fichier->entetebmp.hauteur,fichier->entetebmp.largeur,"image_erode.bmp");		
}

void dilatation (struct fichierimage * fichier)
{
	int i, j ;
	struct fichierimage *buff;
	buff=nouveau(fichier->entetebmp.largeur,fichier->entetebmp.hauteur); 

	for(j=1; j<fichier->entetebmp.hauteur-1; j++)
	{
		for(i=1; i<fichier->entetebmp.largeur-1; i++)
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

	enregistrer("image_dilate.bmp",buff);
	free(buff);
	//ouvrir_image(fichier->entetebmp.hauteur,fichier->entetebmp.largeur,"image_dilate.bmp");			
}

void etiquetage(struct fichierimage* fichier)/*OK*/
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
					minvois=T[i][j]=minVoisin(voisins);
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
					minvois=T[i][j]=minVoisin(voisins);
					etiquette += T[i][j] == etiquette; 
				}//finsi
			}//fin pour2
		}//finpour1
	}//fintantque
	//Affichage de l image etiquetter en mettant des couleurs correspondant a chaque etiquette
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
	//ouvrir_image(fichier->entetebmp.hauteur,fichier->entetebmp.largeur,"image_etique.bmp");		
}

int main()
{
// variable permettant le parcours d'une image
int i,j;

// exemple de dÈclaration d'un pointeur image
struct fichierimage *fichier=NULL;

// exemple pour effectuer un copier coller

//Traitement image niv gris
//fichier=charger("image.bmp");
//img_nvgris(fichier);

//Traitement image couleur a binaire
fichier=charger("image.bmp");
img_binaire(fichier);

//Traitement image binaire to erode
fichier=charger("image_binaire.bmp");
erosion(fichier);
	ouvrir_image(fichier->entetebmp.hauteur,fichier->entetebmp.largeur,"image_binaire.bmp","image_etique.bmp");		
/*fichier =  charger("image_binaire.bmp");
dilatation(fichier);*/

//fichier=charger("image_binaire.bmp");
//etiquetage(fichier);

}


