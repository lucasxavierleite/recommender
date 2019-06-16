#ifndef GRAFO_H
#define GRAFO_H

typedef struct grafo GRAFO;

GRAFO *grafo_criar(FILE *arquivo);
void grafo_liberar(GRAFO *grafo);
void grafo_imprimir_filmes(GRAFO *grafo);
void grafo_recomendar(GRAFO* grafo, char* nome);

#endif

