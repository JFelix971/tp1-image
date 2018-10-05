#include "image.h" 

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
	for(i=fichier->entetebmp.hauteur;i>0;i--)
	{
		for(j=0;j<fichier->entetebmp.largeur;j++)
		{
			nvgris = (fichier->image[i][j].r + fichier->image[i][j].g + fichier->image[i][j].b)/3;
			fichier->image[i][j].r = nvgris;
			fichier->image[i][j].g = nvgris;
			fichier->image[i][j].b = nvgris;
		}
	nvgris=0;
	}
	enregistrer("image_nvgris.bmp",fichier);
	free(fichier);	
}

void img_binaire(struct fichierimage *fichier)
{
	int i, j;
	int seuil=0,nbpix=0;
	
	for(i=fichier->entetebmp.hauteur; i>0; i--)
	{
		for(j=0; j<fichier->entetebmp.largeur; j++)
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
	for(i=fichier->entetebmp.hauteur; i>0; i--)
	{
		for(j=0; j<fichier->entetebmp.largeur; j++)
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
	free(fichier);
}

void erosion (struct fichierimage * fichier)
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
				     fichier->image[i+1][j].r || fichier->image[i+1][j+1].r ) == 0) 
				{
					buff->image[i][j].r = 0;
					buff->image[i][j].g = 0;
					buff->image[i][j].b = 0;
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

	enregistrer("image_erode.bmp",fichier);
	free(fichier);	
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
	int i, min=0, max=0;
	min = max = tab[0];
	for (i=0;i<8;i++)
	{
		if (tab[i] < min)
		{
			min = tab[i];
		}
		if(tab[i]>max){ max= tab[i];}
	}

	return min;
}

void etiquetage(struct fichierimage* fichier)
{
	int i, j, t;
	int etiquette = 1 ;
	int test = 1;
	int voisins[8]={0,0,0,0,0,0,0,0};
	int minvois = 0; 
	int T[fichier->entetebmp.hauteur][fichier->entetebmp.largeur]	;

	for(i=0;i<fichier->entetebmp.hauteur;i++)
		for(j=0;j<fichier->entetebmp.largeur;j++)
			T[i][j]=fichier->image[i][j].r;


	while( test == 1)
	{
		test = 0;
		for(i=1; i<fichier->entetebmp.hauteur-1; i++)
		{
			for(j=1; j<fichier->entetebmp.largeur-1; j++)
			{
				if(fichier->image[i][j].r > 0)
				{	//on regarde si dans l image buff ce pixel possede une etiquette 
					if(T[i][j] == 0)
					{
						voisins[0]=T[i-1][j-1];
						voisins[1]=T[i-1][j];
						voisins[2]=T[i-1][j+1];						     
						voisins[3]=T[i][j-1];
						voisins[4]=T[i][j+1];
						voisins[5]=T[i+1][j-1];
						voisins[6]=T[i+1][j];
						voisins[7]=T[i+1][j+1];
						minvois=minVoisin(voisins); printf(" plus petit voisin de [%d,%d] est %d \n",i,j,minvois);
						//On utilise un filtre manuel pour regarder voisin du pixel i,j
						if(  minvois == 0) 
						{	
							T[i][j] = etiquette;
							etiquette += 1;//printf("min voisin= %d etiquette : %d \n",minvois,etiquette);
						}
						else
						{
							T[i][j]=minvois;printf("ici \n");
						}
						test+=1;
					}
					else
					{printf("ici 2 \n");
						voisins[0]=T[i-1][j-1];
						voisins[1]=T[i-1][j];
						voisins[2]=T[i-1][j+1];						     
						voisins[3]=T[i][j-1];
						voisins[4]=T[i][j+1];
						voisins[5]=T[i+1][j-1];
						voisins[6]=T[i+1][j];
						voisins[7]=T[i+1][j+1];
						minvois=minVoisin(voisins);
						if(T[i][j] != (minvois=minVoisin(voisins)))
						{//printf("ici iffff \n");
							T[i][j]=minvois;printf("ici iffff min vois %d\n",minvois);
							test=1;
						}
					}//finsinon
				}//finsi
			}//fin pour2
		}//finpour1

//second parours bas en haut, droite gauche
		/*for(i=fichier->entetebmp.hauteur-1;i>1; i--)
		{
			for(j=fichier->entetebmp.largeur-1;j>1 ; j--)
			{	//on verfiei qu 1 pixel est allume dans l image de base
				if(fichier->image[i][j].r > 0)
				{	//on regarde si dans l image buff ce pixel possede une etiquette 
					if(T[i][j] == 0)
					{
						voisins[0]=T[i-1][j-1];
						voisins[1]=T[i-1][j];
						voisins[2]=T[i-1][j+1];						     
						voisins[3]=T[i][j-1];
						voisins[4]=T[i][j+1];
						voisins[5]=T[i+1][j-1];
						voisins[6]=T[i+1][j];
						voisins[7]=T[i+1][j+1]; 
						//On utilise un filtre manuel pour regarder voisin du pixel i,j
						if( minvois=minVoisin(voisins) == 0) 
						{	
							T[i][j] = etiquette;
							T[i][j] = etiquette;
							T[i][j] = etiquette;
							etiquette += 1;
						}
						else
						{
							T[i][j]=minvois;
						}
						test+=1;
					}
					else
					{
						if(T[i][j] != (minvois=minVoisin(voisins)))
						{
							T[i][j]=minvois;
							test=1;
						}
					}//finsinon
				}//finsi
			}//fin pour2

		}//finpour1*/
	}//fintantque

//printf("\n etiquette = %d",etiquette);
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
fichier=charger("image_binaire.bmp");
erosion(fichier);

fichier =  charger("image_binaire.bmp");
dilatation(fichier);

fichier=charger("image_binaire.bmp");
etiquetage(fichier);

}
