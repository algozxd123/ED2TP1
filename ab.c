#include <stdio.h>
#include <stdlib.h>
#include "ab.h"

int transferencias_index_ab = 0;
int transferencias_pesquisa_ab = 0;
int comparacoes_index_ab = 0;
int comparacoes_pesquisa_ab = 0;

void Inicializa(TipoApontador *arvore){//Inicialização da raiz
    *arvore = NULL;
}

int pesquisa_ab(TipoRegistro *x, TipoApontador ap){
    long i=1;
    comparacoes_pesquisa_ab++;
    if(ap == NULL){//Se árvore é vazia ou chegou em um nó folha sem encontrar, apresentar falha na pesquisa
        printf("TipoRegistro nao esta presente na arvore\n");
        return 0;
    }
    
    while(i < ap->n && x->chave > ap->r[i-1].chave) {// Percorre os registros até aquele com um índice com uma chave maior
        comparacoes_pesquisa_ab++;
        i++;
    }
    
    comparacoes_pesquisa_ab++;
    if(x->chave == ap->r[i-1].chave){// Compara o valor procurado com o que o índice em que parou
        *x = ap->r[i-1];
        return 1;
    }

    comparacoes_pesquisa_ab++;
    if(x->chave < ap->r[i-1].chave){// Se o passo anterior não foi um sucesso, realizar recursão dessa função sobre a direita ou esquerda, de acordo o valor procurado
        pesquisa_ab(x, ap->p[i-1]);
    }else{
        pesquisa_ab(x, ap->p[i]);
    }
}

void Imprime(TipoApontador arvore){//Função de Impressão da árvore que percorre os nós de todos os nós e os imprime recursivamente
    int i=0;

    if(arvore == NULL) return;

    while(i <= arvore->n){
        Imprime(arvore->p[i]);
        if(i != arvore->n) printf("%d",arvore->r[i].chave);
        i++;
    }
}

void InsereNaPagina(TipoApontador ap, TipoRegistro reg, TipoApontador apDir){//Função de inserção dentro de uma página
    
    int k = ap->n;
    short naoAchouPosicao = (k>0);//Variável usada para determinar se o loop deve continuar

    while(naoAchouPosicao){//Loop que estabelecer a posição que o novo registro deve ficar
        comparacoes_index_ab++;
        if(reg.chave >= ap->r[k-1].chave){// Compara com o valor da chave no registro na última posição, se está OK, sai do loop, se não,
            naoAchouPosicao = 0;          // os valores são deslocados uma posição para a direita, abrindo espaço, e repete-se o ciclo
            break;                        // Se não houver mais posições, é colocado no início
        }
        ap->r[k] = ap->r[k-1];
        ap->p[k+1] = ap->p[k];
        k--;
        comparacoes_index_ab++;
        if(k < 1) naoAchouPosicao = 0;
    }

    ap->r[k] = reg; //Insere o registro na posição aberta
    ap->p[k+1] = apDir;
    ap->n++;
}

void Ins(TipoRegistro reg, TipoApontador ap, short *cresceu, TipoRegistro *regRetorno, TipoApontador *apRetorno){// Função de inserção dentro da árvore
    long i = 1;//Contadores
    long j;

    TipoApontador apTemp;
    comparacoes_index_ab++;
    if(ap == NULL){
        *cresceu = 1;
        (*regRetorno) = reg;
        (*apRetorno) = NULL;
        return;
    }
    
    while(i < ap->n && reg.chave > ap->r[i-1].chave) {// Avalia onde o registro deve ficar a partir das chaves
        comparacoes_index_ab++;
        i++;
    }

    comparacoes_index_ab++;
    if(reg.chave == ap->r[i-1].chave){ // Avalia se o registro a ser colocado é o mesmo presente na posição onde o loop parou
                                       // E encerra sem crescer
       printf("Erro: Registro ja esta presente\n");
        *cresceu = 0;
        return;
    }

    comparacoes_index_ab++;
    if(reg.chave < ap->r[i-1].chave) i--; // Verifica se o valor do registro é menor que o menor valor da pagina

    Ins(reg, ap->p[i], cresceu, regRetorno, apRetorno);

    comparacoes_index_ab++;
    if(!(*cresceu)) return; // Se estamos em uma página folha ou em uma página que precisa ser expandida, continuar
                            // Se não, está tudo bem e basta retornar

    comparacoes_index_ab++;
    if(ap->n < MM){// Verifica o tamanho da página e se couber mais um registro, utiliza a função
                   // InsereNaPagina para fazer a inserção. E como cabe, o programa acaba
        InsereNaPagina(ap, *regRetorno, *apRetorno);
        *cresceu = 0;
        return;
    }

    apTemp = (TipoApontador)malloc(sizeof(TipoPagina)); // No caso de não ter espaço, cria uma nova página para valores restantes
    apTemp->n = 0;
    apTemp->p[0] = NULL;

    comparacoes_index_ab++;
    if(i < M+1){ // Condicional para decidir se o valor do registro vai ser colocado na nova página ou atual
        InsereNaPagina(apTemp, ap->r[MM-1], ap->p[MM]);
        ap->n--;
        InsereNaPagina(ap, *regRetorno, *apRetorno);
    }else{
        InsereNaPagina(apTemp, *regRetorno, *apRetorno);
    }
    for(int j = M+2; j <= MM; j++){ // Inserção dos valores maiores que o meio na nova página
        comparacoes_index_ab++;
        InsereNaPagina(apTemp, ap->r[j-1], ap->p[j]);
    }
    ap->n = M; // Atualização dos valores e dos ponteiros
    apTemp->p[0] = ap->p[M+1]; // Valor central que irá "subir", junto ao ponteiro de seu filho
    *regRetorno = ap->r[M];
    *apRetorno = apTemp;
}

void insere_ab(TipoRegistro reg, TipoApontador *ap){ //Inicio do processo de insercao e detecta o crescimento da árvore para cima
    short cresceu;
    TipoRegistro regRetorno;
    TipoPagina *apRetorno, *apTemp;

    Ins(reg, *ap, &cresceu, &regRetorno, &apRetorno);
    comparacoes_index_ab++;
    if(cresceu){ //Se o Cresceu de Ins retornar 1 ao chegar na raiz, se cria uma nova página que servirá como raiz
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

    if((arq = fopen(filename,"rb")) == NULL){ // Teste de abertura do arquivo para leitura binária
        printf("Erro na abertura do arquivo\n");
        return 0;
    }


    clock_t start, end;

    start = clock();
    int i = 0;
    Inicializa(&arvore);
    while(fread(&x, sizeof(x), 1, arq) == 1 && i<quantidade){ //Leitura da árvore e inserção de itens em uma árvore inicializada
        transferencias_index_ab++;
        comparacoes_index_ab++;
        if(p_flag) printf("%d ",x.chave);
        insere_ab(x, &arvore);
        i++;
    }

    x.chave = chaveBusca;

    end = clock();
    *tempo_index_ab = ((double) (end - start)) / CLOCKS_PER_SEC;

    start = clock();
    if(pesquisa_ab(&x, arvore)){ //Pesquisa em árvore B
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
