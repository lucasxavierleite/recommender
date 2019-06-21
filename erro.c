#include <stdio.h>
#include "erro.h"

void erro_filme_nao_encontrado() {
	printf("\n> Infelizmente, não fomos capazes de recomendar um filme para você");
	printf("\nTente novamente com outro filme\n");
		printf("\n> Esse filme não consta na nossa base de dados");
		printf("\nPor favor, verifique o título e tente novamente\n");
}

void erro_filme_sem_recomendacoes() {
	printf("\n> Infelizmente, não fomos capazes de encontrar um filme para você");
	printf("\nTente novamente com outro filme\n");
}
