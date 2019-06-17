#ifndef AUX_H
#define AUX_H

#include <stdio.h>

void trim(char *str);
void match(char **a, char **b, int *n, int c1, int c2);
double cosseno(int* u, int* v, int n);
char** tokenize(char *str, int *n, char *delim);
int comparar_nome(const void *a, const void *b);
int buscar(char *chave, int n, int ni, char v[][51]);

#endif
