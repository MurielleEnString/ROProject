all: Projet_BoisDolez.o fonctionsRO.o
	gcc -g Projet_BoisDolez.o fonctionsRO.o -lglpk -lm

Projet_BoisDolez.o: 
	gcc -g -c Projet_BoisDolez.c
	
fonctionsRO.o:
	gcc -g fonctionsRO.c -c
	
clean: 
	rm -f *.o
