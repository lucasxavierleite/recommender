#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "filme.h"
#include "aux.h"

struct filme {
	char *nome;
	unsigned int n_generos;
	char **generos;
	char *sinopse;
	unsigned int ano;
};

FILME *filme_criar(char *nome, int ano, char *generos, char *sinopse) {
	FILME *filme = (FILME *) malloc(sizeof(FILME));

	filme->nome = (char *) malloc(sizeof(char) * strlen(nome) + 1);
	strcpy(filme->nome, nome);
	filme->ano = ano;
	filme->generos = NULL;

	char *token = strtok(generos, ",\n");

	int i = 0;
	while((token)) {
		filme->generos = (char **) realloc(filme->generos, i + 1);
		filme->generos[i] = (char *) malloc(strlen(token) + 1);
		trim(token);
		strcpy(filme->generos[i], token);
		token = strtok(NULL, ",\n");
		i++;
	}

	free(token);

	filme->n_generos = i;
	filme->sinopse = (char *) malloc(sizeof(char) * strlen(sinopse) + 1);
	strcpy(filme->sinopse, sinopse);

	return filme;
}

FILME *filme_ler_arquivo(FILE *arquivo) {
	char lixo[50];
	char nome[100];
	int ano;
	char generos[200];
	char sinopse[5000];

	fscanf(arquivo, "%s %[^\n]", lixo, nome);
	fscanf(arquivo, "%s %d",  lixo, &ano);
	fscanf(arquivo, "%s %[^\n]", lixo, generos);
	fscanf(arquivo, "%s %[^\n] ", lixo, sinopse);

	return filme_criar(nome, ano, generos, sinopse);
}

void filme_liberar(FILME *filme) {
	free(filme->nome);

	for(int i = 0; i < filme->n_generos; i++)
		free(filme->generos[i]);

	free(filme->generos);
	free(filme->sinopse);
	free(filme);
}

void filme_imprimir(FILME *filme) {
	printf("nome: %s\n", filme->nome);
	printf("ano: %d\n", filme->ano);
	printf("generos: ");

	for(int i = 0; i < filme->n_generos; i++) {
		printf("%s", filme->generos[i]);
		if(i + 1 < filme->n_generos)
			printf(", ");
	}

	printf("\n");
	printf("sinopse: %s\n", filme->sinopse);
}

void filme_imprimir_lista(FILME *filme) {
	printf("%s (%d), ", filme->nome, filme->ano);
	for(int i = 0; i < filme->n_generos; i++) {
		printf("%s", filme->generos[i]);
		if(i + 1 < filme->n_generos)
			printf(", ");
	}
}
