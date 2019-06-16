#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "aux.h"
#include <math.h>

void trim(char *str) {
	if(!isspace(*str) && !isspace(*(str + strlen(str) - 1)))
		return;

	char *beg = str, *end = str + strlen(str) - 1;

	while(isspace(*beg)) *beg++;
	while(!isalpha(*end)) *end--;

	*(end + 1) = '\0';

	strcpy(str, beg);
}

void tokenize(char *str, char **v, int *n, char *delim) {
	*n = 0;

	char* token = strtok(str, delim);
	while(token) {
		v = (char **) realloc(v, sizeof(char *) * (*n+1));
		token = strtok(NULL, delim);
		v[*n] = (char *) malloc(sizeof(char) * (strlen(token) + 1));
		strcpy(v[*n], token);
		*n++;
	}
}

void match(char **a, char **b, int *n, int c1, int c2) {
	for(int i = 0; i < fmin(c1, c2); i++)
		for(int j = 0; j < fmax(c1, c2); j++)
			if(strcmp(a[i], b[j]) == 0) *n++;
}

/*cuidado com o tipo na funcao*/
double cosseno(int* u, int* v, int n){
	int produto = 0; 
	double norma_u = 0, norma_v = 0;
	
	for(int i = 0; i < n; i++){
		produto += u[i] * v[i];
		norma_u += pow(u[i],2);
		norma_v += pow(v[i],2);
	}
	
	norma_u = sqrt(norma_u);
	norma_v = sqrt(norma_v);
	
	return (double)produto / (norma_u * norma_v);
}