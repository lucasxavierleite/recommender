#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grafo.h"
#include "filme.h"
#include "aux.h"

typedef struct pesos {
	double nome;
	double sinopse;
	double genero;
	double ano;
} FILME_PESOS;

typedef struct aresta {
	unsigned int vertice;
	FILME_PESOS pesos;
	struct aresta *proximo;
} ARESTA;

typedef struct vertice {
	FILME *filme;
	ARESTA *inicio, *fim;
} VERTICE;

struct grafo {
	VERTICE *adj;
	unsigned int n_vertices;
};

void grafo_adicionar_vertice(GRAFO *grafo, FILME *filme) {
	if(grafo) {
		grafo->adj = realloc(grafo->adj, (grafo->n_vertices + 1) * sizeof(VERTICE));
		grafo->adj[grafo->n_vertices].filme = filme;
		grafo->adj[grafo->n_vertices].inicio = NULL;
		grafo->adj[grafo->n_vertices].fim = NULL;
		grafo->n_vertices++;
	}
}

GRAFO *grafo_criar(FILE *arquivo) {
	GRAFO *grafo = (GRAFO *) malloc(sizeof(GRAFO));

	if(grafo) {
		grafo->adj = NULL;
		grafo->n_vertices = 0;

		while(!feof(arquivo)) {

			if(feof(arquivo))
				break;

			FILME *filme = filme_ler_arquivo(arquivo);
			grafo_adicionar_vertice(grafo, filme);
		}
	}

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
