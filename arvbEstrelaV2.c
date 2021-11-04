#include <stdio.h>
#include <stdlib.h>
#include "arvbEstrelaV2.h"



//AS ALTERACOES FEITAS FORAM REFAZER OS METODOS DE INSERCAO DAS PAGINAS INTERNAS, EXCLUINDO TOTALMENTE A FUNCAO insereNaPag QUE O PROFESSOR TINHA FEITO
//AI FALTA ADAPTAR O CODIGO PARA O REGISTRO DO TIPO CERTO E COLOCAR AS VARIAVEIS PRA CONTAR A QTE DE COMPARACOES E TRANSFERENCIAS
//O CODIGO TA "RUIM" PQ ACABEI TENDO QUE FAZER TUDO DO ZERO KKKKKK AI O QUE TU VER QUE DA PRA MELHORAR PODE MEXER


//Pesquisa quase identica ao slide, so mudou o tipo de retorno de void para Int
int pesquisa(Registro *x, PontPag *pagina){
    int i;
    PontPag Pag;
    Pag = *pagina;
    if((*pagina)->Pt == Interna){
        i = 1;
        while(i < Pag->UU.U0.ni && x->chave > Pag->UU.U0.ri[i-1])
            i++;
        if(x->chave < Pag->UU.U0.ri[i-1])
            return pesquisa(x, &Pag->UU.U0.pi[i-1]);
        else
            return pesquisa(x, &Pag->UU.U0.pi[i]);
    }
    i = 1;
    while(i < Pag->UU.U1.ne && x->chave > Pag->UU.U1.re[i-1].chave)
        i++;
    if(x->chave == Pag->UU.U1.re[i-1].chave){
        *x = Pag->UU.U1.re[i-1];
        return 1;
    }
    else
        return 0;
}   


//Outra parte da insercao da Arvore B para paginas internas (vulgo arvore B), identico ao slide
void InsereAuxArvB(PontPag pagina, short *Cresceu, Chave *ChaveRetorno, PontPag *pagRetorno){
    int i = 1;
    PontPag pagTemp;
    while(i < pagina->UU.U0.ni && *ChaveRetorno > pagina->UU.U0.ri[i-1])
        i++;
    if(*ChaveRetorno == pagina->UU.U0.ri[i-1]){
        *Cresceu = 0;
        return;
    }
    if(*ChaveRetorno < pagina->UU.U0.ri[i-1])
        i--;
    if(!*Cresceu)
        return;
    if(pagina->UU.U0.ni < M + M){
        for(int j = pagina->UU.U0.ni; j > i; j--){
            pagina->UU.U0.ri[j] = pagina->UU.U0.ri[j-1];
            pagina->UU.U0.pi[j+1] = pagina->UU.U0.pi[j];
        }
        pagina->UU.U0.ni++;
        pagina->UU.U0.ri[i] = *ChaveRetorno;
        pagina->UU.U0.pi[i+1] = *pagRetorno;
        *Cresceu = 0;
        return;
    }
    pagTemp = (PontPag) malloc(sizeof(Pagina));
    pagTemp->Pt = Interna;
    pagTemp->UU.U0.ni = 0;
    pagTemp->UU.U0.pi[0] = NULL;
     //Caso o registro a ser inserido seja menor ou igual a mediana, entrara na pagina a esquerda
    if(i < M+1){
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
        //Caso a chave Retorno esteja no meio da pagina, nao se muda o *ChaveRetorno, uma vez que ela "subira" para a prox chave
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
void InsereAux(Registro Reg, PontPag pagina, short *Cresceu, Chave *ChaveRetorno, PontPag *pagRetorno){
    int i = 1;
    //Ira percorrer a Estrutura da Arvore ate achar o local correto do registro
    if((pagina->Pt == Interna)){
        while(i < pagina->UU.U0.ni && Reg.chave > pagina->UU.U0.ri[i-1])
            i++;
        if(Reg.chave < pagina->UU.U0.ri[i-1])
            i--;
        //Recursao para acessar a proxima pagina
        InsereAux(Reg, pagina->UU.U0.pi[i], Cresceu, ChaveRetorno, pagRetorno);
        if(*Cresceu){
            //Insercao igual ArvB normal
            InsereAuxArvB(pagina, Cresceu, ChaveRetorno, pagRetorno);
        }
        return;
    }
    else{
        //ESSA PARTE DO CODIGO SE REFERE A INSERCAO NAS PAGINAS EXTERNAS
        if(pesquisa(&Reg, &pagina))
            //Nao faz sentido colocar um registro que ja existe
            return;
        //Verificar aonde o registro entrara na pagina externa
        while(i < pagina->UU.U1.ne && Reg.chave > pagina->UU.U1.re[i-1].chave)
            i++;
        if(Reg.chave < pagina->UU.U1.re[i-1].chave)
            i--;
        //Caso a pagina tenha menos de 2M itens
        if(pagina->UU.U1.ne < M + M){
            for(int j = pagina->UU.U1.ne; j > i; j--){
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
            //Caso o registro a ser inserido seja menor que a mediana, entrara na pagina a esquerda
            if(i < M + 1){
                for(int j = pagina->UU.U1.ne - 1; j >= M - 1; j--){
                    pagDireita->UU.U1.re[k] = pagina->UU.U1.re[j];
                    pagDireita->UU.U1.ne++;
                    k--;
                    pagina->UU.U1.re[j] = pagina->UU.U1.re[j-1];
                }
                pagina->UU.U1.ne = M;
                for(int j = M-1; j > i; j--)
                    pagina->UU.U1.re[j] = pagina->UU.U1.re[j-1];
                pagina->UU.U1.re[i] = Reg;
                *Cresceu = 1;
                *ChaveRetorno = pagDireita->UU.U1.re[0].chave;
                *pagRetorno = pagDireita;
                //Subir a mediana e passar a referencia
            }
            else{
                //Caso o registro a ser inserido seja maior ou igual a mediana, entrara na pagina a direita
                for(int j = pagina->UU.U1.ne - 1; j > M - 1; j--){
                    pagDireita->UU.U1.re[k] = pagina->UU.U1.re[j];
                    pagDireita->UU.U1.ne++;
                    k--;
                    pagina->UU.U1.re[j] = pagina->UU.U1.re[j-1];
                }
                pagina->UU.U1.ne = M;
                if(i - M != 0 && pagDireita->UU.U1.re[i-M].chave < Reg.chave){
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
void Insere(Registro x, PontPag *arvore){
    //Caso a arvore esteja vazia, iniciar a pagina Interna e as paginas externas
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
