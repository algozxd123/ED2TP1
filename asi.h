#ifndef ASI_H
#define ASI_H

#define ITENSPAGINA 4

#include <stdio.h>
#include <time.h>

typedef struct {
    int posicao;
    int chave;
} tipoindice;

typedef struct {
    int chave;
    long dado1;
    char dado2[1000];
    char dado3[5000];
} tipoitem;

int asi(int maxTabela, char *filename, int chaveBusca, int *n_comparacoes_index_asi, int *n_transferencias_index_asi, int *n_comparacoes_pesquisa_asi, int *n_transferencias_pesquisa_asi, double *tempo_index_asi, double *tempo_pesquisa_asi, int p_flag);

#endif