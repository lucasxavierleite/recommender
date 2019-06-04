#include <stdio.h>
#include "grafo.h"

int main(int argc, char *argv[]) {
	FILE *arquivo = fopen("filmes.txt", "r");
	ler_arquivo(arquivo);
	fclose(arquivo);
	return 0;
}
