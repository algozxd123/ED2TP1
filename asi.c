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
    while(i < tam && tab[i].chave <= item->chave) { // Itera sobre as chaves da tabela, estas que representam a página
        comparacoes_pesquisa_asi++;
        i++;
    }

    comparacoes_pesquisa_asi++;
    if(i == 0) return 0;
    else{
        comparacoes_pesquisa_asi++;
        if(i < tam) quantitens = ITENSPAGINA; // Se a página em que a iteração parou não for a última, então o tamanho
                                              // Da página é normal, se não, então calcular quantos itens tem pelo else
        else {
            fseek(arq, 0, SEEK_END);
            quantitens = (ftell(arq)/sizeof(tipoitem))%ITENSPAGINA;
            if(quantitens == 0) quantitens = ITENSPAGINA;
        }

        desloc = (tab[i-1].posicao-1)*ITENSPAGINA*sizeof(tipoitem);
        fseek(arq, desloc, SEEK_SET); //Pesquisando no arquivo baseado no tamanho da página e a posição da chave que a representa
        fread(&pagina, sizeof(tipoitem), quantitens, arq); //Página recuperada pelo que foi visto da tabela
        transferencias_pesquisa_asi += quantitens;

        for(i=0; i < quantitens; i++){ // Varrendo a página em busca do valor
            comparacoes_pesquisa_asi++;
            if(pagina[i].chave == item->chave){
                *item = pagina[i]; 
                return 1; // Valor foi encontrado e atribuído ao ponteiro *item
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
    while(fread(&x, sizeof(x), 1, arq) == 1 && i<maxTabela){ //Leitura dos itens no arquivo e construção da tabela
        transferencias_index_asi++;
        cont++;
        if(p_flag) printf("%d ",x.chave);
        comparacoes_index_asi++;
        if(cont%ITENSPAGINA == 1){
            tabela[pos].chave = x.chave; // Armazenando a chave do primeiro elemento de cada página em cada espaço da tabela 
            tabela[pos].posicao = pos+1;
            pos++;
        }
        i++;
    }

    x.chave = chaveBusca;

    end = clock();
    *tempo_index_asi = ((double) (end - start)) / CLOCKS_PER_SEC;

    start = clock();
    if(pesquisa(tabela, pos, &x, arq)){ //Pesquisa com sucesso ou sem sucesso + dados
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
