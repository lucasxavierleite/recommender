#include <stdio.h>
#include "interface.h"
/*
*	Função que imprime na tela um menu para interação do usuário
*/
void menu_imprimir() {
	printf(ANSI_COR_PRETO_BRILHANTE ANSI_COR_AZUL "1) Listar filmes\n");
	printf("2) Buscar filme\n");
	printf("3) Buscar filmes relacionados com base na sinopse\n");
	printf("4) Buscar filmes relacionados com base em todos os critérios\n");
	printf("5) Verificar relação entre dois filmes\n");
	printf("6) Sair\n" ANSI_COR_RESET);
}

/*
*	Função que lê opção escolhida pelo usuário
*
*	Parâmetros:
*		char *opcao  :  opção entrada pelo usuário
*
*/
void menu_ler_opcao(char *opcao) {
	*opcao = getchar();
	getchar();
}

/*
*	Função para a leitura do título
*
*	Parâmetros:
*		char *titulo  :  título digitado pelo usuário
*
*/
void menu_ler_titulo(char *titulo) {
	printf(ANSI_COR_PRETO_BRILHANTE "\n> " ANSI_COR_RESET);
	scanf("%[^\n]", titulo);
	getchar();
}

/*
*	Função que imprime na tela uma mensagem de encerramento
*/
void mensagem_encerrar() {
	printf(ANSI_COR_PRETO_BRILHANTE "\n> Até\n" ANSI_COR_RESET);
}

/*
*	Função que imprime na tela uma mensagem de erro por não encontrar o filme digitado pelo usuário
*/
void erro_filme_nao_encontrado() {
	printf(ANSI_COR_PRETO_BRILHANTE ANSI_COR_VERMELHA "\n> Esse filme não consta na nossa base de dados");
	printf("\n  Por favor, verifique o título e tente novamente\n\n" ANSI_COR_RESET);
}

/*
*	Função que imprime na tela uma mensagem de erro por não encontrar filmes relacionados ao filme digitado pelo usuário
*/
void erro_filme_sem_recomendacoes() {
	printf(ANSI_COR_PRETO_BRILHANTE ANSI_COR_VERMELHA "\n> Infelizmente, não fomos capazes de encontrar um filme para você");
	printf("\n  Tente novamente com outro filme\n\n" ANSI_COR_RESET);
}
