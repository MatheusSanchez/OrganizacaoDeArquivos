#ifndef __MANIPULA_ARQUIVO_H__
#define __MANIPULA_ARQUIVO_H__

#include <stdlib.h>
#include <stdio.h>

#define TABOM 0
#define DEURUIM 42
#define DATANULA "0000000000"
#define TAMANHOREGISTRO 112
#define OCUPADOS 41

struct registro {
	int codigo_escola;
	char data_inicio[11];
	char data_final[11];
	int indicador_tamanho_escola;
	char * nome_escola;
	int indicador_tamanho_municipio;
	char * municipio;
	int indicador_tamanho_endereco;
	char * endereco;
};

typedef struct registro Registro;

struct arquivo {
	int n_registros_lidos;
	// pode ser 0 se o arquivo foi corrompido ou 1 se está íntegro
	char status;
	int topo_pilha;
	Registro * registros_lidos;
};

typedef struct arquivo Arquivo;

char * le_tamanho_variavel(FILE * posicao_atual, int * tamanho_campo );

Arquivo le_dados(char * nome_arquivo);


FILE * arquivo_saida(Arquivo *entrada);

#endif
