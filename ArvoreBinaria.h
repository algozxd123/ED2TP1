#ifndef ARVOREBINARIA_H
#define	ARVOREBINARIA_H

//#define TAM_ARQUIVO 100
//#define TAM_PAGINA 10

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include "Bibliotecas.h"

typedef struct {
    int chave, pagina;
} item;


typedef struct No {
    struct No* pEsq;
    struct No* pDir;
    item   valor;
} No;

typedef struct {
    No*   pRaiz;
} ArvoreBinaria;

typedef enum {
    AB_SUCESSO,
    AB_FALHA
} CodigoErro;

int ArvoreBin(int quantidade, char *filename, int chaveBusca, int *n_comparacoes__index_abin, int *n_transferencias__index_abin, int *n_comparacoes__pesquisa_abin, int *n_transferencias__pesquisa_abin, double *tempo__index_abin, double *tempo__pesquisa_abin, int p_flag);


#endif	/* ARVOREBINARIA_H */

        