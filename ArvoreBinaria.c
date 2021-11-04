#include "ArvoreBinaria.h"

int transferencias_index_abin = 0;
int transferencias_pesquisa_abin = 0;
int comparacoes_index_abin = 0;
int comparacoes_pesquisa_abin = 0;

int pesquisa_rec(No* pNo, int c, item* it) { // Pesquisa pelos nós da árvore
	if (pNo == NULL) return 0;
	if (pNo->valor.chave < c) { //Valor menor, ir para esquerda

		comparacoes_pesquisa_abin++;

		return pesquisa_rec(pNo->pEsq, c, it);

	} else if (pNo->valor.chave > c) { // Valor maior, ir para direita

		comparacoes_pesquisa_abin++;

		return pesquisa_rec(pNo->pDir, c, it);

	} else { // Valor igual, retornar 1 para confirmar o sucesso e a chave pelo ponteiro it
		it->chave = pNo->valor.chave;
		it->pagina = pNo->valor.pagina;
		return 1;
	}
}

int pesquisa_abin(ArvoreBinaria* pAB, int c, item* it) { //Pesquisa pela árvore, começando da raiz
	return pesquisa_rec(pAB->pRaiz, c, it);
}

int pesquisa_Sequencial(Registro * pagina, Registro* reg, int tam) { // Pesquisa sequencial 

	for (int i = 0; i < tam; i++) {
		
		comparacoes_pesquisa_abin++;
		
		if (pagina[i].chave == reg->chave) {

			reg->chave = pagina[i].chave;
			reg->dado1 = pagina[i].dado1;
			strcpy(reg->dado2, pagina[i].dado2);
			strcpy(reg->dado3, pagina[i].dado3);

			return 1;
		}
	}
	return 0; // não encontrado
}

void desalocaSubarvore(No* pN) { //Desaloca os ponteiros que constituem a árvore recursivamente
	if (pN == NULL) {
		return;
	} else {
		desalocaSubarvore(pN->pEsq);
		desalocaSubarvore(pN->pDir);
		free(pN);
	}
}

void destroi(ArvoreBinaria* pAB) { // Destruição da árvore a partir de sua raiz
	assert(pAB);
	desalocaSubarvore(pAB->pRaiz);
}

int insere_abin(ArvoreBinaria * pAB, item *it) { //Inserção dos nós em uma árvore

	comparacoes_index_abin++;
	if (pAB->pRaiz == NULL) { //Se a árvore é vazia, criar o nó raiz
		pAB->pRaiz = (No *) calloc(1, sizeof (No));
		pAB->pRaiz->valor.chave = it->chave;
		pAB->pRaiz->valor.pagina = it->pagina;
		return 1;
	}
	No* pPai = pAB->pRaiz; //Criação de variável que irá percorrer a árvore até a folha em que o registro será inserido
	int verdade = 0;
	comparacoes_index_abin++;
	while (!verdade) {
		comparacoes_index_abin++;
		if (pPai->valor.chave < it->chave) {
			
			comparacoes_index_abin++;
			if (pPai->pEsq == NULL)
				verdade = 1;
			else
				pPai = pPai->pEsq;

		} else {
			comparacoes_index_abin++;
			if (pPai->valor.chave > it->chave) {
				comparacoes_index_abin++;
				if (pPai->pDir == NULL)
					verdade = 1;
				else
					pPai = pPai->pDir;

			} else return 0;
		}
	}
	comparacoes_index_abin++;
	if (pPai->valor.chave < it->chave) { // Chegou em um nó folha ou com vaga pra filho, decidir onde irá ser colocado

		pPai->pEsq = (No *) calloc(1, sizeof (No));
		pPai->pEsq->valor.chave = it->chave;
		pPai->pEsq->valor.pagina = it->pagina;


	} else {
		pPai->pDir = (No *) calloc(1, sizeof (No));
		pPai->pDir->valor.chave = it->chave;
		pPai->pDir->valor.pagina = it->pagina;
	}
	return 1;
}

int ArvoreBin(int quantidade, char *filename, int chaveBusca, int *n_comparacoes_index_abin, int *n_transferencias_index_abin, int *n_comparacoes_pesquisa_abin, int *n_transferencias_pesquisa_abin, double *tempo_index_abin, double *tempo_pesquisa_abin, int p_flag) {	
	ArvoreBinaria pAB; //Criação da árvore vazia
	pAB.pRaiz = NULL;

	Registro aux[TAM_PAGINA];
	item it[TAM_PAGINA];

	clock_t start, end;
	
	start = clock();

	FILE *fp;
	if((fp = fopen(filename,"rb")) == NULL){ // Leitura do arquivo binário
        printf("Erro na abertura do arquivo\n");
        return 0;
    }

	int j = 0;
	int n_pages = quantidade / TAM_PAGINA;
	int resto = quantidade % TAM_PAGINA;
	while (fread(&aux, sizeof (Registro), TAM_PAGINA, fp) == TAM_PAGINA && j < n_pages) { //Leitura das páginas 
		comparacoes_index_abin++;
		for (int i = 0; i < TAM_PAGINA; i++) { //Leitura dos itens nas páginas
			comparacoes_index_abin++;
			if(p_flag) printf("%d ",aux[i].chave);
			it[i].chave = aux[i].chave;
			it[i].pagina = j;
			insere_abin(&pAB, &it[i]); //Inserção dos valores na página
		}
		j++;
		transferencias_index_abin++;
	}
	
	comparacoes_index_abin++;
	if (resto != 0) { //Leitura da última página caso não esteja completa

		fread(&aux, sizeof (Registro), resto, fp);

		for (int i = 0; i < resto; i++) {
			comparacoes_index_abin++;
			if(p_flag) printf("%d ",aux[i].chave);
			it[i].chave = aux[i].chave;
			it[i].pagina = j;
			insere_abin(&pAB, &it[i]);
		}

		transferencias_index_abin++;

	} else
		resto = TAM_PAGINA;

	end = clock();
    *tempo_index_abin = ((double) (end - start)) / CLOCKS_PER_SEC;

	start = clock();
	item res;
	if (pesquisa_abin(&pAB, chaveBusca, &res)) { //Feita a pesquisa binária em árvore na memória principal, se descobre a
						     //posição dos dados em memória secundária , e uma pesquisa sequencial é feita lá.

		long deslocamento;
		deslocamento = sizeof (Registro) * TAM_PAGINA * (res.pagina);

		fseek(fp, deslocamento, SEEK_SET);
		fread(&aux, sizeof (Registro), resto, fp);

		transferencias_pesquisa_abin++; // numero de transf

		Registro reg;
		reg.chave = res.chave;        
		
		comparacoes_pesquisa_abin++;
		if (res.pagina == resto / TAM_PAGINA) { //Pesquisa sequencial dentro da página avaliando se a mesma está completa ou incompleta
			comparacoes_pesquisa_abin++;
			if (pesquisa_Sequencial(aux, &reg, resto)) {
				*n_transferencias_index_abin = transferencias_index_abin;
				*n_comparacoes_index_abin = comparacoes_index_abin;
				*n_transferencias_pesquisa_abin = transferencias_pesquisa_abin;
				*n_comparacoes_pesquisa_abin = comparacoes_pesquisa_abin;

				printf("Item de chave %d foi localizado\n",reg.chave);

				destroi(&pAB);
				end = clock();
    			*tempo_pesquisa_abin = ((double) (end - start)) / CLOCKS_PER_SEC;
				return 1;
			}

		} else { // Caso da chave não ser encontrada dentro da estrutura da árvore binária
			comparacoes_pesquisa_abin++;
			if (pesquisa_Sequencial(aux, &reg, TAM_PAGINA)) {
				*n_transferencias_index_abin = transferencias_index_abin;
				*n_comparacoes_index_abin = comparacoes_index_abin;
				*n_transferencias_pesquisa_abin = transferencias_pesquisa_abin;
				*n_comparacoes_pesquisa_abin = comparacoes_pesquisa_abin;

				printf("Item de chave %d foi localizado\n",reg.chave);

				destroi(&pAB);

				end = clock();
    			*tempo_pesquisa_abin = ((double) (end - start)) / CLOCKS_PER_SEC;
				return 1;
			}

		}
	} else {
		printf("Item de chave %d nao foi localizado\n",chaveBusca);
		
		destroi(&pAB);
		*n_transferencias_index_abin = transferencias_index_abin;
		*n_comparacoes_index_abin = comparacoes_index_abin;
		*n_transferencias_pesquisa_abin = transferencias_pesquisa_abin;
		*n_comparacoes_pesquisa_abin = comparacoes_pesquisa_abin;

		end = clock();
    	*tempo_pesquisa_abin = ((double) (end - start)) / CLOCKS_PER_SEC;
		return 0;
	}

}
