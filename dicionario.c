#include "dicionario.h"
#include "aux.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/*
*	Struct DICIONARIO
*		char **palavras  :  vetor de strings (palavras do dicionário)
*		unsigned n_palavras  :  número total de palavras no dicionário
*  		unsigned ordenado  :  flag para indicar se dicionario esta ordenado ou não
*
*/
struct dicionario {
	char **palavras;
	unsigned n_palavras;
    unsigned ordenado;
};

/*
*  Função para a inicialização e alocação de memória para o dicionário
*
*  Retorno:
*    retorna um dicionário inicializado
*/
DICIONARIO *dicionario_criar() {
	DICIONARIO *d = (DICIONARIO *) malloc(sizeof(DICIONARIO));

	if(d) {
		d->palavras = NULL;
		d->n_palavras = 0;
		d->ordenado = 0;
	}

	return d;
}

/*
*  Função que cria um dicionário com as palavras de um vetor de strings
*
*  Parâmetros: 
*    char **v  :  vetor de strings
*	 int n  :  número de palavras 
*  
*  Retorno:
*    retorna um dicionário de palavras
*/
DICIONARIO *dicionario_criar_vetor(char **v, int n) {
	DICIONARIO *d = (DICIONARIO *) malloc(sizeof(DICIONARIO));

	if(d) {
		d->palavras = (char**) malloc(sizeof(char *) * sizeof(v));
		d->n_palavras = 0;

		for(int i = 0; i < n; i++)
			dicionario_inserir(d, v[i]);

		d->ordenado = 0;
	}

	return d;
}

/*
*  Função que ordena um DICIONARIO 
*
*  Parâmetros: 
*    DICIONARIO *d  :  dicionário que será ordenado
*    
*/
void dicionario_ordenar(DICIONARIO *d) {
	qsort(d->palavras, d->n_palavras, sizeof(d->palavras[0]), comparar_nome);
	d->ordenado = 1;
}

/*
*  Função para verificar se contém a palavra "nome" no dicionário "d" (que será ordenado caso já não esteja)
*
*  Parâmetros: 
*    DICIONARIO *d : dicionário que será ordenado
*    char *nome  :  palavra à ser buscada
*  Retorno:
*    retorna 1, caso chave se encontre no vetor de strings;
*		     0, caso chave não se encontre no vetor de strings;
*/
int dicionario_contem(DICIONARIO *d, char *nome) {
	if(!d->ordenado)
		dicionario_ordenar(d);
	return bsearch(nome, d->palavras, d->n_palavras, sizeof(d->palavras[0]), comparar_nome) ? 1 : 0;
}

/*
*  Função que insere uma nova palavra ao dicionário
*
*  Parâmetros: 
*    DICIONARIO *d  :  dicionário que receberá novo nome
*	 char *nome  : nome a ser inserido no dicionário
*    
*/
void dicionario_inserir(DICIONARIO *d, char *nome) {
	if(!dicionario_contem(d, nome)) {
		d->palavras = realloc(d->palavras, sizeof(char *) * (d->n_palavras + 1));
		d->palavras[d->n_palavras] = malloc(sizeof(char) * 51);
		strcpy(d->palavras[d->n_palavras], nome);
		d->n_palavras++;
		d->ordenado = 0;
	}
}

/*
*  Função que busca uma palavra no dicionário dada a sua posição
*
*  Parâmetros: 
*    DICIONARIO *d  :  dicionário em questão
*  	 int i  :  posição da palavra
*
*  Retorno:
*    retorna a palavra da posição de número i
*/
char *dicionario_buscar_pos(DICIONARIO *d, int i) {
	char *str = (char *) malloc(sizeof(char) * (strlen(d->palavras[i] + 1)));
	strcpy(str, d->palavras[i]);
	return str;
}

/*
*  Função que retorna o número de palavras presentes no dicionário
*
*  Parâmetros: 
*    DICIONARIO *d  :  dicionário em questão
*  
*  Retorno:
*    retorna o numero de palavras presentes no dicionário
*/
int dicionario_numero_palavras(DICIONARIO *d) {
	return d->n_palavras;
}

/*
*  Função que retorna as palavras no dicionário
*
*  Parâmetros: 
*    DICIONARIO *d  :  dicionário em questão
*  
*  Retorno:
*    retorna vetor de strings
*/
char **dicionario_para_vetor(DICIONARIO *d) {
	return d->palavras;
}
