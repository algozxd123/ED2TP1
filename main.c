#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <string.h>
#include "asi.h"
#include "ab.h"
#include "ArvoreBinaria.h"
#include "arvbEstrela.h"

int main(int argc, char** argv){

    /**
     * Método:
     * 1 = acesso sequencial indexado
     * 2 = árvore binária de pesquisa adequada à memória externa
     * 3 = árvore B
     * 4 = árvore B*
     */
    int metodo = strtol(argv[1], NULL, 10);

    //Quantidade
    int n_itens = strtol(argv[2], NULL, 10);
    
    /**
     * Situação:
     * 1 = ascendente
     * 2 = descendente
     * 3 = aleatorio
     */
    int situacao = strtol(argv[3], NULL, 10);

    char filename[18];
    if(situacao == 1){
        strcpy(filename, "arquivo_asc.bin");
    }
    if(situacao == 2){
        strcpy(filename, "arquivo_desc.bin");
    }
    if(situacao == 3){
        strcpy(filename, "arquivo_rand.bin");
    }

    //Chave
    int chave = strtol(argv[4], NULL, 10);

    //Flag opcional -P
    int p_flag = 0;
    if(argc == 6 && !strcmp(argv[5], "-P")) p_flag = 1;

    //Variáveis de medidas de performance
    int n_comparacoes_index = 0;
    int n_transferencias_index = 0;
    int n_comparacoes_pesquisa = 0;
    int n_transferencias_pesquisa = 0;
    clock_t start, end;
    double tempo_index;
    double tempo_pesquisa;

    int resultado_flag = 0;
    
    if(metodo == 1){
        resultado_flag = asi(n_itens, filename, chave, &n_comparacoes_index, &n_transferencias_index, &n_comparacoes_pesquisa, &n_transferencias_pesquisa, &tempo_index, &tempo_pesquisa, p_flag);
    }else if(metodo == 2){
        resultado_flag = ArvoreBin(n_itens, filename, chave, &n_comparacoes_index, &n_transferencias_index, &n_comparacoes_pesquisa, &n_transferencias_pesquisa, &tempo_index, &tempo_pesquisa, p_flag);
    }else if(metodo == 3){
        resultado_flag = ab(n_itens, filename, chave, &n_comparacoes_index, &n_transferencias_index, &n_comparacoes_pesquisa, &n_transferencias_pesquisa, &tempo_index, &tempo_pesquisa, p_flag);
    }else if(metodo == 4){
        resultado_flag = abe(n_itens, filename, chave, &n_comparacoes_index, &n_transferencias_index, &n_comparacoes_pesquisa, &n_transferencias_pesquisa, &tempo_index, &tempo_pesquisa, p_flag);
    }

    if(resultado_flag){
        printf("Numero de transferencias na criacao dos indices: %d\n",n_transferencias_index);
        printf("Numero de comparacoes na criacao dos indices: %d\n",n_comparacoes_index);
        printf("Numero de transferencias na pesquisa: %d\n",n_transferencias_pesquisa);
        printf("Numero de comparacoes na pesquisa: %d\n",n_comparacoes_pesquisa);
        printf("Tempo de execucao na criacao dos indices: %lf s\n",tempo_index);
        printf("Tempo de execucao na pesquisa: %lf s\n",tempo_pesquisa);
    }
    
    return 0;
}