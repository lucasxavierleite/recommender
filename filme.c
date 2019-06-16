#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "filme.h"
#include "aux.h"
#include <math.h>
#include "dicionario.h"

#define NAO_ALFA  " !@#$%*()_+=-[{]};:?//.,|'\n"

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

FILME *filme_ler_arquivo(FILE *arquivo, DICIONARIO* dicionario_sinopse) {
	char lixo[50];
	char nome[100];
	int ano;
	char generos[200];
	char sinopse[5000];

	fscanf(arquivo, "%s %[^\n]", lixo, nome);
	fscanf(arquivo, "%s %d",  lixo, &ano);
	fscanf(arquivo, "%s %[^\n]", lixo, generos);
	fscanf(arquivo, "%s %[^\n] ", lixo, sinopse);

	char** v = NULL;
	int nv = 0;
	char sinopse_cp[strlen(sinopse)];
	strcpy(sinopse_cp, sinopse);
	
	tokenize(sinopse_cp, v, &nv, NAO_ALFA); 
	for(int i = 0; i < nv; i++){
		dicionario_inserir(dicionario_sinopse, v[i]);
	}

	return filme_criar(nome, ano, generos, sinopse);
}

double filme_calcula_peso_nome(FILME* f1, FILME* f2) {
	char **v1 = NULL, **v2 = NULL; /* vetorES de strings */
	int c1 = 0, c2 = 0;

	char f1_cp[strlen(f1->nome)];
	strcpy(f1_cp, f1->nome);
	tokenize(f1_cp, v1, &c1, NAO_ALFA);

	char f2_cp[strlen(f2->nome)];
	strcpy(f2_cp, f2->nome);
	tokenize(f2_cp, v2, &c2, NAO_ALFA);

	int correspondencias = 0;
	match(v1, v2, &correspondencias, c1, c2);
	
	return (double) correspondencias / (double) (c1 + c2);
}

double filme_calcula_peso_ano(FILME *f1, FILME *f2) {
	return 1 / (abs(f1->ano - f2->ano) + 1); 
}

double filme_calcula_peso_generos(FILME *f1, FILME *f2) {
	int correspondencias = 0;
	match(f1->generos, f2->generos, &correspondencias, f1->n_generos, f2->n_generos);
	
	return (double) correspondencias / (double) (f1->n_generos + f2->n_generos);
}

double filme_calcula_peso_sinopse(DICIONARIO* d, FILME* f1, FILME* f2) {
	/*vetores de bits(1 = contem, 0 = nao contem)*/
	unsigned correspondencias1[dicionario_numero_palavras(d)];
	unsigned correspondencias2[dicionario_numero_palavras(d)];
	char** v1 = NULL, **v2 = NULL;

	int c1 = 0, c2 = 0;

	char f1_cp[strlen(f1->sinopse)];
	strcpy(f1_cp, f1->sinopse);
	tokenize(f1_cp, v1, &c1, NAO_ALFA);

	char f2_cp[strlen(f2->sinopse)];
	strcpy(f2_cp, f2->sinopse);
	tokenize(f2_cp, v2, &c2, NAO_ALFA);

	for(int i = 0; i < dicionario_numero_palavras(d); i++){
		correspondencias1[i] == (strcmp(dicionario_buscar_pos(d, i), v1[i]) == 0);
		correspondencias2[i] == (strcmp(dicionario_buscar_pos(d, i), v2[i]) == 0);
	}

	return cosseno(correspondencias1, correspondencias2, dicionario_numero_palavras(d));
}

char* filme_nome(FILME* filme){
	return filme->nome;
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
