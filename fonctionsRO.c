#include "fonctionsRO.h"


tabInt * initTabInt(tabInt * t, int taille){
	t=malloc(sizeof(tabInt));
	t->t=malloc(sizeof(int)*taille);
	t->tailleUtil=0;
	t->tailleMax=taille;
}

tabC * initTabC(tabC * t, int taille){
	t=malloc(sizeof(tabC));
	
	t->tabCycles=initTabInt(t->tabCycles, taille);
	t->tailleCycles=initTabInt(t->tailleCycles, taille);
	t->debutCycles=initTabInt(t->debutCycles, taille);
	
	return t;
}

tabC * reinit(tabC * t){
	int i;
	for(i=0;i<t->tabCycles->tailleUtil;++i){
		t->tabCycles->t[i]=0;
		t->tailleCycles->t[i]=0;
		t->debutCycles->t[i]=0;
	}
	t->tabCycles->tailleUtil=0;
	t->tailleCycles->tailleUtil=0;
	t->debutCycles->tailleUtil=0;
	return t;
}
	
	
	

int appartientTab(int n, tabInt * t){
	int i;
	for(i=0;i<t->tailleMax;++i){
		if( t->t[i]==n){
			return 1;
		}
	}
	return 0;
}

tabInt * remplirPermut( tabInt * perm, double * sol, donnees d){
	int i,j;
	for(i=1;i<=d.n;++i){
		for(j=1;j<=d.n;++j){
			if((int)(sol[(i-1)*d.n+j-1]+0.5)==1){
				perm->t[i-1]=j;
			}
		}
	}
	return perm;
}

void affichageTabInt(tabInt * t){
	int i;
	printf("\n");
	for(i=0;i<t->tailleUtil;++i){
		printf("%d \n",t->t[i]);
	}
	printf("\n");
}



tabC * remplirTabC(tabC * cycles, tabInt * perm){
	int i=1,j=0,k=0, n=0, deb=0;
	
	
	cycles->debutCycles->t[0]=0;
	cycles->debutCycles->tailleUtil++;
	
	while(cycles->tabCycles->tailleUtil!=cycles->tabCycles->tailleMax){
		
		
		if(appartientTab(perm->t[i-1], cycles->tabCycles)==0 ){
			
			cycles->tabCycles->t[k]=perm->t[i-1];
			cycles->tabCycles->tailleUtil++;
			++k;
			++n;
			++deb;
			i=perm->t[i-1];
		}
		else{
			cycles->tailleCycles->t[j]=n;
			cycles->tailleCycles->tailleUtil++;
			
			cycles->debutCycles->t[j+1]=deb;
			cycles->debutCycles->tailleUtil++;
			
			++j;
			n=0;
			for(i=1;appartientTab(perm->t[i-1], cycles->tabCycles) && i<cycles->tabCycles->tailleMax;++i) ;
		}
	}
	cycles->tailleCycles->t[j]=n;
	cycles->tailleCycles->tailleUtil++;
	
	
	return cycles;
	
}

		
	


donnees lecture_fichier(char *file){
	donnees d;
	int val,i,j;
	FILE *fin;
	
	fin = fopen(file,"r");
	
	fscanf(fin,"%d",&val);
	d.n = val;
	
	d.nbVar=(d.n)*(d.n);
	d.nbContr=2*d.n;
	d.nbCreux=2*d.nbVar-2*d.n;
	
	
	
	
	
	d.t=(int **)malloc(sizeof(int *)*d.n);
	for(i=0;i<d.n;++i) d.t[i]=(int *) malloc(sizeof(int)*d.n);
	printf("%d \n",val);
	
	for(i=0;i<d.n;++i){
		for(j=0;j<d.n;++j){
			fscanf(fin,"%d",&val);
			
			d.t[i][j]=val;
		}	
	}
	
	for(i=0;i<d.n;++i){
		for(j=0;j<d.n;++j){
			printf(" %d ",d.t[i][j]);
		}
		printf(" \n");
	}
	return d;
	
}
