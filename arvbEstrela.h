#ifndef ARVBESTRELA_H
#define ARVBESTRELA_H

#define M 2
#include <time.h>
#include <stdlib.h>

typedef int Chave;
typedef struct{
    Chave chave;
    long dado1;
    char dado2[1000];
    char dado3[5000];
}TRegistro;

typedef enum {Interna, Externa} TipoIntExt;
typedef struct Pagina* PontPag;
typedef struct Pagina{
    TipoIntExt Pt;
    union{
        struct{
            int ni;
            Chave ri[M + M];
            PontPag pi[M + M + 1];
        } U0;
        struct {
            int ne;
            TRegistro re[M + M];
        } U1;
    } UU;
    
}Pagina;

int abe(int quantidade, char* filename, int chaveBusca, int *n_comparacoes_index_ab, int *n_transferencias_index_abe, int *n_comparacoes_pesquisa_abe, int *n_transferencias_pesquisa_abe, double *tempo_index_abe, double *tempo_pesquisa_abe, int p_flag);

#endif