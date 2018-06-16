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
#define arquivoIndice "indice.bin"
#define TAMANHOREGISTRO 112
#define TAMANHOPAGINA 116
#define T_CABECALHO 5
#define T_CABECALHO_INDICE 13

struct elemento{

int chave;
int RRN;


};

typedef struct elemento Elemento

struct pagina{

	int n;
	int ponteiros[10];
	Elemento elementos[9];
};

typedef struct pagina Pagina;

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

//Funcionalidade 1
char * le_tamanho_variavel(FILE * posicao_atual, int * tamanho_campo );
Arquivo le_dados(char * nome_arquivo);
void arquivo_saida(Arquivo *entrada);
//Funcionalidade 2
void exibe_registros();
//Funcionalidade 3
void buscaCampo(FILE* saida, char* nome_campo, char* val_campo);
void compCampo(char* nome_campo, char* val_campo, Registro *r);
//Funcionalidade 4
bool ImprimeRegistro(FILE* fp, int RRN);
//Funcionalidade 5
void RemoveRegistro(FILE* saida, int RRN);
//Funcionalidade 6
void Insercao(FILE* saida, int codEscola,  char* dataInicio,  char* dataFinal,  char* nome_escola,  char* municipio,  char*endereco);
//Funcionalidade 7
void updateRegistro(FILE* saida, int codEscola,  char* dataInicio,  char* dataFinal,  char* nome_escola,  char* municipio,  char*endereco, int RRN);
//Funcionalidade 8
void CompactaArquivo();
//Funcionalidade 9
void allRegRemovidos();

//Funcoes auxiliares
FILE * abreArquivo(char *nome_arquivo); // abre o arquivo em modo r+
bool existeReg(int RRN, FILE * fp); // verifica se o registro existe
void fechaArquivo(FILE *fp); // fecha o arquivo
int bytesRestantes(FILE* fp, int RRN);
Registro *reg( FILE *fp, int RRN); // le o registro do RRN dado e retorna a struct
long int tamArquivo(FILE *fp); // retorna o numero de bytes do arquivo
bool proxRegistro(FILE *fp); // anda o ponteiro do arquivo para o proximo registro

void EscreveRegistro(FILE* fp,Registro reg, int RRN); // Esreve o Registro no arquivo no RRN dado
#endif
