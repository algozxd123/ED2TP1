#include "asi.h"

int transferencias_index_asi = 0;
int transferencias_pesquisa_asi = 0;
int comparacoes_index_asi = 0;
int comparacoes_pesquisa_asi = 0;

int pesquisa(tipoindice tab[], int tam, tipoitem* item, FILE *arq){
    tipoitem pagina[ITENSPAGINA];
    int i, quantitens;
    long desloc;

    i = 0;
    while(i < tam && tab[i].chave <= item->chave) {
        comparacoes_pesquisa_asi++;
        i++;
    }

    comparacoes_pesquisa_asi++;
    if(i == 0) return 0;
    else{
        comparacoes_pesquisa_asi++;
        if(i < tam) quantitens = ITENSPAGINA;
        else {
            fseek(arq, 0, SEEK_END);
            quantitens = (ftell(arq)/sizeof(tipoitem))%ITENSPAGINA;
            if(quantitens == 0) quantitens = ITENSPAGINA;
        }

        desloc = (tab[i-1].posicao-1)*ITENSPAGINA*sizeof(tipoitem);
        fseek(arq, desloc, SEEK_SET);
        fread(&pagina, sizeof(tipoitem), quantitens, arq);
        transferencias_pesquisa_asi += quantitens;

        for(i=0; i < quantitens; i++){
            comparacoes_pesquisa_asi++;
            if(pagina[i].chave == item->chave){
                *item = pagina[i];
                return 1;
            }
        }
        return 0;
    }
}

int asi(int maxTabela, char* filename, int chaveBusca, int *n_comparacoes_index_asi, int *n_transferencias_index_asi, int *n_comparacoes_pesquisa_asi, int *n_transferencias_pesquisa_asi, double *tempo_index_asi, double *tempo_pesquisa_asi, int p_flag){
    tipoindice tabela[maxTabela];
    FILE *arq;
    tipoitem x;
    int pos, cont;

    clock_t start, end;

    start = clock();
    if((arq = fopen(filename,"rb")) == NULL){
        printf("Erro na abertura do arquivo\n");
        return 0;
    }

    cont = 0;
    pos = 0;

    int i = 0;
    while(fread(&x, sizeof(x), 1, arq) == 1 && i<maxTabela){
        transferencias_index_asi++;
        cont++;
        if(p_flag) printf("%d ",x.chave);
        comparacoes_index_asi++;
        if(cont%ITENSPAGINA == 1){
            tabela[pos].chave = x.chave;
            tabela[pos].posicao = pos+1;
            pos++;
        }
        i++;
    }

    x.chave = chaveBusca;

    end = clock();
    *tempo_index_asi = ((double) (end - start)) / CLOCKS_PER_SEC;

    start = clock();
    if(pesquisa(tabela, pos, &x, arq)){
        printf("Item de chave %d foi localizado\n",x.chave);

        *n_transferencias_index_asi = transferencias_index_asi;
        *n_comparacoes_index_asi = comparacoes_index_asi;
        *n_transferencias_pesquisa_asi = transferencias_pesquisa_asi;
        *n_comparacoes_pesquisa_asi = comparacoes_pesquisa_asi;

        end = clock();
        *tempo_pesquisa_asi = ((double) (end - start)) / CLOCKS_PER_SEC;
        fclose(arq);

        return 1;
    }else{
        printf("Item de chave %d nao foi localizado\n",x.chave);

        *n_transferencias_index_asi = transferencias_index_asi;
        *n_comparacoes_index_asi = comparacoes_index_asi;
        *n_transferencias_pesquisa_asi = transferencias_pesquisa_asi;
        *n_comparacoes_pesquisa_asi = comparacoes_pesquisa_asi;
        
        end = clock();
        *tempo_pesquisa_asi = ((double) (end - start)) / CLOCKS_PER_SEC;
        fclose(arq);
        
        return 0;
    }
}