#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grafo.h"
#include "filme.h"
#include "aux.h"

#define PONDERACAO_ANO 1
#define PONDERACAO_SINOPSE 2
#define PONDERACAO_NOME 3
#define PONDERACAO_GENEROS 4
#define PESO_MINIMO 0.1

typedef struct pesos {
	double nome;
	double sinopse;
	double generos;
	double ano;
	double media;
} FILME_PESOS;

typedef struct aresta {
	unsigned vertice;
	FILME_PESOS *pesos;
	struct aresta *proximo;
} ARESTA;

typedef struct vertice {
	FILME *filme;
	ARESTA *inicio, *fim;
} VERTICE;

struct grafo {
	VERTICE *adj;
	DICIONARIO* dicionario_sinopse;
	unsigned n_vertices;
};

FILME_PESOS *grafo_calcula_peso(GRAFO* grafo, VERTICE v1, VERTICE v2, DICIONARIO* stopwords){
	/* FUNCAO CALLER PARA CALCULAR O PESO DE UMA ARESTA*/
	FILME_PESOS *p = (FILME_PESOS *) malloc(sizeof(FILME_PESOS));
	p->nome = filme_calcula_peso_nome(stopwords, v1.filme, v2.filme);
	p->ano = filme_calcula_peso_ano(v1.filme, v2.filme);
	p->generos = filme_calcula_peso_generos(v1.filme, v2.filme);
	p->sinopse = filme_calcula_peso_sinopse(grafo->dicionario_sinopse, stopwords, v1.filme, v2.filme);
	p->media = (p->nome * PONDERACAO_NOME + p->ano * PONDERACAO_ANO + p->generos * PONDERACAO_GENEROS + p->sinopse * PONDERACAO_SINOPSE) /
		(PONDERACAO_NOME + PONDERACAO_ANO + PONDERACAO_GENEROS + PONDERACAO_SINOPSE);
	return p;
}

void grafo_adicionar_vertice(GRAFO *grafo, FILME *filme) {
	if(grafo) {
		grafo->adj = realloc(grafo->adj, (grafo->n_vertices + 1) * sizeof(VERTICE));
		grafo->adj[grafo->n_vertices].filme = filme; 
		grafo->adj[grafo->n_vertices].inicio = NULL; /*inicio lista de arestas*/
		grafo->adj[grafo->n_vertices].fim = NULL;/*fim lista de arestas*/
		grafo->n_vertices++;
	}
}

void grafo_calcula_arestas(DICIONARIO* stopwords, GRAFO *grafo, int iv){
	if(grafo) {
		for(int i = 0; i < grafo->n_vertices; i++) {
			if(i == iv)
				continue;

			FILME_PESOS *pesos = grafo_calcula_peso(grafo, grafo->adj[iv],grafo->adj[i], stopwords);

			if(pesos->media < PESO_MINIMO)
				continue;

			ARESTA *atual = NULL;
			ARESTA *proximo = grafo->adj[iv].inicio;

			/*while(proximo && proximo->pesos->media < pesos->media) {*/
			while(proximo) {
				atual = proximo;
				proximo = proximo->proximo;
			}

			ARESTA *a = (ARESTA *) malloc(sizeof(ARESTA));

			if(atual) {
				ARESTA *aux = atual->proximo;
				atual->proximo = a;
			} else {
				a->proximo = NULL;
				grafo->adj[iv].inicio = a;
			}

			a->vertice = i;
			a->pesos = pesos;
		}
	}

	return;
}

void grafo_imprimir_vertice(GRAFO *grafo, VERTICE *vertice) {
	if(grafo) {
		ARESTA *proximo = vertice->inicio;

		while(proximo) {
			filme_imprimir(grafo->adj[proximo->vertice].filme);
			proximo = proximo->proximo;
		}
	}
}

int grafo_comparar_vertice_nome(const void *a, const void *b) {
	VERTICE *A = (VERTICE *) a;
	VERTICE *B = (VERTICE *) b;

	return strcmp(filme_nome(B->filme), filme_nome(A->filme));
}

VERTICE *buscar_nome(GRAFO *grafo, char *nome){
	/*qsort(grafo->adj, grafo->n_vertices, sizeof(VERTICE), grafo_comparar_vertice_nome);*/
	/*return bsearch(nome, grafo->adj, grafo->n_vertices, sizeof(VERTICE), grafo_comparar_vertice_nome);*/
	for(int i = 0; i < grafo->n_vertices; i++)
		if(comparar_nome(nome, filme_nome(grafo->adj[i].filme)) == 0)
			return &grafo->adj[i];
}

void grafo_recomendar(GRAFO *grafo, char *nome){
	VERTICE *res = buscar_nome(grafo, nome);

	/*ARESTA *proximo = res->inicio;*/

	/*while(proximo) {*/
		/*filme_imprimir(grafo->adj[proximo->vertice].filme);*/
	/*}*/

	grafo_imprimir_vertice(grafo, res);
}

GRAFO *grafo_criar(FILE *arquivo) {
	GRAFO *grafo = (GRAFO *) malloc(sizeof(GRAFO));
	grafo->dicionario_sinopse = dicionario_criar();
	DICIONARIO* stopwords = dicionario_criar_vetor(vet_stopwords_global, 220);

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
	for(int i = 0; i < grafo->n_vertices; i++)
		grafo_calcula_arestas(stopwords, grafo, i);

	return grafo;
}

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

void grafo_imprimir_filmes(GRAFO *grafo) {
	for(int i = 0; i < grafo->n_vertices; i++) {
		printf("%d. ", i + 1);
		filme_imprimir_lista(grafo->adj[i].filme);
		printf("\n");
	}
}
