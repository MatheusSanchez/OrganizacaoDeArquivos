#ifndef __MANIPULA_ARQUIVO_H__
#define __MANIPULA_ARQUIVO_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#define bool int
#define true 1
#define false 0 
#define arquivoSaida "saida.bin"
#define arquivoEntrada "entrada.csv"

#define DATANULA "0000000000"
#define TAMANHOREGISTRO 112
#define T_CABECALHO 5
#define T_DATA 10

struct registro {
	int codEscola;
	char dataInicio[11];
	char dataFinal[11];
	int indicador_tamanho_escola;
	char * nomeEscola;
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


void arquivo_saida(Arquivo *entrada);

void exibe_registros();
void func3(FILE* saida, char* nome_campo, char* val_campo);
char* query (FILE* fp, int b_inicial, int b_final);

void ImprimeRegistro(FILE* fp, int b_inicial);

void RemoveRegistro(FILE* saida, int RRN);

void Insercao(FILE* saida, int codEscola,  char* dataInicio,  char* dataFinal,  char* nome_escola,  char* municipio,  char*endereco);
void updateRegistro(FILE* saida, int codEscola,  char* dataInicio,  char* dataFinal,  char* nome_escola,  char* municipio,  char*endereco, int RRN);
void func8();
void func9();
FILE * abreArquivo(char *nome_arquivo);
bool existeReg(int RRN, FILE * fp);
void fechaArquivo(FILE *fp);

Registro *reg( FILE *fp, int RRN);
long int tamArquivo(FILE *fp);
bool proxRegistro(FILE *fp);
void free_reg(Registro *reg);
#endif
