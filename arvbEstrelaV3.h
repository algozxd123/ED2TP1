#ifndef ARVBESTRELA_H
#define ARVBESTRELA_H

#define M 2

typedef int Chave;
typedef struct{
    Chave chave;
}Registro;

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
            Registro re[M + M];
        } U1;
    } UU;
    
}Pagina;

int pesquisa(Registro *x, PontPag *pagina);
//INSERCAO PAGINAS INTERNAS
void InsereAuxArvB(PontPag pagina, short *Cresceu, Chave *ChaveRetorno, PontPag *pagRetorno);
//INSERCAO PAGINAS EXTERNAS
void InsereAux(Registro Reg, PontPag pagina, short *Cresceu, Chave *ChaveRetorno, PontPag *pagRetorno);
void Insere(Registro x, PontPag *arvore);


#endif