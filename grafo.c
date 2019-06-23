#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grafo.h"
#include "filme.h"
#include "interface.h"
#include "aux.h"

/*
*  Struct pesos
*    double nome  :  resultado do cálculo dos pesos do nome do filme
*    double sinopse  :  resultado do cálculo dos pesos da sinopse do filme
*    double generos  :  resultado do cálculo dos pesos dos gêneros do filme
*    double ano  :  resultado do cálculo dos pesos do ano de lançamento do filme
*    double media  :  media dos pesos
*
*/

typedef struct pesos {
	double nome;
	double sinopse;
	double generos;
	double ano;
	double media;
} FILME_PESOS;

/*
*  Struct aresta
*    unsigned vertice  :  vértice de "origem" da aresta
*    FILME_PESOS *pesos  :  struct com os pesos calculados entre os dois
*                           vértices que a aresta conecta
*    struct aresta *proximo  :  vértice na outra extremidade da aresta
*
*/

typedef struct aresta {
	unsigned vertice;
	FILME_PESOS *pesos;
	struct aresta *proximo;
} ARESTA;

/*
*  Struct vertice
*    FILME *filme  :  struct de um filme
*    ARESTA *inicio  : início da lista de adjacentes ao vértice (relacionados)
*    unsigned n_relacionados  :  número de relacionados
*    unsigned n_relacionados_sinopse  :   número de relacionados pela sinopse
*
*/

typedef struct vertice {
	FILME *filme;
	ARESTA *inicio;
	unsigned n_relacionados;
	unsigned n_relacionados_sinopse;
} VERTICE;

/*
*  Struct grafo
*    VERTICE *adj                   :  vértice
*    DICIONARIO *dicionario_sinopse :  dicionário contendo as palavras
*                                      armazenadas após a leitura das sinopses
*    unsigned n_vertices            :  número de vértices no grafo
*
*/

struct grafo {
	VERTICE *adj;
	DICIONARIO *dicionario_sinopse;
	unsigned n_vertices;
};

/*
*  Função para calcular pesos entre dois vértices do grafo (armazenado na aresta)
*
*  Parâmetros: 
*    GRAFO *grafo :  grafo em questão
*    VERTICE v1   :  vértice v1
*    VERTICE v2   :  vértice v2
*    DICIONARIO *stopwords  :  dicionario de stopwords 
*
*  Retorno:
*    retorna um struct com os pesos que será a aresta entre os vértices v1 e v2
*/

FILME_PESOS *grafo_calcula_peso(GRAFO *grafo, VERTICE v1, VERTICE v2, DICIONARIO *stopwords){
	FILME_PESOS *p = (FILME_PESOS *) malloc(sizeof(FILME_PESOS));
	p->nome = filme_calcula_peso_nome(stopwords, v1.filme, v2.filme);
	p->ano = filme_calcula_peso_ano(v1.filme, v2.filme);
	p->generos = filme_calcula_peso_generos(v1.filme, v2.filme);
	p->sinopse = filme_calcula_peso_sinopse(grafo->dicionario_sinopse, stopwords, v1.filme, v2.filme);
	p->media = (p->nome * PONDERACAO_NOME + p->ano * PONDERACAO_ANO + p->generos * PONDERACAO_GENEROS + p->sinopse * PONDERACAO_SINOPSE) /
		(PONDERACAO_NOME + PONDERACAO_ANO + PONDERACAO_GENEROS + PONDERACAO_SINOPSE);
	return p;
}

/*
*  Função para adicionar um vértice no grafo 
*
*  Parâmetros: 
*    GRAFO *grafo  :  grafo em questão
*    FILME filme  :  filme a ser inserido
* 
*/

void grafo_adicionar_vertice(GRAFO *grafo, FILME *filme) {
	if(grafo) {
		grafo->adj = realloc(grafo->adj, (grafo->n_vertices + 1) * sizeof(VERTICE));
		grafo->adj[grafo->n_vertices].filme = filme;
		grafo->adj[grafo->n_vertices].inicio = (ARESTA *) malloc(sizeof(ARESTA));
		grafo->adj[grafo->n_vertices].inicio->vertice = -1;
		grafo->adj[grafo->n_vertices].inicio->proximo = NULL;
		grafo->adj[grafo->n_vertices].n_relacionados = 0;
		grafo->adj[grafo->n_vertices].n_relacionados_sinopse = 0;
		grafo->n_vertices++;
	}
}

/*
*  Função para calcular pesos das arestas entre o vértice de índice "iv" e os outros do grafo
*
*  Parâmetros: 
*    GRAFO *grafo  :  grafo em questão
*    int iv  :  índice do vértice
*    DICIONARIO *stopwords  :  dicionario de stopwords 
*/

void grafo_calcula_arestas(GRAFO *grafo, int iv, DICIONARIO *stopwords){
	if(grafo) {
		for(int i = 0; i < grafo->n_vertices; i++) {
			if(i == iv)
				continue;

			FILME_PESOS *pesos = grafo_calcula_peso(grafo, grafo->adj[iv],grafo->adj[i], stopwords);

			ARESTA *p = grafo->adj[iv].inicio;

			while(p->proximo && p->proximo->pesos->media > pesos->media)
				p = p->proximo;

			ARESTA *a = (ARESTA *) malloc(sizeof(ARESTA));
			a->vertice = i;
			a->pesos = pesos;
			a->proximo = p->proximo;
			p->proximo = a;

			if(pesos->sinopse > PESO_MINIMO)
				grafo->adj[iv].n_relacionados_sinopse++;

			if(pesos->media > PESO_MINIMO)
				grafo->adj[iv].n_relacionados++;
		}
	}
}

/*
*  Função para imprimir na tela as informações de vértice de índice iv
*
*  Parâmetros: 
*    GRAFO *grafo  :  grafo em questão
*    VERTICE *vertice  :  vertice em questão
*    int peso_base  :  valor do peso que será comparado para ver se entra nas
*                      recomendações
*/

void grafo_imprimir_vertice(GRAFO *grafo, VERTICE *vertice, int peso_base) {
	if(grafo) {
		ARESTA *p = vertice->inicio;

		while(p->proximo) {
			double relevancia;

			if(peso_base == RECOMENDACAO_GERAL) {
				if(p->proximo->pesos->media <= PESO_MINIMO) break;
				relevancia = p->proximo->pesos->media * 100;
			} else if(peso_base == RECOMENDACAO_SINOPSE) {
				relevancia = p->proximo->pesos->sinopse * 100;
			} else if(peso_base == RECOMENDACAO_NAO_RELACIONADOS) {
				if(p->proximo->pesos->media <= PESO_MINIMO) {
					relevancia = p->proximo->pesos->media * 100;
					filme_imprimir_lista(grafo->adj[p->proximo->vertice].filme);
					printf(ANSI_COR_PRETO_BRILHANTE "  | %.2lf%% relevante\n" ANSI_COR_RESET, relevancia);
					p = p->proximo;
					continue;
				}
			}

			filme_imprimir_lista(grafo->adj[p->proximo->vertice].filme);
			printf(ANSI_COR_PRETO_BRILHANTE "  | %.2lf%% relevante\n" ANSI_COR_RESET, relevancia);

			p = p->proximo;
		}

		printf("\n");
	}
}

/*
*  Lista todos os filmes de determinado gênero
*
*  Parâmetros: 
*    GRAFO *grafo : grafo que representa o conjunto de filmes
*    char *genero : gênero buscado
*/

void grafo_recomendar_filmes_genero(GRAFO *grafo, char *genero) {
	printf("\n");

	int encontrados = 0;

	for(int i = 0; i < grafo->n_vertices; i++){
		char v_e[filme_n_generos(grafo->adj[i].filme)][51];

		for(int j = 0; j < filme_n_generos(grafo->adj[i].filme); j++)
			strcpy(v_e[j], filme_generos(grafo->adj[i].filme)[j]);

		if(buscar(genero, filme_n_generos(grafo->adj[i].filme), sizeof(char)*51, v_e)) {
			filme_imprimir_lista(grafo->adj[i].filme);
			printf("\n");
			encontrados++;
		}
	}

	if(encontrados == 0)
		erro_genero_nao_encontrado();

	printf("\n");
}

/*
*  Compara dois vértices pelo nome do filme (case insensitive)
*
*  Parâmetros: 
*    const void *a : ponteiro genérico para a primeiro vértice a ser comparado
*    const void *b : ponteiro genérico para a segundo vértice a ser comparado
*
*  Retorno:
*   Inteiro < 0, caso 'a' entre primeiro pela ordem alfabética;
*           = 0, caso 'a' e 'b' sejam iguais
*           > 0, caso 'b' entre primeiro pela ordem alfabética;
*/

int grafo_comparar_vertice_nome(const void *a, const void *b) {
	VERTICE *A = (VERTICE *) a;
	VERTICE *B = (VERTICE *) b;

	return strcasecmp(filme_nome(A->filme), filme_nome(B->filme));
}

/*
*  Função para procurar um vértice de determinado nome no grafo
*
*  Parâmetros: 
*    GRAFO *grafo  :  grafo em questão
*    char *nome  :  nome do vértice procurado
*
*  Retorno:
*    retorna o vértice desejado, caso o encontre
*/

VERTICE *grafo_buscar_vertice(GRAFO *grafo, char *nome) {
	for(int i = 0; i < grafo->n_vertices; i++) {
		if(comparar_nome(nome, filme_nome(grafo->adj[i].filme)) < 0)
			break;
		else if(comparar_nome(nome, filme_nome(grafo->adj[i].filme)) == 0)
			return &grafo->adj[i];
	}

	return NULL;
}

/*
*  Função para imprimir na tela informações do filme de determinado nome depois
*  de uma busca pelo grafo
*
*  Parâmetros: 
*    GRAFO *grafo  :  grafo em questão
*    char *nome  :  nome do vértice procurado
* 
*/

void grafo_buscar(GRAFO *grafo, char *nome) {
	VERTICE *res = grafo_buscar_vertice(grafo, nome);

	if(res) {
		printf("\n");
		filme_imprimir(res->filme);
		printf("\n\n");
	} else {
		erro_filme_nao_encontrado();
	}
}

/*
*  Função principal para o cálculo das recomendações entre os filmes
*
*  Parâmetros: 
*    GRAFO *grafo   :  grafo em questão
*    char *nome     :  nome do filme digitado pelo usuário
*    int peso_base  :  valor do peso que será comparado para ver se entra nas
*                      recomendações
*/

void grafo_recomendar(GRAFO *grafo, char *nome, int peso_base) {
	VERTICE *res = grafo_buscar_vertice(grafo, nome);

	if(res) {
		if( (peso_base == RECOMENDACAO_SINOPSE && res->n_relacionados_sinopse > 0) ||
			(peso_base == RECOMENDACAO_GERAL && res->n_relacionados > 0)) {
			printf(ANSI_COR_PRETO_BRILHANTE "\n> Talvez você goste de: \n\n" ANSI_COR_RESET);
			grafo_imprimir_vertice(grafo, res, peso_base);
		} else if(peso_base == RECOMENDACAO_NAO_RELACIONADOS ){
			printf(ANSI_COR_PRETO_BRILHANTE "\n> Você provavelmente não gostará de: \n\n" ANSI_COR_RESET);
			grafo_imprimir_vertice(grafo, res, peso_base);
		} else {
			erro_filme_sem_recomendacoes();
		}
	} else {
		erro_filme_nao_encontrado();
	}
}

/*
*  Verifica a relação entre dois filmes
*
*  Parâmetros: 
*    GRAFO *grafo  : grafo que representa o conjunto de filmes
*    char *nome_f1 : nome do primeiro filme
*    char *nome_f2 : nome do segundo filme
*/

void grafo_verificar_relacao(GRAFO *grafo, char *nome_f1, char *nome_f2) {
	VERTICE *v1 = grafo_buscar_vertice(grafo, nome_f1);
	VERTICE *v2 = grafo_buscar_vertice(grafo, nome_f2);

	if(v1 && v2) {
		ARESTA *p = v1->inicio;

		while(p->proximo) {
			if(grafo_comparar_vertice_nome(v2, &grafo->adj[p->proximo->vertice]) == 0) {
				double relevancia = p->proximo->pesos->media * 100;
				printf(ANSI_COR_PRETO_BRILHANTE "\nOs filmes são %.2lf%% similares\n\n" ANSI_COR_RESET, relevancia);
				break;
			}
			p = p->proximo;
		}
	} else {
		erro_filme_nao_encontrado();
	}
}

/*
*  Função para inicializar e fazer alocação dinâmica para um grafo
*
*  Parâmetros: 
*    FILE *arquivo  :  ponteiro do arquivo
*
*  Retorno:
*   retorna um grafo inicializado
*/

GRAFO *grafo_criar(FILE *arquivo) {
	GRAFO *grafo = (GRAFO *) malloc(sizeof(GRAFO));
	grafo->dicionario_sinopse = dicionario_criar();
	DICIONARIO *stopwords = dicionario_criar_vetor(vetor_stopwords_global, 220);

	if(grafo) {
		grafo->adj = NULL;
		grafo->n_vertices = 0;

		while(!feof(arquivo)) {

			if(feof(arquivo))
				break;

			FILME *filme = filme_ler_arquivo(arquivo, grafo->dicionario_sinopse, stopwords);
			grafo_adicionar_vertice(grafo, filme);
		}
	}

	qsort(grafo->adj, grafo->n_vertices, sizeof(VERTICE), grafo_comparar_vertice_nome);

	for(int i = 0; i < grafo->n_vertices; i++)
		grafo_calcula_arestas(grafo, i, stopwords);

	return grafo;
}

/*
*  Função que libera memória dinamicamente alocada para grafo
*
*  Parâmetros: 
*    GRAFO *grafo  :  grafo a ter memória liberada
*
*/

void grafo_liberar(GRAFO *grafo) {
	for(int i = 0; i < grafo->n_vertices; i++) {
		filme_liberar(grafo->adj[i].filme);

		ARESTA *proximo = grafo->adj[i].inicio;

		while(proximo) {
			ARESTA *aux = proximo;
			proximo = proximo->proximo;
			free(aux);
		}
	}

	free(grafo->adj);
	free(grafo);
}

/*
*  Função que imprime na tela os vértices do grafo que são filmes 
*
*  Parâmetros: 
*    GRAFO *grafo  :  grafo em questão
*
*/

void grafo_imprimir_filmes(GRAFO *grafo) {
	printf("\n");

	for(int i = 0; i < grafo->n_vertices; i++) {
		printf(ANSI_COR_PRETO_BRILHANTE "%d. " ANSI_COR_RESET, i + 1);
		filme_imprimir_lista(grafo->adj[i].filme);
		printf("\n");
	}

	printf("\n");
}
