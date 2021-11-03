#include <stdio.h>
#include <stdlib.h>
#include "ab.h"

int transferencias_index_ab = 0;
int transferencias_pesquisa_ab = 0;
int comparacoes_index_ab = 0;
int comparacoes_pesquisa_ab = 0;

void Inicializa(TipoApontador *arvore){
    *arvore = NULL;
}

int Pesquisa(TipoRegistro *x, TipoApontador ap){
    long i=1;
    comparacoes_pesquisa_ab++;
    if(ap == NULL){
        printf("TipoRegistro nao esta presente na arvore\n");
        return 0;
    }
    
    while(i < ap->n && x->chave > ap->r[i-1].chave) {
        comparacoes_pesquisa_ab++;
        i++;
    }
    
    comparacoes_pesquisa_ab++;
    if(x->chave == ap->r[i-1].chave){
        *x = ap->r[i-1];
        return 1;
    }

    comparacoes_pesquisa_ab++;
    if(x->chave < ap->r[i-1].chave){
        Pesquisa(x, ap->p[i-1]);
    }else{
        Pesquisa(x, ap->p[i]);
    }
}

void Imprime(TipoApontador arvore){
    int i=0;

    if(arvore == NULL) return;

    while(i <= arvore->n){
        Imprime(arvore->p[i]);
        if(i != arvore->n) printf("%d",arvore->r[i].chave);
        i++;
    }
}

void InsereNaPagina(TipoApontador ap, TipoRegistro reg, TipoApontador apDir){
    
    int k = ap->n;
    short naoAchouPosicao = (k>0);

    while(naoAchouPosicao){
        comparacoes_index_ab++;
        if(reg.chave >= ap->r[k-1].chave){
            naoAchouPosicao = 0;
            break;
        }
        ap->r[k] = ap->r[k-1];
        ap->p[k+1] = ap->p[k];
        k--;
        comparacoes_index_ab++;
        if(k < 1) naoAchouPosicao = 0;
    }

    ap->r[k] = reg;
    ap->p[k+1] = apDir;
    ap->n++;
}

void Ins(TipoRegistro reg, TipoApontador ap, short *cresceu, TipoRegistro *regRetorno, TipoApontador *apRetorno){
    long i = 1;
    long j;

    TipoApontador apTemp;
    comparacoes_index_ab++;
    if(ap == NULL){
        *cresceu = 1;
        (*regRetorno) = reg;
        (*apRetorno) = NULL;
        return;
    }
    
    while(i < ap->n && reg.chave > ap->r[i-1].chave) {
        comparacoes_index_ab++;
        i++;
    }

    comparacoes_index_ab++;
    if(reg.chave == ap->r[i-1].chave){
        printf("Erro: Registro ja esta presente\n");
        *cresceu = 0;
        return;
    }

    comparacoes_index_ab++;
    if(reg.chave < ap->r[i-1].chave) i--;

    Ins(reg, ap->p[i], cresceu, regRetorno, apRetorno);

    comparacoes_index_ab++;
    if(!(*cresceu)) return;

    comparacoes_index_ab++;
    if(ap->n < MM){
        InsereNaPagina(ap, *regRetorno, *apRetorno);
        *cresceu = 0;
        return;
    }

    apTemp = (TipoApontador)malloc(sizeof(TipoPagina));
    apTemp->n = 0;
    apTemp->p[0] = NULL;

    comparacoes_index_ab++;
    if(i < M+1){
        InsereNaPagina(apTemp, ap->r[MM-1], ap->p[MM]);
        ap->n--;
        InsereNaPagina(ap, *regRetorno, *apRetorno);
    }else{
        InsereNaPagina(apTemp, *regRetorno, *apRetorno);
    }
    for(int j = M+2; j <= MM; j++){
        comparacoes_index_ab++;
        InsereNaPagina(apTemp, ap->r[j-1], ap->p[j]);
    }
    ap->n = M;
    apTemp->p[0] = ap->p[M+1];
    *regRetorno = ap->r[M];
    *apRetorno = apTemp;
}

void Insere(TipoRegistro reg, TipoApontador *ap){
    short cresceu;
    TipoRegistro regRetorno;
    TipoPagina *apRetorno, *apTemp;

    Ins(reg, *ap, &cresceu, &regRetorno, &apRetorno);
    comparacoes_index_ab++;
    if(cresceu){
        apTemp = (TipoPagina*)malloc(sizeof(TipoPagina));
        apTemp->n = 1;
        apTemp->r[0] = regRetorno;
        apTemp->p[1] = apRetorno;
        apTemp->p[0] = *ap;
        *ap = apTemp;
    }
}

int ab(int quantidade, char* filename, int chaveBusca, int *n_comparacoes_index_ab, int *n_transferencias_index_ab, int *n_comparacoes_pesquisa_ab, int *n_transferencias_pesquisa_ab, double *tempo_index_ab, double *tempo_pesquisa_ab, int p_flag){
    TipoApontador arvore;
    FILE *arq;
    TipoRegistro x;
    int pos, cont;

    if((arq = fopen(filename,"rb")) == NULL){
        printf("Erro na abertura do arquivo\n");
        return 0;
    }


    clock_t start, end;

    start = clock();
    int i = 0;
    Inicializa(&arvore);
    while(fread(&x, sizeof(x), 1, arq) == 1 && i<quantidade){
        transferencias_index_ab++;
        comparacoes_index_ab++;
        if(p_flag) printf("%d ",x.chave);
        Insere(x, &arvore);
        i++;
    }

    x.chave = chaveBusca;

    end = clock();
    *tempo_index_ab = ((double) (end - start)) / CLOCKS_PER_SEC;

    start = clock();
    if(Pesquisa(&x, arvore)){
        printf("Item de chave %d foi localizado\n",x.chave);
        *n_transferencias_index_ab = transferencias_index_ab;
        *n_comparacoes_index_ab = comparacoes_index_ab;
        *n_transferencias_pesquisa_ab = transferencias_pesquisa_ab;
        *n_comparacoes_pesquisa_ab = comparacoes_pesquisa_ab;
        fclose(arq);

        end = clock();
        *tempo_pesquisa_ab = ((double) (end - start)) / CLOCKS_PER_SEC;
        return 1;
    }else{
        printf("Item de chave %d nao foi localizado\n",x.chave);
        *n_transferencias_index_ab = transferencias_index_ab;
        *n_comparacoes_index_ab = comparacoes_index_ab;
        *n_transferencias_pesquisa_ab = transferencias_pesquisa_ab;
        *n_comparacoes_pesquisa_ab = comparacoes_pesquisa_ab;
        fclose(arq);

        end = clock();
        *tempo_pesquisa_ab = ((double) (end - start)) / CLOCKS_PER_SEC;
        return 0;
    }
}