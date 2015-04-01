/* Bois Cédric - Dolez Théo*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glpk.h> // nous allons utiliser la bibliothèque de fonctions de GLPK 

/* Déclarations pour le compteur de temps CPU */
#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>
#include "fonctionsRO.h"


struct timeval start_utime, stop_utime;


	



/* Définition des fonctions pour le chronomètre --------------------------------------------------------------------- */

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

/* ------------------------------------------------------------------------------------------------------------------- */


int main(int argc, char *argv[])
{
	int i,j,nb;
	donnees d=lecture_fichier("relief/relief20.dat");
	double temps;
	int nbsol = 0; /* Compteur du nombre d'appels au solveur GLPK */ 
	
	

	crono_start(); /* Lancement du compteur (juste après le chargement des données à partir d'un fichier */

	glp_prob *prob; /* déclaration du pointeur sur le problème */
	tabInt * ia=initTabInt(ia,1+d.nbCreux);
	tabInt * ja=initTabInt(ia,1+d.nbCreux);
	tabD * ar=malloc(sizeof(tabD));
	ar->t=malloc((1+d.nbCreux)*sizeof(double));
	ar->tailleUtil=0;
	ar->tailleMax=1+d.nbCreux;
	
	
	/* variables récupérant les résultats de la résolution du problème (fonction objectif et valeur des variables) */
	
	double z; 
	double x[d.nbVar]; 
	
	
	prob = glp_create_prob(); /* allocation mémoire pour le problème */ 
	glp_set_prob_name(prob, "Robot"); /* affectation d'un nom */
	glp_set_obj_dir(prob, GLP_MIN); /* Il s'agit d'un problème de minimisation */
	
	/* Déclaration du nombre de contraintes (nombre de lignes de la matrice des contraintes) : NBCONTR */
	
	glp_add_rows(prob, d.nbContr);
	
	for(i=1;i<=d.nbContr;++i){
		glp_set_row_bnds(prob, i, GLP_FX, 1.0, 1.0); /*Toutes les contraintes de base doivent être égales à 1*/
	}		
	
	glp_add_cols(prob, d.nbVar); 
	
	
	for(i = 1;i <= d.nbVar;i++)
	{
				
		/* partie obligatoire : bornes éventuelles sur les variables, et type */
		glp_set_col_bnds(prob, i, GLP_DB, 0.0, 1.0); /* Les variables de 1 à NBVAR-1 (xij) sont comprises entre 0 et 1 */
		glp_set_col_kind(prob, i, GLP_BV);	/* Les variables de 1 à NBVAR-1 (xij) sont binaires */
	} 
	
	
	for(i = 1;i <=d.n ;i++) {
		for(j=1;j<=d.n;++j){
			glp_set_obj_coef(prob,(i-1)*d.n+j,d.t[i-1][j-1]); /*on rentre les coefficients de la fonction objectif*/
		}
	}
	
	glp_smcp parm;
	glp_init_smcp(&parm);
	parm.msg_lev = GLP_MSG_OFF; /* Paramètre de GLPK dans la résolution d'un PL en variables continues afin de couper les affichages (dans lesquels on se noierait) */

	glp_iocp parmip;
	glp_init_iocp(&parmip);
	parmip.msg_lev = GLP_MSG_OFF; /* Paramètre de GLPK dans la résolution d'un PL en variables entières afin de couper les affichages (dans lesquels on se noierait) */
	
	/* Les appels glp_simplex(prob,NULL); et gpl_intopt(prob,NULL); (correspondant aux paramètres par défaut) seront ensuite remplacés par glp_simplex(prob,&parm); et glp_intopt(prob,&parmip); */
	
	/* Bloc pour ajouter les contraintes de base au problème -------------------------------------------------------------- */
	nb=1;
	for(i=1;i<=d.n;i++){
		for(j=1;j<=d.n;j++){
			if(i!=j){
				
				ia->t[nb]=i;
				ja->t[nb]=(i-1)*d.n+j;
				ar->t[nb]=1;
				
				
				ia->tailleUtil++;
				ja->tailleUtil++;
				ar->tailleUtil++;
				nb++;
			}
		}
		
	}
	
	for(i=1;i<=d.n;i++){
		for(j=1;j<=d.n;j++){
			if(i!=j){
				
				ia->t[nb]=d.n+j;
				ja->t[nb]=(i-1)*d.n+j;
				ar->t[nb]=1;
				
				ia->tailleUtil++;
				ja->tailleUtil++;
				ar->tailleUtil++;
				nb++;
			}
		}
	}
	/* ---------------------------------------------------------------------------------------------------------------------- */
	
	
	/* chargement de la matrice dans le problème */
	
	glp_load_matrix(prob,d.nbCreux,ia->t,ja->t,ar->t);
	
	
	/* Résolution, puis lecture des résultats */
	
	glp_simplex(prob,NULL);	glp_intopt(prob,NULL); /* Résolution */
	
	
	z = glp_mip_obj_val(prob); /* Récupération de la valeur optimale. Dans le cas d'un problème en variables continues, l'appel est différent : z = glp_get_obj_val(prob); */
	for(i = 0;i < d.nbVar; i++) x[i] = glp_mip_col_val(prob,i+1); /* Récupération de la valeur des variables, Appel différent dans le cas d'un problème en variables continues : for(i = 0;i < p.nbvar;i++) x[i] = glp_get_col_prim(prob,i+1); */

	printf("z = %lf\n",z);
	for(i = 0;i < d.nbVar;i++) printf("x%d%d = %d, ",1+(i/d.n),i%d.n+1,(int)(x[i])); /* un cast est ajouté, x[i] pourrait Ãªtre égal Ã  0.99999... */ 
	puts("");

	
	tabInt * perm=initTabInt(perm,d.n); /* On initialise le TabInt pour les permutations */
	perm= remplirPermut( perm, x, d); /* On le remplit avec la première solution trouvée*/
	tabC * cycles=initTabC(cycles, d.n); /* On initialise la structure TabC en vue de casser les cycles */
	cycles=remplirTabC(cycles, perm); /* On le remplit */
	
	int k;
	while(cycles->tailleCycles->tailleUtil>1 ){ /* Tant qu'il y a plus d'un cycle dans le tableau des cycles, on continue de casser les cycles. */
		
			//On récupère l'indice du pus petit cycle
			i=cycleMin(cycles);
			d.nbContr++;
			
			glp_add_rows(prob, cycles->tailleCycles->t[i]);/*Ajout d'autant de ligne dans la matrice creuse, qu'il y a de cycles dans la solution*/
			glp_set_row_bnds(prob,d.nbContr , GLP_UP, cycles->tailleCycles->t[i]-1, cycles->tailleCycles->t[i]-1);
			
			for(j=0;j<cycles->tailleCycles->t[i];++j){
				
				/* on réalloue de la mémoire pour ia ja et ar si besoin */
				if(ia->tailleUtil>=ia->tailleMax-1 || ja->tailleUtil>=ja->tailleMax-1 || ar->tailleUtil>=ar->tailleMax-1){
					
					
					ia->t=realloc(ia->t,(1+d.nbCreux+20)*sizeof(int));
					ja->t=realloc(ja->t,(1+d.nbCreux+20)*sizeof(int));
					ar->t=realloc(ar->t,(1+d.nbCreux+20)*sizeof(double));
					
					ia->tailleMax+=20;
					ja->tailleMax+=20;
					ar->tailleMax+=20;
				}
				
				/* on rajoute des contraintes en fonction des cycles trouvés */
				d.nbCreux++;
				if(j<cycles->debutCycles->t[i]+cycles->tailleCycles->t[i]-1){
					
					ia->t[nb]=d.nbContr;
					ja->t[nb]=(cycles->tabCycles->t[j]-1)*d.n+cycles->tabCycles->t[j+1];
					ar->t[nb]=1;
					
					ia->tailleUtil++;
					ja->tailleUtil++;
					ar->tailleUtil++;
					
					
				}
				else{
					
					ia->t[nb]=d.nbContr;
					ja->t[nb]=(cycles->tabCycles->t[j]-1)*d.n+cycles->tabCycles->t[cycles->debutCycles->t[i]];
					ar->t[nb]=1;
					
					ia->tailleUtil++;
					ja->tailleUtil++;
					ar->tailleUtil++;
					
				}
				
				nb++;
			}
			
			
		
		
		++nbsol;
		
		printf("Nombre de cycles : %d \n", cycles->tailleCycles->tailleUtil);
		/* chargement de la matrice dans le problème */
			
		glp_load_matrix(prob,d.nbCreux,ia->t,ja->t,ar->t);
		
		/* Optionnel : écriture de la modélisation dans un fichier (TRES utile pour debugger!) */

		glp_write_lp(prob,NULL,"robots.lp");
			
		/* Résolution, puis lecture des résultats */
			
		glp_simplex(prob,NULL);	glp_intopt(prob,NULL); /* Résolution */
		
		for(i = 0;i < d.nbVar; i++) x[i] = glp_mip_col_val(prob,i+1); /* Récupération de la valeur des variables, Appel différent dans le cas d'un problème en variables continues : for(i = 0;i < p.nbvar;i++) x[i] = glp_get_col_prim(prob,i+1); */

		
		if(cycles->debutCycles->tailleUtil>1){ /* Bloc permettant de remplir les permutations et les cycles aevc les nouveaux cycles trouvés */
			cycles=reinit(cycles);
			perm= remplirPermut( perm, x, d);
			cycles=remplirTabC(cycles, perm);
		}
	
	
	}
	
	
	
	
	/* Résolution achevée, arrÃªt du compteur de temps et affichage des résultats */
	crono_stop();
	temps = crono_ms()/1000,0;

	
	
	
	z = glp_mip_obj_val(prob); /* Récupération de la valeur optimale. Dans le cas d'un problème en variables continues, l'appel est différent : z = glp_get_obj_val(prob); */
	for(i = 0;i < d.nbVar; i++) x[i] = glp_mip_col_val(prob,i+1); /* Récupération de la valeur des variables, Appel différent dans le cas d'un problème en variables continues : for(i = 0;i < p.nbvar;i++) x[i] = glp_get_col_prim(prob,i+1); */

	printf("z = %lf\n",z);
	for(i = 0;i < d.nbVar;i++) printf("x%d%d = %d, ",1+(i/d.n),i%d.n+1,(int)(x[i]+0.5)); /* un cast est ajouté, x[i] pourrait Ãªtre égal Ã  0.99999... */ 
	puts("");
	/* libération mémoire */
	glp_delete_prob(prob); 
	
	
	
	printf("\n");
	puts("Résultat :");
	puts("-----------");
	/* Affichage de la solution sous la forme d'un cycle avec sa longueur Ã  ajouter */
	printf("Temps : %f\n",temps);	
	printf("Nombre d'appels Ã  GPLK : %d\n",nbsol);
	printf("Nombre de contraintes ajoutées : %d\n",d.nbContr);
	
	
	
	/* Bloc pour libérer l'espace mémoire ------------------------------------------------------  */
	
	for(i=0;i<d.n;++i) free(d.t[i]);
	free(d.t);
	desallocTabInt(cycles->tailleCycles);
	desallocTabInt(cycles->debutCycles);
	desallocTabInt(cycles->tabCycles);
	free(cycles);
	
	desallocTabInt(perm);
	
	desallocTabInt(ia);
	desallocTabInt(ja);
	free(ar->t);
	free(ar);
	
	/*--------------------------------------------------------------------------------------------- */

}
