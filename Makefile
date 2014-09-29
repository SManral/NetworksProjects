all:
	gcc -g project2.c project2.h student2.c -o project2exe
	gcc -g project2.c project2.h GBN.c -o GBNexe
	gcc -g project2.c project2.h bidirect.c -o bidirectexe