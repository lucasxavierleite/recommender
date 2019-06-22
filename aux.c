#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "aux.h"

/* Vetor de stopwords (palavras consideradas irrelevantes) */

char **vetor_stopwords_global = (char *[]) { "a", "ao", "aos", "aquela", "aquelas",
	"aquele", "aqueles", "aquilo", "as", "até", "com", "como", "da", "das", "de",
	"dela", "delas", "dele", "deles", "depois", "do", "dos", "e", "ela", "elas",
	"ele", "eles", "em", "entre", "era", "era", "eram", "essa", "essas", "esse",
	"esses", "esta", "estamos", "estas", "estava", "estavam", "este", "esteja",
	"estejam", "estejamos", "estes", "esteve", "estive", "estivemos", "estiver",
	"estivera", "estiveram", "estiverem", "estivermos", "estivesse", "estivessem",
	"estivéramos", "estivéssemos", "estou", "está", "está", "estávamos", "estão",
	"estão", "eu", "foi", "foi", "fomos", "for", "fora", "foram", "foram", "forem",
	"formos", "fosse", "fosse", "fossem", "fui", "fôramos", "fôssemos", "haja",
	"hajam", "hajamos", "havemos", "havia", "hei", "houve", "houvemos", "houver",
	"houvera", "houveram", "houverei", "houverem", "houveremos", "houveria",
	"houveriam", "houvermos", "houverá", "houverão", "houveríamos", "houvesse",
	"houvessem", "houvéramos", "houvéssemos", "há", "há", "hão", "isso", "isto",
	"já", "lhe", "lhes", "mais", "mas", "me", "mesmo", "meu", "meus", "minha",
	"minhas", "muito", "na", "nas", "nem", "no", "nos", "nossa", "nossas",
	"nosso", "nossos", "num", "numa", "não", "nós", "o", "os", "ou", "para",
	"pela", "pelas", "pelo", "pelos", "por", "qual", "quando", "que", "quem",
	"se", "seja", "seja", "sejam", "sejamos", "sem", "ser", "serei", "seremos",
	"seria", "seriam", "será", "será", "serão", "seríamos", "seu", "seus",
	"somos", "sou", "sua", "suas", "são", "só", "também", "te", "tem", "tem",
	"temos", "tenha", "tenham", "tenhamos", "tenho", "tenho", "ter", "terei",
	"teremos", "teria", "teriam", "terá", "terão", "teríamos", "teu", "teus",
	"teve", "tinha", "tinha", "tinham", "tive", "tivemos", "tiver", "tivera",
	"tiveram", "tiverem", "tivermos", "tivesse", "tivessem", "tivéramos",
	"tivéssemos", "tu", "tua", "tuas", "tém", "têm", "tínhamos", "um", "uma",
	"você", "vocês", "vos", "à", "às", "é", "éramos" };

/*
*  Remove os espaços em branco antes e depois de uma string
*
*  Parâmetros: 
*    char *str : string a ter espaços do início e fim removidos
*/

void trim(char *str) {
	if(!isspace(*str) && !isspace(*(str + strlen(str) - 1)))
		return;

	char *beg = str, *end = str + strlen(str) - 1;

	while(isspace(*beg)) *beg++;
	while(!isalpha(*end)) *end--;

	*(end + 1) = '\0';

	strcpy(str, beg);
}

/*
*  Extrai as palavras de um texto, com base no delimitador especificado
*
*  Parâmetros: 
*    char *str : string com as palavras e delimitadores 
*    char **v  :  vetor de palavras extraidas do texto
*    int *n  :  número de palavras 
*    char *delim  :  string com os caracteres delimitadores
*
*  Retorno:
*    Vetor de strings (palavras do texto)
*/

char **tokenize(char *str, int *n, char *delim) {
	*n = 0;

	char **v = NULL;
	char *token = strtok(str, delim);

	while(token) {
		v = (char **) realloc(v, sizeof(char *) * (*n+1));
		// v[*n] = (char *) malloc(sizeof(char) * (strlen(token) + 1));
		v[*n] = (char *) malloc(sizeof(char) * 51);
		trim(token);
		strcpy(v[*n], token);
		token = strtok(NULL, delim);
		(*n)++;
	}

	return v;
}

/*
*  Extrai as palavras de um texto, com base no delimitador especificado, ignorando
*  as palavras contidas no dicionario passado como parâmetro (stopwords)
*
*  Parâmetros: 
*    char *str     : vetor de stopwords
*    int *n        :  número de correspondências
*    char *delim   :  string com os caracteres delimitadores
*    DICIONARIO *d : dicionario de palavras a serem ignoradas
*
*  Retorno:
*    Vetor de strings (palavras do texto) que não estão contidas no dicionário
*/

char **tokenize_stopwords(char *str, int *n, char *delim, DICIONARIO *d) {
	*n = 0;

	char **v = NULL;
	char *token = strtok(str, delim);

	while(token) {
		v = (char **) realloc(v, sizeof(char *) * (*n+1));
		// v[*n] = (char *) malloc(sizeof(char) * (strlen(token) + 1));
		v[*n] = (char *) malloc(sizeof(char) * 51);
		trim(token);

		if(!dicionario_contem(d, token))
			strcpy(v[*n], token);

		token = strtok(NULL, delim);
		(*n)++;
	}

	return v;
}

/*
*  Calcula o número de correspondências entre dois vetores de strings
*
*  Parâmetros: 
*    char **a : vetor de palavras
*    char **b  :  vetor de palavras
*    int na  :  tamanho do vetor a
*    int nb  :  tamanho do vetor b
*
*  Retorno:
*    Número de correspondências
*/

int match(char **a, char **b, int na, int nb) {
	int n = 0;
	int n_max = fmax(na, nb);
	int n_min = fmin(na, nb);

	if(n_max == na) {
		for(int i = 0; i < n_max; i++) {
			for(int j = 0; j < n_min; j++) {
				if(strcasecmp(a[i], b[j]) == 0) {
					n++;
					break;
				}
			}
		}
	} else if(n_max == nb) {
		for(int i = 0; i < n_min; i++) {
			for(int j = 0; j < n_max; j++) {
				if(strcasecmp(a[i], b[j]) == 0) {
					n++;
					break;
				}
			}
		}
	}

	return n;
}

/*
*  Compara duas strings (case insensitive)
*
*  Parâmetros: 
*    const void *a : ponteiro genérico para a primeira string a ser comparada
*    const void *b : ponteiro genérico para a segunda string a ser comparada
*
*  Retorno:
*   Inteiro < 0, caso 'a' entre primeiro pela ordem alfabética;
*           = 0, caso 'a' e 'b' sejam iguais
*           > 0, caso 'b' entre primeiro pela ordem alfabética;
*/

int comparar_nome(const void *a, const void *b) {
	for(int i = 0; i < strlen((char *) a); i++)

	return strcasecmp((char *) a, (char *) b);
}

/*
*  Função para busca binária de uma chave dentro de um vetor de strings ordenado
*
*  Parâmetros: 
*    char *chave   : chave buscada
*    int *n        :  número de palavras
*    int ni        :  tamanho máximo das palavras
*    char v[][51]  :  vetor de strings
*
*  Retorno:
*    Inteiro = 1, caso a chave se encontre no vetor de strings
*            = 0, caso a chave não se encontre no vetor de strings
*/

int buscar(char *chave, int n, int ni, char v[][51]) {
	return bsearch(chave, v, n, ni, comparar_nome) ? 1 : 0;
}

/*
*  Função que calcula o cosseno entre dois vetores
*
*  Parâmetros: 
*    int *u : primeiro vetor
*	 int *v : segundo vetor
*    int n  : tamanhos dos vetores u e v
*  
*  Retorno:
*    Cosseno entre u e v
*/

double cosseno(int *u, int *v, int n){
	int produto = 0;
	double norma_u = 0, norma_v = 0;

	for(int i = 0; i < n; i++){
		produto += u[i] * v[i];
		norma_u += pow(u[i], 2);
		norma_v += pow(v[i], 2);
	}

	norma_u = sqrt(norma_u);
	norma_v = sqrt(norma_v);

	return (double) produto / (norma_u * norma_v);
}
