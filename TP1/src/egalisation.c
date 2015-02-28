#include "pgm.h"
#include "egalisation.h"
#include "histogramme.h"
#include <stdlib.h>
#include <time.h>

unsigned char** egalise( unsigned char** sortie,  unsigned char** entree, int nl, int nc){
	if (sortie==NULL) sortie=alloue_image(nl,nc);
	int **histo = hist(entree, nl, nc);
	int i;
	int j;
	for(i = 0 ; i < nl ; i++){
		for(j = 0 ; j < nc ; j++){
			sortie[i][j] = 255*hist_cumul(histo,entree[i][j])/(nl*nc);
		}
	}
	/*amélioration*/
	if(amelioration != 0){
		srand (time (NULL));
		int nb_iteration = 100000000;
		int k;
		int pixel_x;
		int pixel_y;
		int **hists = hist(sortie, nl, nc);
		for(k = 0 ; k < nb_iteration ; k++){
			pixel_x = rand()%nl;
			pixel_y = rand()%nc;
			if(hists[1][sortie[pixel_x][pixel_y]] > nc*nl/256){
				hists[1][sortie[pixel_x][pixel_y]]--;
				if(sortie[pixel_x][pixel_y] == 255){
					sortie[pixel_x][pixel_y]--;
					hists[1][sortie[pixel_x][pixel_y]]++;
				}
				else if (sortie[pixel_x][pixel_y] == 0){
					sortie[pixel_x][pixel_y]++;
					hists[1][sortie[pixel_x][pixel_y]]++;
				}
				else{
					if(rand()%2 == 0){
						sortie[pixel_x][pixel_y]++;
						hists[1][sortie[pixel_x][pixel_y]]++;
					}
					else{
						sortie[pixel_x][pixel_y]--;
						hists[1][sortie[pixel_x][pixel_y]]++;
					}
				}
			}
			
		
		}
		free(hists[0]);
		free(hists[1]);
		free(hists);
	}
	free(histo[0]);
	free(histo[1]);
	free(histo);
	return sortie;
}

int hist_cumul(int** hist, int index){
	int h_k = 0;
	int i;
	for(i = 0 ; i <= index ; i ++){
		h_k += hist[1][i];
	}
	return h_k;
}



int main (int ac, char **av) {  /* av[1] contient le nom de l'image, av[2] le nom du resultat . */
  int nb,nl,nc, oldnl,oldnc;
  unsigned char **im2=NULL,** im1=NULL;
  
  if (ac < 2) {printf("Usage : %s entree sortie \n",av[0]); exit(1); }
	/* Lecture d'une image pgm dont le nom est passe sur la ligne de commande */
  im1=lectureimagepgm(av[1],&nl,&nc);
  if (im1==NULL)  { puts("Lecture image impossible"); exit(1); }
  if(ac < 4){
	  amelioration = 0;
  }
  else {
	  amelioration =(int)av[3];
  }
	/* Calcul de l'égalisation de l'image (avec ou sans amélioration*/
  im2=egalise(im2,im1,nl,nc);
	/* Sauvegarde dans un fichier dont le nom est passe sur la ligne de commande */
  ecritureimagepgm(av[2],im2,nl,nc);
  free(*im1);
  free(im1);
  free(*im2);
  free(im2);
}
