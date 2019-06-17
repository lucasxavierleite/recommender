#ifndef DICIONARIO_H
#define DICIONARIO_H

typedef struct dicionario DICIONARIO;

DICIONARIO *dicionario_criar();
void dicionario_inserir(DICIONARIO *d, char *nome);
char *dicionario_buscar_pos(DICIONARIO *d, int i);
int dicionario_numero_palavras(DICIONARIO *d);
char **dicionario_para_vetor(DICIONARIO *d);


#endif