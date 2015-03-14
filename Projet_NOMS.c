/* NOM1 Prénom1 - NOM2 Prénom2 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glpk.h> // nous allons utiliser la bibliothèque de fonctions de GLPK 

/* Déclarations pour le compteur de temps CPU */
#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>

#define n 7
#define NBVAR n�
#define NBCONTR 2*n
#define  NBCREUX 2*NBVAR-2*n

struct timeval start_utime, stop_utime;

void crono_start()
{
	struct rusage rusage;
	
	getrusage(RUSAGE_SELF, &rusage);
	start_utime = rusage.ru_utime;
}

void crono_stop()
{
	struct rusage rusage;
	
	getrusage(RUSAGE_SELF, &rusage);
	stop_utime = rusage.ru_utime;
}

double crono_ms()
{
	return (stop_utime.tv_sec - start_utime.tv_sec) * 1000 +
    (stop_utime.tv_usec - start_utime.tv_usec) / 1000 ;
}




int main(int argc, char *argv[])
{
	int i,j,nb;
	int n=7;
	int nbvar=n�;
	int nbcontr=2*n;
	int nbcreux=2*nbvar-2*n;
	
	
	
	double temps;
	int nbsol = 0; /* Compteur du nombre d'appels au solveur GLPK */ 
	int nbcontr = 0; /* Compteur du nombre de contraintes ajoutées pour obtenir une solution composée d'un unique cycle */

	/* Déclarations à compléter... */

	crono_start(); /* Lancement du compteur (juste après le chargement des données à partir d'un fichier */

	glp_prob *prob; /* déclaration du pointeur sur le problème */
	int ia[1 + nbcreux];
	int ja[1 + nbcreux];
	double ar[1 + nbcreux];
	
	/* variables récupérant les résultats de la résolution du problème (fonction objectif et valeur des variables) */
	int i;
	double z; 
	double x[NBVAR]; 
	
	prob = glp_create_prob(); /* allocation mémoire pour le problème */ 
	glp_set_prob_name(prob, "Robot"); /* affectation d'un nom */
	glp_set_obj_dir(prob, GLP_MIN); /* Il s'agit d'un problème de minimisation */
	
	/* Déclaration du nombre de contraintes (nombre de lignes de la matrice des contraintes) : NBCONTR */
	
	glp_add_rows(prob, nbcontr);

	glp_smcp parm;
	glp_init_smcp(&parm);
	parm.msg_lev = GLP_MSG_OFF; /* Paramètre de GLPK dans la résolution d'un PL en variables continues afin de couper les affichages (dans lesquels on se noierait) */

	glp_iocp parmip;
	glp_init_iocp(&parmip);
	parmip.msg_lev = GLP_MSG_OFF; /* Paramètre de GLPK dans la résolution d'un PL en variables entières afin de couper les affichages (dans lesquels on se noierait) */
	
	/* Les appels glp_simplex(prob,NULL); et gpl_intopt(prob,NULL); (correspondant aux paramètres par défaut) seront ensuite remplacés par glp_simplex(prob,&parm); et glp_intopt(prob,&parmip); */
	
	nb=1;
	for(i=1;i<=n;i++){
		for(j=1;j<=n;j++){
			ia[nb]=i;
			ja[nb]=(i-1)*n+j;
			ar[nb]=1;
			nb++;
		}
	}
	for(i=1;i<=n;i++){
		for(j=1;j<=n;j++){
			ia[nb]=n+i;
			ja[nb]=(j-1)*n+i;
			ar[nb]=1;
			nb++;
		}
	}
	
	
	/* Résolution achevée, arrêt du compteur de temps et affichage des résultats */
	crono_stop();
	temps = crono_ms()/1000,0;

	printf("\n");
	puts("Résultat :");
	puts("-----------");
	/* Affichage de la solution sous la forme d'un cycle avec sa longueur à ajouter */
	printf("Temps : %f\n",temps);	
	printf("Nombre d'appels à GPLK : %d\n",nbsol);
	printf("Nombre de contraintes ajoutées : %d\n",nbcontr);

}
