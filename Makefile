all: gcc Projet_NOMS.o -lglpk -lm

Projet_NOMS.o : 
	gcc -c Projets_NOMS.c
	
	
clean: 
	rm -f *.o