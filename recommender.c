#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "grafo.h"
#include "aux.h"
#include "interface.h"

int main(int argc, char *argv[]) {
	FILE *arquivo = fopen(argv[1], "r");

	if(!arquivo) {
		printf("Erro ao abrir o arquivo de dados \"%s\"\n", argv[0]);
		return -1;
	}

	GRAFO *grafo = grafo_criar(arquivo);
	fclose(arquivo);

	char opcao;
	char nome_filme[255];

	while(opcao != MENU_SAIR) {
		menu_imprimir();
		menu_ler_opcao(&opcao);

		switch(opcao) {
			case MENU_LISTAR_FILMES:
				grafo_imprimir_filmes(grafo);
				break;
			case MENU_BUSCAR_FILME:
				menu_ler_titulo(nome_filme);
				grafo_buscar(grafo, nome_filme);
				break;
			case MENU_RECOMENDAR_FILMES:
				menu_ler_titulo(nome_filme);
				grafo_recomendar(grafo, nome_filme, RECOMENDACAO_GERAL);
				break;
			case MENU_RECOMENDAR_FILMES_SINOPSE:
				menu_ler_titulo(nome_filme);
				grafo_recomendar(grafo, nome_filme, RECOMENDACAO_SINOPSE);
				break;
			case MENU_RECOMENDAR_FILMES_POR_GENERO:
				menu_ler_titulo(nome_filme);
				grafo_imprimir_filmes_genero(grafo, nome_filme);
				break;
			case MENU_RECOMENDAR_FILMES_NAO_RELACIONADOS:
				menu_ler_titulo(nome_filme);
				grafo_recomendar(grafo, nome_filme, RECOMENDACAO_NAO_RELACIONADOS);
				break;
		}
	}

	mensagem_encerrar();
	grafo_liberar(grafo);

	return 0;
}
