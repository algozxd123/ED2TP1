#ifndef AB_H
#define AB_H

#define M 2
#define MM 4

#include <time.h>
#include <stdio.h>

typedef int TipoChave;

typedef struct TipoRegistro{
    TipoChave chave;
    long dado1;
    char dado2[1000];
    char dado3[5000];
} TipoRegistro;

typedef struct TipoPagina* TipoApontador;

typedef struct TipoPagina{
    short n;
    TipoRegistro r[MM];
    TipoApontador p[MM+1];
} TipoPagina;

int ab(int quantidade, char *filename, int chaveBusca, int *n_comparacoes__index_ab, int *n_transferencias__index_ab, int *n_comparacoes_pesquisa_ab, int *n_transferencias_pesquisa_ab, double *tempo__index_ab, double *tempo_pesquisa_ab, int p_flag);

#endif