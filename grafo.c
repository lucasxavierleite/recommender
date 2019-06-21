#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grafo.h"
#include "filme.h"
#include "erro.h"
#include "aux.h"

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
	ARESTA *inicio;
	unsigned n_relacionados;
} VERTICE;

struct grafo {
	VERTICE *adj;
	DICIONARIO *dicionario_sinopse;
	unsigned n_vertices;
};

FILME_PESOS *grafo_calcula_peso(GRAFO *grafo, VERTICE v1, VERTICE v2){
	FILME_PESOS *p = (FILME_PESOS *) malloc(sizeof(FILME_PESOS));
	p->nome = filme_calcula_peso_nome(v1.filme, v2.filme);
	p->ano = filme_calcula_peso_ano(v1.filme, v2.filme);
	p->generos = filme_calcula_peso_generos(v1.filme, v2.filme);
	p->sinopse = filme_calcula_peso_sinopse(grafo->dicionario_sinopse, v1.filme, v2.filme);
	p->media = (p->nome * PONDERACAO_NOME + p->ano * PONDERACAO_ANO + p->generos * PONDERACAO_GENEROS + p->sinopse * PONDERACAO_SINOPSE) /
		(PONDERACAO_NOME + PONDERACAO_ANO + PONDERACAO_GENEROS + PONDERACAO_SINOPSE);
	return p;
}

void grafo_adicionar_vertice(GRAFO *grafo, FILME *filme) {
	if(grafo) {
		grafo->adj = realloc(grafo->adj, (grafo->n_vertices + 1) * sizeof(VERTICE));
		grafo->adj[grafo->n_vertices].filme = filme; 
		grafo->adj[grafo->n_vertices].inicio = (ARESTA *) malloc(sizeof(ARESTA));
		grafo->adj[grafo->n_vertices].inicio->vertice = -1;
		grafo->adj[grafo->n_vertices].inicio->proximo = NULL;
		grafo->adj[grafo->n_vertices].n_relacionados = 0;
		grafo->n_vertices++;
	}
}

void grafo_calcula_arestas(GRAFO *grafo, int iv){
	if(grafo) {
		for(int i = 0; i < grafo->n_vertices; i++) {
			if(i == iv)
				continue;

			FILME_PESOS *pesos = grafo_calcula_peso(grafo, grafo->adj[iv],grafo->adj[i]);

			if(pesos->media < PESO_MINIMO)
				continue;

			ARESTA *p = grafo->adj[iv].inicio;

			while(p->proximo && p->pesos->media > pesos->media)
				p = p->proximo;

			ARESTA *a = (ARESTA *) malloc(sizeof(ARESTA));
			a->vertice = i;
			a->pesos = pesos;
			a->proximo = p->proximo;
			p->proximo = a;

			grafo->adj[iv].n_relacionados++;
		}
	}
}

void grafo_imprimir_vertice(GRAFO *grafo, VERTICE *vertice) {
	if(grafo) {
		ARESTA *p = vertice->inicio;

		while(p->proximo) {
			filme_imprimir_lista(grafo->adj[p->proximo->vertice].filme);
			printf(" | %%%.2lf relevante\n", p->proximo->pesos->media * 100);
			p = p->proximo;
		}
	}
}

int grafo_comparar_vertice_nome(const void *a, const void *b) {
	VERTICE *A = (VERTICE *) a;
	VERTICE *B = (VERTICE *) b;

	return strcasecmp(filme_nome(A->filme), filme_nome(B->filme));
}

VERTICE *grafo_buscar_vertice(GRAFO *grafo, char *nome){
	/*qsort(grafo->adj, grafo->n_vertices, sizeof(VERTICE), grafo_comparar_vertice_nome);*/
	/*return bsearch(nome, grafo->adj, grafo->n_vertices, sizeof(VERTICE), grafo_comparar_vertice_nome);*/
	for(int i = 0; i < grafo->n_vertices; i++)
		if(comparar_nome(nome, filme_nome(grafo->adj[i].filme)) == 0)
			return &grafo->adj[i];

	return NULL;
}

void grafo_buscar(GRAFO *grafo, char *nome) {
	VERTICE *res = grafo_buscar_vertice(grafo, nome);

	if(res) {
		filme_imprimir(res->filme);
	} else {
		erro_filme_nao_encontrado();
	}
}

void grafo_recomendar(GRAFO *grafo, char *nome) {
	VERTICE *res = grafo_buscar_vertice(grafo, nome);

	if(res) {
		if(res->n_relacionados > 0) {
			printf("\n> Talvez você goste de: \n\n");
			grafo_imprimir_vertice(grafo, res);
		} else {
			erro_filme_sem_recomendacoes();
		}
	} else {
		erro_filme_nao_encontrado();
	}
}

GRAFO *grafo_criar(FILE *arquivo) {
	GRAFO *grafo = (GRAFO *) malloc(sizeof(GRAFO));
	grafo->dicionario_sinopse = dicionario_criar();

	if(grafo) {
		grafo->adj = NULL;
		grafo->n_vertices = 0;

		while(!feof(arquivo)) {

			if(feof(arquivo))
				break;

			FILME *filme = filme_ler_arquivo(arquivo, grafo->dicionario_sinopse);
			grafo_adicionar_vertice(grafo, filme);
		}
	}

	for(int i = 0; i < grafo->n_vertices; i++)
		grafo_calcula_arestas(grafo, i);

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
