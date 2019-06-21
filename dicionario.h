#ifndef DICIONARIO_H
#define DICIONARIO_H

typedef struct dicionario DICIONARIO;

DICIONARIO *dicionario_criar();
DICIONARIO *dicionario_criar_vetor(char **v, int n);
void dicionario_inserir(DICIONARIO *d, char *nome);
char *dicionario_buscar_pos(DICIONARIO *d, int i);
int dicionario_numero_palavras(DICIONARIO *d);
int dicionario_contem(DICIONARIO *d, char *nome);
char **dicionario_para_vetor(DICIONARIO *d);


#endif
