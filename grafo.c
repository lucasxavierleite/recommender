#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct pesos {
	double nome;
	double sinopse;
	double genero;
	double ano;

} PESOS;

typedef struct vertice {
	char *nome;
	int n_generos;
	char **generos;
	char *sinopse;
	int ano;
} VERTICE;

typedef struct aresta {
	PESOS pesos;
	VERTICE *proximo;
} ARESTA;

struct grafo {
	VERTICE *adj;
	unsigned int n_vertices;
};

VERTICE *grafo_criar_vertice(char *nome, int ano, char *generos, char *sinopse) {
	VERTICE *vertice = (VERTICE *) malloc(sizeof(VERTICE));

	vertice->nome = (char *) malloc(sizeof(char) * strlen(nome) + 1);
	strcpy(vertice->nome, nome);

	vertice->ano = ano;

	vertice->generos = NULL;
	char *token = strtok(generos, ",\n");

	int i = 0;
	while((token)) {
		vertice->generos = (char **) realloc(vertice->generos, i + 1);
		vertice->generos[i] = (char *) malloc(strlen(token) + 1);
		strcpy(vertice->generos[i], token);
		token = strtok(NULL, ",\n");
		i++;
	}

	free(token);

	vertice->n_generos = i;

	vertice->sinopse = (char *) malloc(sizeof(char) * strlen(sinopse) + 1);
	strcpy(vertice->sinopse, sinopse);

	return vertice;
}

void grafo_liberar_vertice(VERTICE *vertice) {
	free(vertice->nome);

	for(int i = 0; i < vertice->n_generos; i++) {
		free(vertice->generos[i]);
	}

	free(vertice->generos);
	free(vertice->sinopse);
	free(vertice);
}

void filme_imprimir(VERTICE *vertice) {
	printf("nome: %s\n", vertice->nome);
	printf("ano: %d\n", vertice->ano);
	printf("generos: ");

	for(int i = 0; i < vertice->n_generos; i++) {
		printf("%s", vertice->generos[i]);
		if(i + 1 < vertice->n_generos)
			printf(", ");
	}

	printf("\n");
	printf("sinopse: %s\n", vertice->sinopse);
}

VERTICE *ler_arquivo_filme(FILE *arquivo) {
	char lixo[50];
	char nome[100];
	int ano;
	char generos[200];
	char sinopse[5000];

	fscanf(arquivo, "%s %[^\n]", lixo, nome);
	fscanf(arquivo, "%s %d",  lixo, &ano);
	fscanf(arquivo, "%s %[^\n]", lixo, generos);
	fscanf(arquivo, "%s %[^\n] ", lixo, sinopse);

	return grafo_criar_vertice(nome, ano, generos, sinopse);
}

void ler_arquivo(FILE *arquivo) {
	while(!feof(arquivo)) {
		if(feof(arquivo))
			break;

		VERTICE *vertice = ler_arquivo_filme(arquivo);
		filme_imprimir(vertice);
		printf("\n");
		grafo_liberar_vertice(vertice);
	}
}
