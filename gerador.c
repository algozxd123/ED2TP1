#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct{
    int chave;
    long dado1;
    char dado2[1000];
    char dado3[5000];
} reg;

void geradorArquivo(int quantidade){

    FILE *arq1 = fopen("arquivo_asc.bin","wb");
    FILE *arq2 = fopen("arquivo_desc.bin","wb");
    FILE *arq3 = fopen("arquivo_rand.bin","wb");
    reg r;

    int *chaves = (int*)malloc(sizeof(int) * quantidade);
    for(int i=0;i<quantidade;i++){
        chaves[i] = i+1;
    }

    int rand_index;
    int aux;
    for(int i=0;i<quantidade;i++){
        rand_index = rand() % quantidade;
        aux = chaves[i];
        chaves[i] = chaves[rand_index];
        chaves[rand_index] = aux;
    }

    for(int i=0;i<quantidade;i++){
        if(i%10000 == 0) printf("%d\n",i);
        r.dado1 = rand() % LONG_MAX;
        for(int j=0;j<1000;j++){
            r.dado2[j] = '!' + (rand() % 94);
        }
        for(int j=0;j<5000;j++){
            r.dado3[j] = '!' + (rand() % 94);
        }

        r.chave = i+1;
        fwrite(&r, sizeof(r), 1, arq1);

        r.chave = quantidade - i;
        fwrite(&r, sizeof(r), 1, arq2);

        r.chave = chaves[i];
        fwrite(&r, sizeof(r), 1, arq3);
    }

    fclose(arq1);
    fclose(arq2);
    fclose(arq3);
}

void lerArquivo(){
    FILE *arq = fopen("arquivo_rand.bin","rb");
    int index[] = {17650, 182550, 221440, 339086, 511841, 597810, 704325, 890901, 903550, 971005};
    reg x;
    int i=0;
    while(fread(&x, sizeof(x), 1, arq) == 1){
        for(int j=0;j<10;j++){
            if(i == index[j]){
                printf("%d,",x.chave);
            }
        }            
        i++;
        if(i==1000000)
            return;
    }
    fclose(arq);
}

int main(){
    //geradorArquivo(1000000);
    lerArquivo();
    return 0;
}