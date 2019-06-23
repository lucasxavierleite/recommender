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

	titulo();

	int opcao;
	char nome_filme_1[255];
	char nome_filme_2[255];

	while(opcao != MENU_SAIR) {
		menu_imprimir();
		opcao = menu_ler_opcao(&opcao);

		switch(opcao) {
			case MENU_LISTAR_FILMES:
				grafo_imprimir_filmes(grafo);
				break;
			case MENU_BUSCAR_FILME:
				menu_ler_titulo(nome_filme_1);
				grafo_buscar(grafo, nome_filme_1);
				break;
			case MENU_RECOMENDAR_FILMES:
				menu_ler_titulo(nome_filme_1);
				grafo_recomendar(grafo, nome_filme_1, RECOMENDACAO_GERAL);
				break;
			case MENU_RECOMENDAR_FILMES_SINOPSE:
				menu_ler_titulo(nome_filme_1);
				grafo_recomendar(grafo, nome_filme_1, RECOMENDACAO_SINOPSE);
				break;
			case MENU_RECOMENDAR_FILMES_POR_GENERO:
				menu_ler_titulo(nome_filme_1);
				grafo_recomendar_filmes_genero(grafo, nome_filme_1);
				break;
			case MENU_RECOMENDAR_FILMES_NAO_RELACIONADOS:
				menu_ler_titulo(nome_filme_1);
				grafo_recomendar(grafo, nome_filme_1, RECOMENDACAO_NAO_RELACIONADOS);
				break;
			case MENU_VERIFICAR_RELACAO_FILMES:
				menu_ler_titulo(nome_filme_1);
				menu_ler_titulo(nome_filme_2);
				grafo_verificar_relacao(grafo, nome_filme_1, nome_filme_2);
				break;
			case MENU_SAIR:
				continue;
				break;
			default:
				menu_opcao_invalida();
				break;
		}
	}

	mensagem_encerrar();
	grafo_liberar(grafo);

	return 0;
}
