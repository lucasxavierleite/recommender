#include <stdio.h>
#include "grafo.h"
#include "aux.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
	FILE *arquivo = fopen(argv[1], "r");

	if(!arquivo) {
		printf("Erro ao abrir o arquivo de dados \"%s\"\n", argv[0]);
		return -1;
	}

	GRAFO *grafo = grafo_criar(arquivo);
	
	char nome_filme[255];
	scanf("%[^\n]", nome_filme);

	grafo_recomendar(grafo, nome_filme);
	grafo_liberar(grafo);

	fclose(arquivo);

	return 0;
}
