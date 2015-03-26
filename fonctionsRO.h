#ifndef FONCTIONSRO_H
	#define FONCTIONSRO_H
	
#include <stdio.h>
#include <stdlib.h>



typedef struct {
	int n;
	int nbVar;
	int nbContr;
	int nbCreux;
	int ** t;
}donnees;

typedef struct{
	int tailleUtil;
	int tailleMax;
	int * t;
} tabInt;

typedef struct{
	int taille;
	double * t;
} tabD;

typedef struct{
	tabInt * tabCycles;
	tabInt * tailleCycles;
	tabInt * debutCycles;
}tabC;


tabInt * initTabInt(tabInt * t, int taille);

tabC * initTabC(tabC * t, int taille);
tabC * reinit(tabC * t);
int appartientTab(int n, tabInt * t);
tabInt * remplirPermut( tabInt * perm, double * sol, donnees d);
void affichageTabInt(tabInt * t);
tabC * remplirTabC(tabC * cycles, tabInt * perm);
donnees lecture_fichier(char *file);

#endif