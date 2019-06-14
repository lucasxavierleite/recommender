#include <stdio.h>
#include "grafo.h"
#include "aux.h"

int main(int argc, char *argv[]) {
	FILE *arquivo = fopen(argv[1], "r");

	if(!arquivo) {
		printf("Erro ao abrir o arquivo de dados \"%s\"\n", argv[0]);
		return -1;
	}

	GRAFO *grafo = grafo_criar(arquivo);
	grafo_imprimir_filmes(grafo);
	grafo_liberar(grafo);

	fclose(arquivo);

	return 0;
}
