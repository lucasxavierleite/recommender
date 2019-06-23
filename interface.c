#include <stdio.h>
#include "interface.h"

/* Imprime o título do programa e a mensagem do boas-vindas */

void titulo() {
	const char *titulo[80];
	titulo[0] = " _____                                                   _";
	titulo[1] = "|  __ \\                                                 | |";
	titulo[2] = "| |__) |___  ___ ___  _ __ ___  _ __ ___   ___ _ __   __| | ___ _ __";
	titulo[3] = "|  _  // _ \\/ __/ _ \\| '_ ` _ \\| '_ ` _ \\ / _ \\ '_ \\ / _` |/ _ \\ '__|";
	titulo[4] = "| | \\ \\  __/ (_| (_) | | | | | | | | | | |  __/ | | | (_| |  __/ |   ";
	titulo[5] = "|_|  \\_\\___|\\___\\___/|_| |_| |_|_| |_| |_|\\___|_| |_|\\__,_|\\___|_|";

	printf(ANSI_COR_PRETO_BRILHANTE ANSI_COR_MAGENTA);

	for(int i = 0; i < 6; i++)
		printf("%s\n", titulo[i]);

	printf("\n" ANSI_COR_RESET);
	printf(ANSI_COR_PRETO_BRILHANTE "\n> Bem-vindo ao Recommender\n\n" ANSI_COR_RESET);
}

/*
*  Função que imprime na tela um menu para interação do usuário
*/

void menu_imprimir() {
	printf(ANSI_COR_PRETO_BRILHANTE ANSI_COR_AZUL "1) Listar filmes\n");
	printf("2) Buscar filme\n");
	printf("3) Recomendar filmes relacionados com base em todos os critérios\n");
	printf("4) Recomendar filmes relacionados com base na sinopse\n");
	printf("5) Recomendar filmes por gênero\n");
	printf("6) Recomendar filmes não relacionados\n");
	printf("7) Verificar relação entre dois filmes\n");
	printf("8) Sair\n" ANSI_COR_RESET);
}

/*
*  Função que lê opção escolhida pelo usuário
*
*  Retorno:
*    Opção entrada pelo usuário
*/

int menu_ler_opcao() {
	printf(ANSI_COR_PRETO_BRILHANTE "\n> " ANSI_COR_RESET);

	char buffer[100];
	scanf("%[^\n]", buffer);
	getchar();

	return buffer[0] - '0';
}

/*
*  Função para a leitura do título
*
*  Parâmetros:
*    char *titulo  :  título digitado pelo usuário
*/

void menu_ler_titulo(char *titulo) {
	printf(ANSI_COR_PRETO_BRILHANTE "\n> Digite o título do filme: " ANSI_COR_RESET);
	scanf("%[^\n]", titulo);
	getchar();
}

/* Imprime a mensagem de opção inválida */

void menu_opcao_invalida() {
	printf(ANSI_COR_PRETO_BRILHANTE ANSI_COR_VERMELHA "\n> Opção inválida\n\n" ANSI_COR_RESET);
}

/* Imprime na tela a mensagem de encerramento */

void mensagem_encerrar() {
	printf(ANSI_COR_PRETO_BRILHANTE "\n> Até\n" ANSI_COR_RESET);
}

/*
*  Imprime na tela uma mensagem de erro por não encontrar o filme especificado
*/

void erro_filme_nao_encontrado() {
	printf(ANSI_COR_PRETO_BRILHANTE ANSI_COR_VERMELHA "\n> Esse filme não consta na nossa base de dados");
	printf("\n  Por favor, verifique o título e tente novamente\n\n" ANSI_COR_RESET);
}

/*
*  Imprime na tela uma mensagem de erro por não encontrar filmes relacionados
*  ao filme especificado
*/

void erro_filme_sem_recomendacoes() {
	printf(ANSI_COR_PRETO_BRILHANTE ANSI_COR_VERMELHA "\n> Infelizmente, não fomos capazes de encontrar um filme para você");
	printf("\n  Tente novamente com outro filme\n\n" ANSI_COR_RESET);
}

/*
*  Imprime na tela uma mensagem de erro por não encontrar filmes com o gênero
*  especificado
*/

void erro_genero_nao_encontrado() {
	printf(ANSI_COR_PRETO_BRILHANTE ANSI_COR_VERMELHA "\n> Não há nenhum filme com o gênero especificado em nossa base de dados");
	printf("\n  Tente outro gênero\n\n" ANSI_COR_RESET);
}
