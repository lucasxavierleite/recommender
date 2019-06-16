#include "dicionario.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

struct dicionario{
	char **palavras;
	unsigned int n_palavras;
    int ordenado;
};

DICIONARIO* dicionario_criar(){
    DICIONARIO* d = (DICIONARIO*)malloc(sizeof(DICIONARIO));
    if(d){
        d->palavras = NULL;
        d->n_palavras = 0;
        d->ordenado = 0;
    }
    return d;
}

int comparar_nomes(const void*a, const void*b){
    return strcmp((char*) b, (char*) a);  
}

void dicionario_ordenar(DICIONARIO* d){
    qsort(d->palavras, d->n_palavras, sizeof(d->palavras[0]), comparar_nomes);
    d->ordenado = 1;
}


int dicionario_contem(DICIONARIO *d, char *nome) {
    if(!d->ordenado)
        dicionario_ordenar(d);
    return bsearch(nome, d->palavras, d->n_palavras, sizeof(d->palavras[0]), comparar_nomes) ? 1 : 0;
}

void dicionario_inserir(DICIONARIO* d, char* nome){
    if(!dicionario_contem(d, nome)) {
        d->palavras = realloc(d->palavras, (d->n_palavras + 1));
        d->palavras[d->n_palavras] = malloc(sizeof(char)*50);
        strcpy(d->palavras[d->n_palavras], nome);
        d->n_palavras++;
        d->ordenado = 0;
    }
    return;
}

char** dicionario_para_vetor(DICIONARIO* d){
    return d->palavras;
}


char *dicionario_buscar_pos(DICIONARIO *d, int i) {
    char *str = (char *) malloc(sizeof(char) * (strlen(d->palavras[i] + 1)));
    strcpy(str, d->palavras[i]);
    return str;
}

int dicionario_numero_palavras(DICIONARIO* d){
    return d->n_palavras;
}