#include <stdio.h>
#include <stdlib.h>
#include "arvbEstrela.h"

int transferencias_index_abe = 0;
int transferencias_pesquisa_abe = 0;
int comparacoes_index_abe = 0;
int comparacoes_pesquisa_abe = 0;

//Pesquisa quase identica ao slide, so mudou o tipo de retorno de void para Int
int pesquisa_abe(TRegistro *x, PontPag *pagina){
    int i;
    PontPag Pag;
    Pag = *pagina;
    comparacoes_pesquisa_abe++;
    if((*pagina)->Pt == Interna){
        i = 1;
        while(i < Pag->UU.U0.ni && x->chave > Pag->UU.U0.ri[i-1]){
            comparacoes_pesquisa_abe++;
            i++;
        }
        comparacoes_pesquisa_abe++;
        if(x->chave < Pag->UU.U0.ri[i-1])
            return pesquisa_abe(x, &Pag->UU.U0.pi[i-1]);
        else
            return pesquisa_abe(x, &Pag->UU.U0.pi[i]);
    }
    i = 1;
    while(i < Pag->UU.U1.ne && x->chave > Pag->UU.U1.re[i-1].chave){
        comparacoes_pesquisa_abe++;
        i++;
    }
    comparacoes_pesquisa_abe++;
    if(x->chave == Pag->UU.U1.re[i-1].chave){
        *x = Pag->UU.U1.re[i-1];
        return 1;
    }
    else
        return 0;
}   

//Parte da Insercao da arvore B para paginas internas (vulgo arvore B) identico ao slide
void InsereNaPag(PontPag pagina, Chave Reg, PontPag paginaDireita){

    short NotFind;
    int k = pagina->UU.U0.ni;
    while(NotFind){
        comparacoes_index_abe++;
        if(Reg >= pagina->UU.U0.ri[k-1]){
            NotFind = 0;
            break;
        }
        pagina->UU.U0.ri[k] = pagina->UU.U0.ri[k-1];
        pagina->UU.U0.pi[k+1] = pagina->UU.U0.pi[k];
        k--;
        comparacoes_index_abe++;
        if(k<1)
            NotFind = 0;
    }
    pagina->UU.U0.ri[k] = Reg;
    pagina->UU.U0.pi[k+1] = paginaDireita;
    pagina->UU.U0.ni++;
}

//Outra parte da insercao da Arvore B para paginas internas (vulgo arvore B), identico ao slide
void InsereAuxArvB(PontPag pagina, short *Cresceu, Chave *ChaveRetorno, PontPag *pagRetorno){
    int i = 1;
    PontPag pagTemp;
    while(i < pagina->UU.U0.ni && *ChaveRetorno > pagina->UU.U0.ri[i-1]){
        comparacoes_index_abe++;
        i++;
    }
    comparacoes_index_abe++;
    if(*ChaveRetorno == pagina->UU.U0.ri[i-1]){
        *Cresceu = 0;
        return;
    }
    comparacoes_index_abe++;
    if(*ChaveRetorno < pagina->UU.U0.ri[i-1])
        i--;
    comparacoes_index_abe++;
    if(!*Cresceu)
        return;
    comparacoes_index_abe++;
    if(pagina->UU.U0.ni < M + M){
        for(int j = pagina->UU.U0.ni; j > i; j--){
            comparacoes_index_abe++;
            pagina->UU.U0.ri[j] = pagina->UU.U0.ri[j-1];
            pagina->UU.U0.pi[j+1] = pagina->UU.U0.pi[j];
        }
        pagina->UU.U0.ni++;
        /*
        if(*ChaveRetorno == 16 && pagina->Pt == Interna){
            for(int u = 0; u < pagina->UU.U0.pi[0]->UU.U1.ne; u++){
                printf("1a Pagina [%d] = %d\n", u, pagina->UU.U0.pi[0]->UU.U1.re[u].chave);
            }
            for(int u = 0; u < (*pagRetorno)->UU.U1.ne; u++){
                printf("DEBUG [%d] = %d\n", u, (*pagRetorno)->UU.U1.re[u].chave);
            }
        }
        */
        pagina->UU.U0.ri[i] = *ChaveRetorno;
        pagina->UU.U0.pi[i+1] = *pagRetorno;
        /*
        printf("Chave = %d\n", *ChaveRetorno);
        InsereNaPag(pagina, *ChaveRetorno, *pagRetorno);
        */
        *Cresceu = 0;
        return;
    }
    pagTemp = (PontPag) malloc(sizeof(Pagina));
    pagTemp->Pt = Interna;
    pagTemp->UU.U0.ni = 0;
    pagTemp->UU.U0.pi[0] = NULL;
    comparacoes_index_abe++;
     //Caso o registro a ser inserido seja menor ou igual a mediana, entrara na pagina a esquerda
    if(i < M+1){
        int k = M-1;
        int j;
        //Coloca as chaves na pagina a ser criada
        for(j = pagina->UU.U0.ni - 1; j > M - 1; j--){
            comparacoes_index_abe++;
            pagTemp->UU.U0.ri[k] = pagina->UU.U0.ri[j];
            pagTemp->UU.U0.pi[k+1] = pagina->UU.U0.pi[j+1];
            pagTemp->UU.U0.ni++;
            k--;
        }
        pagina->UU.U0.ni = M;
        //Caso a chave Retorno esteja no meio da pagina, nao se muda o *ChaveRetorno, uma vez que ela "subira" para a prox chave
        comparacoes_index_abe++;
        if(i == M){
            pagTemp->UU.U0.pi[0] = *pagRetorno;
            *pagRetorno = pagTemp;
        }
        //Caso nao, colocar o *ChaveRetorno na pagina e na posicao correta e pegar o proximo *ChaveRetorno que ira "subir"
        else{
            pagTemp->UU.U0.pi[0] = pagina->UU.U0.pi[j+1];
            Chave aux = pagina->UU.U0.ri[j];
            for(int u = M; u > i; u--){
                pagina->UU.U0.ri[u] = pagina->UU.U0.ri[u-1];
                pagina->UU.U0.pi[u+1] = pagina->UU.U0.pi[u];
            }
            pagina->UU.U0.ri[i] = *ChaveRetorno;
            pagina->UU.U0.pi[i+1] = *pagRetorno;
            //Pagina a direita passada por referencia
            *pagRetorno = pagTemp;
            *ChaveRetorno = aux;
        }

    }
    //Registro maior que a Mediana
    else{
        int k = M-1;
        int j;
        //Coloca as chaves na pagina a ser criada
        for(j = pagina->UU.U0.ni - 1; j > M - 1; j--){
            pagTemp->UU.U0.ri[k] = pagina->UU.U0.ri[j];
            pagTemp->UU.U0.pi[k+1] = pagina->UU.U0.pi[j+1];
            pagTemp->UU.U0.ni++;
            k--;
        }
        pagina->UU.U0.ni = M;
        //colocar o *ChaveRetorno na pagina e posicao correta e pegar o proximo *ChaveRetorno que ira "subir"
        Chave aux = pagTemp->UU.U0.ri[0];
        for(int u = 0; u < i-M; u++){
            pagTemp->UU.U0.pi[u] = pagTemp->UU.U0.pi[u+1];
            pagTemp->UU.U0.ri[u] = pagTemp->UU.U0.ri[u+1];
        }
        pagTemp->UU.U0.ri[i-M-1] = *ChaveRetorno;
        pagTemp->UU.U0.pi[i-M] = *pagRetorno;
        *pagRetorno = pagTemp;
        *ChaveRetorno = aux;
    }
}

//Insercao para a arvore B*
void InsereAux(TRegistro Reg, PontPag pagina, short *Cresceu, Chave *ChaveRetorno, PontPag *pagRetorno){

    int i = 1;
    //Ira percorrer a Estrutura da Arvore ate achar o local correto do registro
    comparacoes_index_abe++;
    if((pagina->Pt == Interna)){
        while(i < pagina->UU.U0.ni && Reg.chave > pagina->UU.U0.ri[i-1]){
            comparacoes_index_abe++;
            i++;
        }
        comparacoes_index_abe++;
        if(Reg.chave < pagina->UU.U0.ri[i-1])
            i--;
        //Recursao para acessar a proxima pagina
        InsereAux(Reg, pagina->UU.U0.pi[i], Cresceu, ChaveRetorno, pagRetorno);
        comparacoes_index_abe++;
        if(*Cresceu){
            //Insercao igual ArvB normal
            InsereAuxArvB(pagina, Cresceu, ChaveRetorno, pagRetorno);
        }
        return;
    }
    else{
        comparacoes_index_abe++;
        //ESSA PARTE DO CODIGO SE REFERE A INSERCAO NAS PAGINAS EXTERNAS
        if(pesquisa_abe(&Reg, &pagina))
            //Nao faz sentido colocar um registro que ja existe
            return;
        //Verificar aonde o registro entrara na pagina externa
        while(i < pagina->UU.U1.ne && Reg.chave > pagina->UU.U1.re[i-1].chave){
            comparacoes_index_abe++;
            i++;
        }
        comparacoes_index_abe++;
        if(Reg.chave < pagina->UU.U1.re[i-1].chave)
            i--;
        //Caso a pagina tenha menos de 2M itens
        comparacoes_index_abe++;
        if(pagina->UU.U1.ne < M + M){
            for(int j = pagina->UU.U1.ne; j > i; j--){
                comparacoes_index_abe++;
                pagina->UU.U1.re[j] = pagina->UU.U1.re[j-1];
            }
            pagina->UU.U1.ne++;
            pagina->UU.U1.re[i] = Reg;
            *Cresceu = 0;
            return;
        }
        //Caso a pagina tenha 2M itens, sera necessario fazer rearranjo das paginas
        else{
            PontPag pagDireita = (PontPag) malloc(sizeof(Pagina));
            pagDireita->Pt = Externa;
            pagDireita->UU.U1.ne = 0;
            int k = M;
            comparacoes_index_abe++;
            //Caso o registro a ser inserido seja menor que a mediana, entrara na pagina a esquerda
            if(i < M ){
                for(int j = pagina->UU.U1.ne - 1; j >= M - 1; j--){
                    comparacoes_index_abe++;
                    pagDireita->UU.U1.re[k] = pagina->UU.U1.re[j];
                    pagDireita->UU.U1.ne++;
                    k--;
                    pagina->UU.U1.re[j] = pagina->UU.U1.re[j-1];
                }
                pagina->UU.U1.ne = M;
                for(int j = M-1; j > i; j--){
                    comparacoes_index_abe++;
                    pagina->UU.U1.re[j] = pagina->UU.U1.re[j-1];
                }
                pagina->UU.U1.re[i] = Reg;
                *Cresceu = 1;
                *ChaveRetorno = pagDireita->UU.U1.re[0].chave;
                *pagRetorno = pagDireita;
                //Subir a mediana e passar a referencia
            }
            else{
                //Caso o registro a ser inserido seja maior ou igual a mediana, entrara na pagina a direita
                for(int j = pagina->UU.U1.ne - 1; j > M - 1; j--){
                    comparacoes_index_abe++;
                    pagDireita->UU.U1.re[k] = pagina->UU.U1.re[j];
                    pagDireita->UU.U1.ne++;
                    k--;
                    pagina->UU.U1.re[j] = pagina->UU.U1.re[j-1];
                }
                pagina->UU.U1.ne = M;
                if(i - M != 0 && pagDireita->UU.U1.re[i-M].chave < Reg.chave){
                    comparacoes_index_abe++;
                    for(int j = 0; j < i-M; j++)
                        pagDireita->UU.U1.re[j] = pagDireita->UU.U1.re[j+1];
                }
                pagDireita->UU.U1.ne++;
                pagDireita->UU.U1.re[i-M] = Reg;
                *Cresceu = 1;
                *ChaveRetorno = pagDireita->UU.U1.re[0].chave;
                *pagRetorno = pagDireita;
                //Subir a mediana e passar a referencia
            }
        }
    }
    return;
}
//Inicio do processo de Insercao
void _insere_abe(TRegistro x, PontPag *arvore){
    //Caso a arvore esteja vazia, iniciar a pagina Interna e as paginas externas
    comparacoes_index_abe++;
    if(*arvore == NULL){
        Pagina *pagNova = NULL;
        Pagina *pagDireita = NULL;
        Pagina *pagEsquerda = NULL;
        pagNova = (PontPag) malloc(sizeof(Pagina));
        pagNova->Pt = Interna;
        pagNova->UU.U0.ni = 1;
        pagNova->UU.U0.ri[0] = x.chave;
        pagEsquerda = (Pagina*) malloc(sizeof(Pagina));
        pagNova->UU.U0.pi[0] = pagEsquerda;
        pagEsquerda->Pt = Externa;
        pagEsquerda->UU.U1.ne = 0;
        pagDireita = (Pagina*) malloc(sizeof(Pagina));
        pagNova->UU.U0.pi[1] = pagDireita;
        pagDireita->Pt = Externa;
        pagDireita->UU.U1.ne = 1;
        pagDireita->UU.U1.re[0] = x;
        *arvore = pagNova;
        return;
    }
    //Caso nao, Prosseguir o procedimento
    short Cresceu = 0;
    Chave ChaveRetorno;
    Pagina *pagRetorno;
    //Funcao auxiliar para Insercao
    InsereAux(x, *arvore, &Cresceu, &ChaveRetorno, &pagRetorno);
    comparacoes_index_abe++;
    if(Cresceu){
        Pagina *pagTemp = (PontPag) malloc(sizeof(Pagina));
        pagTemp->Pt = Interna;
        pagTemp->UU.U0.ni = 1;
        pagTemp->UU.U0.ri[0] = ChaveRetorno;
        pagTemp->UU.U0.pi[1] = pagRetorno;
        pagTemp->UU.U0.pi[0] = *arvore;
        *arvore = pagTemp;
    }
    return;
}

int abe(int quantidade, char* filename, int chaveBusca, int *n_comparacoes_index_abe, int *n_transferencias_index_abe, int *n_comparacoes_pesquisa_abe, int *n_transferencias_pesquisa_abe, double *tempo_index_abe, double *tempo_pesquisa_abe, int p_flag){
    PontPag arvore;
    FILE *arq;
    TRegistro x;
    int pos, cont;

    if((arq = fopen(filename,"rb")) == NULL){
        printf("Erro na abertura do arquivo\n");
        return 0;
    }


    clock_t start, end;

    start = clock();
    int i = 0;
    arvore == NULL;
    while(fread(&x, sizeof(x), 1, arq) == 1 && i<quantidade){
        transferencias_index_abe++;
        if(p_flag) printf("%d ",x.chave);
        _insere_abe(x, &arvore);
        i++;
    }

    x.chave = chaveBusca;

    end = clock();
    *tempo_index_abe = ((double) (end - start)) / CLOCKS_PER_SEC;

    start = clock();
    if(pesquisa_abe(&x, &arvore)){
        printf("Item de chave %d foi localizado\n",x.chave);
        *n_transferencias_index_abe = transferencias_index_abe;
        *n_comparacoes_index_abe = comparacoes_index_abe;
        *n_transferencias_pesquisa_abe = transferencias_pesquisa_abe;
        *n_comparacoes_pesquisa_abe = comparacoes_pesquisa_abe;
        fclose(arq);

        end = clock();
        *tempo_pesquisa_abe = ((double) (end - start)) / CLOCKS_PER_SEC;
        return 1;
    }else{
        printf("Item de chave %d nao foi localizado\n",x.chave);
        *n_transferencias_index_abe = transferencias_index_abe;
        *n_comparacoes_index_abe = comparacoes_index_abe;
        *n_transferencias_pesquisa_abe = transferencias_pesquisa_abe;
        *n_comparacoes_pesquisa_abe = comparacoes_pesquisa_abe;
        fclose(arq);

        end = clock();
        *tempo_pesquisa_abe = ((double) (end - start)) / CLOCKS_PER_SEC;
        return 0;
    }
}