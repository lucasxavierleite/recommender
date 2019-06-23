#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "filme.h"
#include "dicionario.h"
#include "aux.h"
#include "interface.h"

/*
*  Struct FILME
*    char *nome  :  nome do filme
*    unsigned n_generos  :  número de gêneros
*      char **generos  :  vetor de strings (gêneros do filme)
*      char *sinopse  :  sinopse do filme
*      unsigned ano  :  ano de lançamento do filme
*
*/

struct filme {
	char *nome;
	unsigned n_generos;
	char **generos;
	char *sinopse;
	unsigned ano;
};

unsigned filme_n_generos(FILME* f){
	return f->n_generos;
}

char** filme_generos(FILME* f){
	return f->generos;
}

/*
*  Função que inicializa e faz alocação dinâmica para um filme 
*
*  Parâmetros: 
*    char *nome  :  nome do filme
*  	 int ano  :  ano de lançamento do filme
*	 char *generos  :  gênero do filme
*	 char *sinopse  :  sinopse do filme
*
*  Retorno:
*    retorna o FILME inicializado com as suas devidas informações 
*/

FILME *filme_criar(char *nome, int ano, char *generos, char *sinopse) {
	FILME *filme = (FILME *) malloc(sizeof(FILME));

	filme->nome = (char *) malloc(sizeof(char) * strlen(nome) + 1);
	strcpy(filme->nome, nome);
	filme->ano = ano;
	filme->generos = tokenize(generos, &filme->n_generos, ",\n");
	filme->sinopse = (char *) malloc(sizeof(char) * strlen(sinopse) + 1);
	strcpy(filme->sinopse, sinopse);

	return filme;
}

/*
*  Função que lê um filme do arquivo e armazena sua sinopse no dicionário (não armazena stopwords)
*
*  Parâmetros: 
*  	 FILE *arquivo  :  ponteiro do arquivo 
*    DICIONARIO *dicionario_sinopse  :  dicionario 
*	 DICIONARIO *stopwords  :  dicionario de stopwords 
*
*  Retorno:
*    retorna uma struct FILME inicializada e deidamente alocada
*/

FILME *filme_ler_arquivo(FILE *arquivo, DICIONARIO *dicionario_sinopse, DICIONARIO *stopwords) {
	char lixo[50];
	char nome[100];
	int ano;
	char generos[200];
	char sinopse[5000];

	fscanf(arquivo, "%s %[^\n]", lixo, nome);
	fscanf(arquivo, "%s %d",  lixo, &ano);
	fscanf(arquivo, "%s %[^\n]", lixo, generos);
	fscanf(arquivo, "%s %[^\n] ", lixo, sinopse);

	int nv = 0;
	char sinopse_cp[strlen(sinopse)];
	strcpy(sinopse_cp, sinopse);

	char **v = tokenize_stopwords(sinopse_cp, &nv, NAO_ALFA, stopwords);

	for(int i = 0; i < nv; i++) {
		dicionario_inserir(dicionario_sinopse, v[i]);
	}

	return filme_criar(nome, ano, generos, sinopse);
}

/*
*  Função para calcular o peso entre os nomes de f1 e f2 (stopwords são ignoradas)
*
*  Parâmetros: 
*	 DICIONARIO *stopwords  :  dicionario de stopwords 
*    FILME *f1  :  filme 1 da comparação
*    FILME *f2  :  filme 2 da comparação
* 
*  Retorno:
*    retorna cálculo da semelhança entre dois filmes com base em seus nomes
*/

double filme_calcula_peso_nome(DICIONARIO* stopwords, FILME *f1, FILME *f2) {
	int nv1 = 0, nv2 = 0;

	char f1_cp[strlen(f1->nome)];
	strcpy(f1_cp, f1->nome);
	char **v1 = tokenize_stopwords(f1_cp, &nv1, NAO_ALFA, stopwords);

	char f2_cp[strlen(f2->nome)];
	strcpy(f2_cp, f2->nome);
	char **v2 = tokenize_stopwords(f2_cp, &nv2, NAO_ALFA, stopwords);

	int correspondencias = match(v1, v2, nv1, nv2);

	return (double) correspondencias / (double) fmax(nv1, nv2);
}

/*
*  Função para calcular o peso entre os anos de lançamento de f1 e f2 
*
*  Parâmetros:
*    FILME *f1  :  filme 1 da comparação
*    FILME *f2  :  filme 2 da comparação
*
*  Retorno:
*    retorna cálculo da semelhança entre dois filmes com base em seus anos de lançamento
*/

double filme_calcula_peso_ano(FILME *f1, FILME *f2) {
	return 1 / (fabs((int) f1->ano - (int) f2->ano) / RELACAO_MAXIMA_ANO + 1);
}

/*
*  Função para calcular o peso entre os gêneros de f1 e f2 
*
*  Parâmetros: 
*    FILME *f1  :  filme 1 da comparação
*    FILME *f2  :  filme 2 da comparação
*
*  Retorno:
*   retorna a divisão do número de palavras correspondentes entre os vetores de gêneros dos filmes pelo total de palavras nos dois vetores
*/

double filme_calcula_peso_generos(FILME *f1, FILME *f2) {
	int correspondencias = match(f1->generos, f2->generos, f1->n_generos, f2->n_generos);
	return (double) correspondencias / (double) fmax(f1->n_generos, f2->n_generos);
}

/*
*  Função para calcular o peso entre as sinopses de f1 e f2 (stopwords são ignoradas)
*
*  Parâmetros: 
*    DICIONARIO *d  : dicionário de palavras
*	 DICIONARIO *stopwords  :  dicionario de stopwords 
*  	 FILME *f1  :  filme 1 da comparação
*    FILME *f2  :  filme 2 da comparação
*
*  Retorno:
*    retorna cálculo da semelhança entre dois filmes com base em suas sinopses 
*/

double filme_calcula_peso_sinopse(DICIONARIO *d, DICIONARIO *stopwords, FILME *f1, FILME *f2) {
	/* Vetores binários (1 = contém, 0 = não contém) */
	unsigned correspondencias1[dicionario_numero_palavras(d)];
	unsigned correspondencias2[dicionario_numero_palavras(d)];

	int nv1 = 0, nv2 = 0;

	char f1_cp[strlen(f1->sinopse)];
	strcpy(f1_cp, f1->sinopse);
	char **v1 = tokenize_stopwords(f1_cp, &nv1, NAO_ALFA, stopwords);

	char v1_e[nv1][51];
	for(int i = 0; i < nv1; i++)
		strcpy(v1_e[i], v1[i]);

	char f2_cp[strlen(f2->sinopse)];
	strcpy(f2_cp, f2->sinopse);
	char **v2 = tokenize_stopwords(f2_cp, &nv2, NAO_ALFA, stopwords);

	char v2_e[nv2][51];
	for(int i = 0; i < nv2; i++)
		strcpy(v2_e[i], v2[i]);

	qsort(v1_e, nv1, sizeof(char) * 51, comparar_nome);
	qsort(v2_e, nv2, sizeof(char) * 51, comparar_nome);

	for(int i = 0; i < dicionario_numero_palavras(d); i++) {
		correspondencias1[i] = buscar(dicionario_buscar_pos(d, i), nv1, sizeof(char) * 51, v1_e);
		correspondencias2[i] = buscar(dicionario_buscar_pos(d, i), nv2, sizeof(char) * 51, v2_e);
	}

	for(int i = 0; i < nv1; i++) free(v1[i]);
	for(int i = 0; i < nv2; i++) free(v2[i]);

	free(v1);
	free(v2);

	return cosseno(correspondencias1, correspondencias2, dicionario_numero_palavras(d));
}

/*
*  Função que retorna o nome do filme em questão
*
*  Parâmetros: 
*    FILME *filme  :  filme em questão
* 
*  Retorno:
*    string com o nome do filme em questão
*/

char *filme_nome(FILME *filme){
	return filme->nome;
}

/*
*  Função para liberar memória dinamicamente alocada para o filme em questão 
*
*  Parâmetros: 
*    FILME *filme  :  filme a ter memória liberada
*    
*/

void filme_liberar(FILME *filme) {
	free(filme->nome);

	for(int i = 0; i < filme->n_generos; i++)
		free(filme->generos[i]);

	free(filme->generos);
	free(filme->sinopse);
	free(filme);
}

/*
*  Função que imprime na tela as informações do filme como parâmetro
*
*  Parâmetros: 
*    FILME *filme  :  filme que se deseja imprimir as informações na tela 
*    
*/

void filme_imprimir(FILME *filme) {
	printf(ANSI_COR_PRETO_BRILHANTE "nome: %s\n", filme->nome);
	printf("ano: %d\n", filme->ano);
	printf("generos: ");

	for(int i = 0; i < filme->n_generos; i++) {
		printf("%s", filme->generos[i]);
		if(i + 1 < filme->n_generos)
			printf(", ");
	}

	printf("\n");
	printf("sinopse: %s" ANSI_COR_RESET, filme->sinopse);
}

/*
*  Função que imprime os dados do filme no formato de lista
*
*  Parâmetros: 
*    FILME *filme  :  filme em questão
*    
*/

void filme_imprimir_lista(FILME *filme) {
	printf(ANSI_COR_PRETO_BRILHANTE "%s (%d) | ", filme->nome, filme->ano);
	for(int i = 0; i < filme->n_generos; i++) {
		printf("%s", filme->generos[i]);
		if(i + 1 < filme->n_generos)
			printf(", ");
	}

	printf(ANSI_COR_RESET);
}
