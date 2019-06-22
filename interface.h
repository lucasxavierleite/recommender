#ifndef INTERFACE_H
#define INTERFACE_H

#define ANSI_COR_PRETO_BRILHANTE "\x1b[30;1m"
#define ANSI_COR_VERMELHA "\x1b[31m"
#define ANSI_COR_VERDE    "\x1b[32m"
#define ANSI_COR_AMARELA  "\x1b[33m"
#define ANSI_COR_AZUL     "\x1b[34m"
#define ANSI_COR_MAGENTA  "\x1b[35m"
#define ANSI_COR_CIANO    "\x1b[36m"
#define ANSI_COR_RESET    "\x1b[0m"

#define MENU_LISTAR_FILMES '1'
#define MENU_BUSCAR_FILME '2'
#define MENU_RECOMENDAR_FILMES '3'
#define MENU_RECOMENDAR_FILMES_SINOPSE '4'
#define MENU_RECOMENDAR_FILMES_POR_GENERO '5'
#define MENU_RECOMENDAR_FILMES_NAO_RELACIONADOS '6'
#define MENU_VERIFICAR_RELACAO_FILMES '7'
#define MENU_SAIR '8'

void menu_imprimir();
void menu_ler_opcao(char *opcao);
void menu_ler_titulo(char *titulo);
void mensagem_encerrar();
void erro_filme_nao_encontrado();
void erro_filme_sem_recomendacoes();
void erro_genero_nao_encontrado();

#endif
