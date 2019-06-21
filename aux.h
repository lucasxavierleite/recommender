#ifndef AUX_H
#define AUX_H

#include <stdio.h>
#include "dicionario.h"

void trim(char *str);
int match(char **a, char **b, int na, int nb);
double cosseno(int* u, int* v, int n);
char** tokenize(char *str, int *n, char *delim);
char** tokenize_stopwords(char *str, int *n, char *delim, DICIONARIO* d); 
int comparar_nome(const void *a, const void *b);
int buscar(char *chave, int n, int ni, char v[][51]);
extern char** vet_stopwords_global;

#endif
