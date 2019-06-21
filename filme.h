#ifndef FILME_H
#define FILME_H

#include "dicionario.h"

#define RELACAO_MAXIMA_ANO 8

typedef struct filme FILME;

FILME *filme_criar(char *nome, int ano, char *generos, char *sinopse);
FILME *filme_ler_arquivo(FILE *arquivo, DICIONARIO* dicionario_sinopse, DICIONARIO* stopwords);
void filme_liberar(FILME *filme);
void filme_imprimir(FILME *filme);
void filme_imprimir_lista(FILME *filme);
double filme_calcula_peso_nome(DICIONARIO* stopwords, FILME* f1, FILME* f2);
double filme_calcula_peso_ano(FILME* f1, FILME* f2);
double filme_calcula_peso_generos(FILME* f1, FILME* f2);
double filme_calcula_peso_sinopse(DICIONARIO* d, DICIONARIO* stopwords, FILME* f1, FILME* f2);
char* filme_nome(FILME* filme);


#endif
