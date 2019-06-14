#ifndef FILME_H
#define FILME_H

typedef struct filme FILME;

FILME *filme_criar(char *nome, int ano, char *generos, char *sinopse);
FILME *filme_ler_arquivo(FILE *arquivo);
void filme_liberar(FILME *filme);
void filme_imprimir(FILME *filme);
void filme_imprimir_lista(FILME *filme);

#endif
