#ifndef GRAFO_H
#define GRAFO_H

#define PONDERACAO_ANO 1
#define PONDERACAO_SINOPSE 2
#define PONDERACAO_NOME 3
#define PONDERACAO_GENEROS 4
#define PESO_MINIMO 0.4

typedef struct grafo GRAFO;

GRAFO *grafo_criar(FILE *arquivo);
void grafo_liberar(GRAFO *grafo);
void grafo_imprimir_filmes(GRAFO *grafo);
void grafo_buscar(GRAFO *grafo, char *nome);
void grafo_recomendar(GRAFO *grafo, char *nome);

#endif

