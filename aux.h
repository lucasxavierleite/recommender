#ifndef AUX_H
#define AUX_H

void trim(char *str);
void match(char **a, char **b, int *n, int c1, int c2);
double cosseno(int* u, int* v, int n);
void tokenize(char *str, char **v, int *n, char *delim);

#endif
