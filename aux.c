#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "aux.h"

void trim(char *str) {
	if(!isspace(*str) && !isspace(*(str + strlen(str) - 1)))
		return;

	char *beg = str, *end = str + strlen(str) - 1;

	while(isspace(*beg)) *beg++;
	while(!isalpha(*end)) *end--;

	*(end + 1) = '\0';

	strcpy(str, beg);
}

char **tokenize(char *str, int *n, char *delim) {
	*n = 0;

	char **v = NULL;
	char *token = strtok(str, delim);
	
	while(token) {
		v = (char **) realloc(v, sizeof(char *) * (*n+1));
		// v[*n] = (char *) malloc(sizeof(char) * (strlen(token) + 1));
		v[*n] = (char *) malloc(sizeof(char) * 51);
		strcpy(v[*n], token);
		token = strtok(NULL, delim);
		(*n)++;
	}

	return v;
}

void match(char **a, char **b, int *n, int c1, int c2) {
	for(int i = 0; i < fmin(c1, c2); i++) {
		for(int j = 0; j < fmax(c1, c2); j++) {
			if(c1 < c2) {
				if(strcmp(a[i], b[j]) == 0) (*n)++;
			} else {
				if(strcmp(a[j], b[i]) == 0) (*n)++;
			}
		}
	}

	return;
}

int comparar_nome(const void *a, const void *b) {
    return strcmp((char *) b, (char *) a);  
}

int buscar(char *chave, int n, int ni, char v[][51]) {
	return bsearch(chave, v, n, ni, comparar_nome) ? 1 : 0;
}

/*cuidado com o tipo na funcao*/
double cosseno(int *u, int *v, int n){
	int produto = 0; 
	double norma_u = 0, norma_v = 0;
	
	for(int i = 0; i < n; i++){
		produto += u[i] * v[i];
		norma_u += pow(u[i], 2);
		norma_v += pow(v[i], 2);
	}
	
	norma_u = sqrt(norma_u);
	norma_v = sqrt(norma_v);
	
	return (double) produto / (norma_u * norma_v);
}