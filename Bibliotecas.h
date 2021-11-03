#ifndef BIBLIOTECAS_H
#define	BIBLIOTECAS_H

//////Constantes ////////

#define TAM_PAGINA 10
#define M 2 // lembrar de testar ..... 

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <stdio.h>
#include <assert.h>

typedef struct{
    
    int chave;
    int pos;

}Item;

typedef struct{
    int chave;
    long int dado1;
    char dado2[1000]; 
    char dado3[5000];   
}Registro;

typedef struct{
    
    int posicao;
    int chave;

}tipoIndice;

int pesquisaBinaria(Registro *,Registro *,int);

#endif	/* BIBLIOTECAS_H */