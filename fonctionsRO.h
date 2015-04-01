#ifndef FONCTIONSRO_H
	#define FONCTIONSRO_H
	
#include <stdio.h>
#include <stdlib.h>

//-------------------Déclaration des différentes structures---------------------------


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
	int tailleUtil;
	int tailleMax;
	double * t;
} tabD;

typedef struct{
	tabInt * tabCycles;
	tabInt * tailleCycles;
	tabInt * debutCycles;
}tabC;

//----------------------------------------Déclaration des différentes fonctions--------------------------------------------------------



// fonction désallouant un TabInt
void desallocTabInt(tabInt * t);

// fonction initialisant un TabInt
tabInt * initTabInt(tabInt * t, int taille);

// fonction initialisant un TabC
tabC * initTabC(tabC * t, int taille);

// fonction qui réinitialise un tabC 
tabC * reinit(tabC * t);

// test d'appartenance à un TabInt
int appartientTab(int n, tabInt * t);

// fonction qui remplit le TabInt des Permutations (en vue de casser les cycles)
tabInt * remplirPermut( tabInt * perm, double * sol, donnees d);

// fonction qui affiche un TabInt
void affichageTabInt(tabInt * t);

// fonction qui remplit le TabC des cycles (en vue de casser les cycles)
tabC * remplirTabC(tabC * cycles, tabInt * perm);

// fonction pour lire les fichiers
donnees lecture_fichier(char *file);

int cycleMin(tabC * cycles);

#endif
