#include "manipula_arquivo.h"
#include <string.h>

char * le_tamanho_variavel(FILE * posicao_atual, int * tamanho_campo) {
	char atual, *palavra_lida = NULL;
	int n_letras = 0;
	FILE * pa;
	pa = posicao_atual;
	
	do {
		// le char a char o que está no arquivo
		atual = fgetc(pa);

		// enquanto não chega no delimitador realoca a string lida e atribui
		// o caractere à última posicao 

		if (atual != ';') {
			palavra_lida = realloc(palavra_lida, n_letras + 1);
			*(palavra_lida + n_letras) = atual;
			n_letras++;
		}

	} while ((atual != ';'));
	

	// delimita a string lida

	if (n_letras == 0) {
		palavra_lida = realloc(palavra_lida, n_letras + 1);
		*(palavra_lida + n_letras) = ';';
		*tamanho_campo = n_letras ;
	} else {
		palavra_lida = realloc(palavra_lida, n_letras + 1);
		*(palavra_lida + n_letras) = '\0';
		*tamanho_campo = n_letras ;
	}

	return palavra_lida;
}

Arquivo le_dados(char * nome_arquivo) {

	FILE * fp = NULL;
	
	int i =0;

	Arquivo novo;

	//SETANDO O REGISTRO DE CABEÇALHO
	novo.status = '1';
	novo.topo_pilha = -1;

	novo.n_registros_lidos = 0;
	char auxiliar;
	fp = abreArquivo("entrada.csv");
	long int tamanho_arquivo = tamArquivo(fp);
	novo.registros_lidos = NULL;
	if(fp == NULL){
		printf("Falha no carregamento do arquivo.\n");
		return novo;
	}

	while (ftell(fp) < tamanho_arquivo){

		novo.registros_lidos = realloc(novo.registros_lidos, (novo.n_registros_lidos + 1) * sizeof(Registro));
		
		// acessa o Arquivo no vetor de registros e salva em cada elemento
		// da struct o que lhe é destinado

		// Leio o codigo da escola e pulo o ;
		fscanf(fp,"%d",&(novo.registros_lidos[i].codEscola));
		fgetc(fp);

		// Leio os Registros de Tamanho Variavel
		novo.registros_lidos[i].nomeEscola = 
		le_tamanho_variavel(fp, &(novo.registros_lidos[i].indicador_tamanho_escola));
		novo.registros_lidos[i].municipio = 
		le_tamanho_variavel(fp, &(novo.registros_lidos[i].indicador_tamanho_municipio));
		novo.registros_lidos[i].endereco = 
		le_tamanho_variavel(fp, &(novo.registros_lidos[i].indicador_tamanho_endereco));

		//Se o proximo caracter for um ; entao quer dizer que a data inicio é NULA
		auxiliar = fgetc(fp);

		if (auxiliar == ';') {
			strcpy(novo.registros_lidos[i].dataInicio, "0000000000\0");
		} else {
			// senao for, então atribuo o caracter lido para a primeira posicao da string e leio +9 bytes
			novo.registros_lidos[i].dataInicio[0] = auxiliar;
			fread((novo.registros_lidos[i].dataInicio)+1, 1, 9, fp);	
			novo.registros_lidos[i].dataInicio[10] = '\0'; // para indicar o final da string
			fgetc(fp); // leio o ;
		}	

		// se o proximo caracter for um \n, entao a data final é nula
		auxiliar = fgetc(fp);

		if (auxiliar == '\n') {	
			strcpy(novo.registros_lidos[i].dataFinal, "0000000000\0");
		} else {
			// senao for, então atribuo o caracter lido para a primeira posicao da string e leio +9 bytes
			novo.registros_lidos[i].dataFinal[0] = auxiliar;
			fread((novo.registros_lidos[i].dataFinal)+1, 1, 9, fp);	
			novo.registros_lidos[i].dataFinal[10] = '\0'; // para indicar o final da string
			auxiliar = fgetc(fp); // leio o \n
			
		}	

		//+1 registro lido com sucesso
		novo.n_registros_lidos++;
		i++;
		
	}

	return novo; // retorno o arquivo
}
// Funcao que recebe o arquivo e o numero do RRN atual e retorna o numero de bytes que faltam para chegar no proximo registro
int bytesRestantes(FILE* fp, int RRN){  

	long int posicao_atual = ftell(fp); // Posicao depois do registro inserido (tirando o cabecalho)

		if((posicao_atual % TAMANHOREGISTRO) != 0){ // Se não estivermos em um multiplo do tamanho do registro, temos que completar o registro

			// Byte Inicial do Proximo RRN = 112 *prox RRN - Tamanho do Cabeçalho
			// Retorna o Byte Inicial do proximo RRN - posicao atual

			return ((TAMANHOREGISTRO*(RRN+1)) - T_CABECALHO) - (posicao_atual); 
		}

}

//Funcao que receber o arquivo e um registro e escreve o registro no arquivo. Necessario saber o RRN
void EscreveRegistro(FILE* fp,Registro reg, int RRN){ 

	// Escrevo todo os campos do registro
	fwrite(&(reg.codEscola), sizeof(int), 1, fp);
	fwrite(&reg.dataInicio, sizeof(char), 10, fp);
	fwrite(&reg.dataFinal, sizeof(char), 10, fp);
	fwrite(&reg.indicador_tamanho_escola, sizeof(int), 1, fp);
	fwrite(reg.nomeEscola, sizeof(char), reg.indicador_tamanho_escola, fp);
	fwrite(&reg.indicador_tamanho_municipio, sizeof(int), 1, fp);
	fwrite(reg.municipio, sizeof(char), reg.indicador_tamanho_municipio, fp);
	fwrite(&reg.indicador_tamanho_endereco, sizeof(int), 1, fp);
	fwrite(reg.endereco, sizeof(char), reg.indicador_tamanho_endereco, fp);

	//Completo os 112 bytes restantes com 0;
	int nBytes = bytesRestantes(fp, RRN);
	char c = '0';

	fwrite(&c, sizeof(char), nBytes, fp); // Preenchendo o fim do arquivo

}

void arquivo_saida(Arquivo *entrada) {


	FILE * fp;
	fp = fopen("saida.bin", "wb");
	
	//Escreve o Registro de Cabeçalho
	
	fwrite(&entrada->status, sizeof(char), 1, fp);
	fwrite(&entrada->topo_pilha, sizeof(int), 1, fp);

	// Escrevo no Arquivo de Saida os Registros Validos
	for (int i = 0; i < entrada->n_registros_lidos; ++i){
		EscreveRegistro(fp, entrada->registros_lidos[i], i);
	}	
	
	fechaArquivo(fp);
	
}

// imprime o registro com o RRN passado
void ImprimeRegistro(FILE* fp, int RRN){ 
	
	int tamanho_arquivo = tamArquivo(fp);

	// Se o byte inicial do RRN pedido for maior que o tamanho do arquivo
	if(tamanho_arquivo < (RRN*TAMANHOREGISTRO+T_CABECALHO)){
		printf("Falha no processamento do arquivo\n");
		return;
	}
	// Le o primeiro byte e ve se o registro foi removido
	char c = fgetc(fp);
	fseek(fp, (RRN*TAMANHOREGISTRO+T_CABECALHO), SEEK_SET);

	if(c == '*'){
		return ;
	}

	//Imprime o Registro
	Registro* r = reg(fp, RRN);

	printf("%d %s %s %d %s %d %s %d %s\n", r->codEscola, r->dataInicio, r->dataFinal, r->indicador_tamanho_escola, r->nomeEscola, r->indicador_tamanho_municipio, r->municipio, r->indicador_tamanho_endereco, r->endereco);

}

void exibe_registros(){ // função 2

	FILE * fp;
	fp = fopen("saida.bin", "rb");
	int tamanho_arquivo = tamArquivo(fp);

	// Anda até o primeiro registro
	fseek(fp, T_CABECALHO, SEEK_SET); // Pula os 5 bytes de Registro de Cabeçalho


	for(int i = 0; ftell(fp) < tamanho_arquivo; i++){ // Enquanto houver registros para ler
		
		//Imprime o Registro e Pula para o proximo
		ImprimeRegistro(fp, i);
		proxRegistro(fp);
		
	}
	fechaArquivo(fp);
}

void func3_auxiliar(FILE* saida, char* nome_campo, char* val_campo){

	int b_inicial = T_CABECALHO + sizeof(int) + (2*T_DATA);
	int tamanho;
	char *str;

	
	int tamanho_arquivo = tamArquivo(fp);
	fseek(saida, T_CABECALHO, SEEK_SET); // PULA OS 5 BYTES DO REGISTRO DE CABEÇALHO


	if(strcmp("nomeEscola",nome_campo) == 0){  // byte 24 -> tam_nome escola + cabecalho


		for(int i = 0; ftell(saida) < tamanho_arquivo; i++){ // enquanto houver linhas linhas para ler

			b_inicial += TAMANHOREGISTRO*i;	

			fseek(saida,b_inicial,SEEK_SET);
			fread(&tamanho,sizeof(int),1,saida);

			str = query(saida,ftell(saida),tamanho);

			fseek(saida, (TAMANHOREGISTRO*i)+T_CABECALHO, SEEK_SET);
			if(strcmp(val_campo, str) == 0){
				ImprimeRegistro(saida, (TAMANHOREGISTRO*i)+T_CABECALHO);
			}	

			fseek(saida, (TAMANHOREGISTRO*(i+1))+T_CABECALHO, SEEK_SET);
			free(str);
		}

	}else if(strcmp("municipio",nome_campo) == 0){ 
		
		for(int i = 0; ftell(saida) < tamanho_arquivo; i++){ // enquanto houver linhas linhas para ler

			b_inicial += TAMANHOREGISTRO*i;	

			fseek(saida,b_inicial,SEEK_SET);
			fread(&tamanho,sizeof(int),1,saida);

			fseek(saida,tamanho,SEEK_CUR);
			fread(&tamanho,sizeof(int),1,saida);

			str = query(saida,ftell(saida),tamanho);

			fseek(saida, (TAMANHOREGISTRO*i)+T_CABECALHO, SEEK_SET);
			printf("%s --- %s\n",val_campo ,str);
			if(strcmp(val_campo, str) == 0){
				ImprimeRegistro(saida, (TAMANHOREGISTRO*i)+T_CABECALHO);
			}	

			fseek(saida, (TAMANHOREGISTRO*(i+1))+T_CABECALHO, SEEK_SET);
			free(str);
		}	

	}else if(strcmp("endereco",nome_campo) == 0){
		for(int i = 0; ftell(saida) < tamanho_arquivo; i++){ // enquanto houver linhas linhas para ler

			b_inicial += TAMANHOREGISTRO*i;	

			fseek(saida,b_inicial,SEEK_SET);
			fread(&tamanho,sizeof(int),1,saida);

			fseek(saida,tamanho,SEEK_CUR);
			fread(&tamanho,sizeof(int),1,saida);

			fseek(saida,tamanho,SEEK_CUR);
			fread(&tamanho,sizeof(int),1,saida);
			//printf("%d\n", tamanho);
			str = query(saida,ftell(saida),tamanho);

			fseek(saida, (TAMANHOREGISTRO*i)+T_CABECALHO, SEEK_SET);
			printf("%s --- %s\n",val_campo ,str);
			if(strcmp(val_campo, str) == 0){
				ImprimeRegistro(saida, (TAMANHOREGISTRO*i)+T_CABECALHO);
			}	

			fseek(saida, (TAMANHOREGISTRO*(i+1))+T_CABECALHO, SEEK_SET);
			free(str);
		}
	}





}

void func3(FILE* saida, char* nome_campo, char* val_campo){

	int b_inicial, tamanho;
	char FLAG_codEscola = 0; // 0 se não é nome da escola, 1 se é

	int codEscola_lido;
	int codEscola_pedido = atoi(val_campo);


	if(strcmp("codEscola",nome_campo) == 0){ // byte 0 + cabecalho
		b_inicial = T_CABECALHO; //byte inicial do codigo escola no primeiro registro
		
		FLAG_codEscola = 1;

	}else if(strcmp("dataInicio",nome_campo) == 0){ 
		b_inicial = T_CABECALHO + sizeof(int); // byte 4 + cabecalho
		tamanho = T_DATA;

	}else if(strcmp("dataFinal",nome_campo) == 0){   // byte 14 + cabecalho
		b_inicial = T_CABECALHO + sizeof(int) + T_DATA;
		tamanho = T_DATA; 

	}else{
		return func3_auxiliar(saida,nome_campo,val_campo);
	}

	fseek(saida, 0, SEEK_END);
	int tamanho_arquivo = ftell(saida);
	fseek(saida, 0, SEEK_SET);
	fseek(saida, T_CABECALHO, SEEK_SET); // PULA OS 5 BYTES DO REGISTRO DE CABEÇALHO


	for(int i = 0; ftell(saida) < tamanho_arquivo; i++){ // enquanto houver linhas linhas para ler


		b_inicial += TAMANHOREGISTRO*i; // byte inicial do campo pedido no registro atual		

		if(FLAG_codEscola == 1){ // se o campo pedido for o codigo da escola
			//leio do arquivo como um int	
			fread(&codEscola_lido, sizeof(int), 1, saida);	
			fseek(saida, (TAMANHOREGISTRO*i)+T_CABECALHO, SEEK_SET);
			if(codEscola_lido == codEscola_pedido){		
				ImprimeRegistro(saida, (TAMANHOREGISTRO*i)+T_CABECALHO);
			}

		}else{ 
			// se o campo pedido for outro
			// leio uma string
			char* str = query(saida, b_inicial, tamanho);
			
			// e comparo com o valor do campo pedido
			if(strcmp(val_campo, str) == 0){
				//imprime registro
				fseek(saida, (TAMANHOREGISTRO*i)+T_CABECALHO, SEEK_SET);
				ImprimeRegistro(saida, (TAMANHOREGISTRO*i)+T_CABECALHO);

			}	

			free(str);
		}

		fseek(saida, (TAMANHOREGISTRO*(i+1))+T_CABECALHO, SEEK_SET);
		
	}



}

char* query (FILE* fp, int b_inicial, int tamanho){


	fseek(fp, b_inicial, SEEK_SET);

	char* str = NULL;
	
	str = realloc(str, (tamanho+1) * sizeof(char)); // tam+1 para suportar o \0
	
	fread(str, sizeof(char),tamanho, fp);
	
	str[tamanho] ='\0';
	//printf("%s %d\n", str,tamanho);
	fseek(fp, b_inicial, SEEK_SET);
	return str;
}


void RemoveRegistro(FILE* saida, int RRN)
{
	fseek(saida, 0, SEEK_END);
	int tamanho_arquivo = ftell(saida);
	fseek(saida, 0, SEEK_SET);

	if(tamanho_arquivo <= (RRN*TAMANHOREGISTRO)+T_CABECALHO){
		printf("Registro Inexistente.\n"); 
		return;
	}

	int topo_pilha;
	fgetc(saida); //pulo o status do cabeçalho
	fread(&topo_pilha, sizeof(int), 1, saida);
	printf("topo_pilha = %d\n", topo_pilha);

	fseek(saida, (RRN*TAMANHOREGISTRO)+T_CABECALHO, SEEK_SET); // vou para o registro a ser removido

	char c = fgetc(saida);
	printf("c = %c\n", c);
	if(c == '*'){ // registo ja foi removido
		
	}else if(c == '*'){
		
		return ;
	}else{


		fseek(saida, (RRN*TAMANHOREGISTRO)+T_CABECALHO, SEEK_SET); // volto pro comeco do registro

		c = '*';
		fwrite(&c, sizeof(char), 1, saida); // escrevo o * para indicar que o registro foi removido
		fwrite(&topo_pilha, sizeof(int), 1, saida); // escrevo o topo da pilha no registro
		fseek(saida, 1, SEEK_SET); //acesso o topo da pilha
		fwrite(&RRN, sizeof(int), 1, saida); //escrevo o registro removido no topo da pilha

		printf("Registro Removido com Sucesso\n");
	}
}

void Insercao(FILE* saida, int codEscola,  char* dataInicio,  char* dataFinal,  char* nome_escola,  char* municipio,  char*endereco){

	fseek(saida, 1, SEEK_SET); // indo para o topo da pilha

	char aux[11] = "0000000000\0"; // string de 10 bytes para escrever as datas nulas
	int topo_pilha; // auxiliar que ira guardar o novo topo da pilha caso a insercao seja feita em cima de um registro removido anteriormente
	int RRN; // RRN onde sera inserido o novo registro
	fread(&RRN, sizeof(int), 1, saida); // leio do topo da pilha onde irei inserir
	int tamanho_arquivo;

	if(RRN == -1) {// se o valor lido for -1 entao devo inserir no final do arquivo
		fseek(saida, 0, SEEK_END); // vou para o final do arquivo
		tamanho_arquivo = ftell(saida); // guardo o tamanho atual do arquivo
	}else{ 	
	// senao
		// acesso o RRN lido
		fseek(saida, (RRN*TAMANHOREGISTRO)+T_CABECALHO, SEEK_SET);
		fgetc(saida); // leio o primeiro byte que vai ser um '*'
		fread(&topo_pilha, sizeof(int), 1, saida); // leio um inteiro guardado no registro removido que agora sera o novo topo da pilha

		//escrevo esse novo valor lido como o novo topo da pilha
		fseek(saida, 1, SEEK_SET); 
		fwrite(&topo_pilha, sizeof(int), 1, saida);

		// volto para onde inserirei o novo arquivo
		fseek(saida, (RRN*TAMANHOREGISTRO)+T_CABECALHO, SEEK_SET);


	}

	//inserindo o registro

	fwrite(&codEscola, sizeof(int), 1, saida);

	//escrevendo dataInicio
	if(strcmp(dataInicio, "0") == 0){ // se a data inicio for nula, inserindo a string aux
		fwrite(aux, sizeof(char), 10, saida);

	}else{
		fwrite(dataInicio, sizeof(char), 10, saida);
	}

	//escrevendo datafinal
	if(strcmp(dataFinal, "0") == 0){ // se a data final for nula , inserindo a string aux
		fwrite(aux, sizeof(char), 10, saida);

	}else{
		fwrite(dataFinal, sizeof(char), 10, saida);
	}

	// tamanho das strings lidas no terminal
	int tam_escola = strlen(nome_escola);
	int tam_municipio = strlen(municipio);
	int tam_endereco = strlen(endereco);

	// escrevo o tamanho e a strings de nome, municipio e endereco, respectivamente
	fwrite(&tam_escola, sizeof(int), 1, saida);
	fwrite(nome_escola, sizeof(char), tam_escola, saida);


	fwrite(&tam_municipio, sizeof(int), 1, saida);
	fwrite(municipio, sizeof(char), tam_municipio, saida);

	fwrite(&tam_endereco, sizeof(int), 1, saida);
	fwrite(endereco, sizeof(char), tam_endereco, saida);


	// calculo o numero de bytes restantes para preencher os 112 bytes do registro
	long int posicao_atual;
	int bytes_faltantes;

	posicao_atual = ftell(saida);

	// se o RRN for -1, entao estou inserindo no final do arquivo
	if(RRN == -1){
		if((posicao_atual % TAMANHOREGISTRO) != 0){ // se não estivermos em um multiplo do tamanho do registro, temos que completar o registro

				bytes_faltantes = ((TAMANHOREGISTRO+tamanho_arquivo) - (posicao_atual)); // qntd de bytes para preencher = tamanho do arquivo antes da insercao + tamanho do registro - a posicao atual

				fwrite(&aux, sizeof(char), bytes_faltantes, saida); // preenchendo o fim do arquivo
			}
		}else{
		// se for inserido em cima de um registro removido
		// entao

		if((posicao_atual % TAMANHOREGISTRO) != 0){ // se não estivermos em um multiplo do tamanho do registro, temos que completar o registro


				posicao_atual -= T_CABECALHO; // necessario para quando for inserir em cima de um registro ja removido
				bytes_faltantes = (TAMANHOREGISTRO*(RRN+1)) - (posicao_atual); // qntd de bytes para preencher = tamanho do registro * o numero do proximo RRN (RRN+1) - posicao final do registro atual;

				
				fwrite(&aux, sizeof(char), bytes_faltantes, saida); // preenchendo o fim do arquivo
			}
		}

/*	posicao_atual = ftell(saida);

	if(((posicao_atual-5) % 112) ==0)
	printf("ola\n");		
*/
	}

void updateRegistro(FILE* saida, int codEscola,  char* dataInicio,  char* dataFinal,  char* nome_escola,  char* municipio,  char*endereco, int RRN){


	char aux[11] = "0000000000\0";
	fseek(saida, (RRN*TAMANHOREGISTRO)+T_CABECALHO, SEEK_SET);
	fwrite(&codEscola, sizeof(int), 1, saida);

		//escrevendo dataInicio
		if(strcmp(dataInicio, "0") == 0){ // se a data inicio for nula, inserindo a string aux
			fwrite(aux, sizeof(char), 10, saida);

		}else{
			fwrite(dataInicio, sizeof(char), 10, saida);
		}

		//escrevendo datafinal
		if(strcmp(dataFinal, "0") == 0){ // se a data final for nula , inserindo a string aux
			fwrite(aux, sizeof(char), 10, saida);

		}else{
			fwrite(dataFinal, sizeof(char), 10, saida);
		}

		// tamanho das strings lidas no terminal
		int tam_escola = strlen(nome_escola);
		int tam_municipio = strlen(municipio);
		int tam_endereco = strlen(endereco);

		// escrevo o tamanho e a strings de nome, municipio e endereco, respectivamente
		fwrite(&tam_escola, sizeof(int), 1, saida);
		fwrite(nome_escola, sizeof(char), tam_escola, saida);


		fwrite(&tam_municipio, sizeof(int), 1, saida);
		fwrite(municipio, sizeof(char), tam_municipio, saida);

		fwrite(&tam_endereco, sizeof(int), 1, saida);
		fwrite(endereco, sizeof(char), tam_endereco, saida);


		// calculo o numero de bytes restantes para preencher os 112 bytes do registro
		long int posicao_atual;
		int bytes_faltantes;

		posicao_atual = ftell(saida);

		
		if((posicao_atual % TAMANHOREGISTRO) != 0){ // se não estivermos em um multiplo do tamanho do registro, temos que completar o registro
			posicao_atual -= T_CABECALHO; // necessario para quando for inserir em cima de um registro ja removido
			bytes_faltantes = (TAMANHOREGISTRO*(RRN+1)) - (posicao_atual); // qntd de bytes para preencher = tamanho do registro * o numero do proximo RRN (RRN+1) - posicao final do registro atual;	
			fwrite(&aux, sizeof(char), bytes_faltantes, saida); // preenchendo o fim do arquivo
		}
}

void func9(){
	FILE * arquivoDados = abreArquivo("saida.bin");
	int topo_pilha;
	fseek(arquivoDados,sizeof(char),SEEK_SET);
	fread(&topo_pilha, sizeof(int), 1, arquivoDados);

	while(topo_pilha != -1){

		printf("%d ", topo_pilha);
		fseek(arquivoDados,(topo_pilha*TAMANHOREGISTRO)+T_CABECALHO,SEEK_SET);
		fseek(arquivoDados,sizeof(char),SEEK_CUR);
		fread(&topo_pilha, sizeof(int), 1, arquivoDados);	
	}


	printf("\n");
	fechaArquivo(arquivoDados);
}

void func8(){

	FILE * arquivoDados = abreArquivo("saida.bin");
	FILE * saida = fopen("saida2.bin","wb");

	char status;
	char registro[112];
	int topo_pilha = -1;

	fread(&status, sizeof(char), 1, arquivoDados);	
	fseek(arquivoDados,sizeof(int),SEEK_CUR);

	fwrite(&status, sizeof(char), 1, saida);
	fwrite(&topo_pilha, sizeof(int), 1, saida);


	fseek(saida, 0, SEEK_END);
	int tamanho_arquivo = ftell(saida);
	fseek(saida, 0, SEEK_SET);
	fseek(saida, T_CABECALHO, SEEK_SET); 


	for(int i = 0; ftell(saida)< tamanho_arquivo; i++){

		if(existeReg(i,arquivoDados) == true){			
			fread(registro, sizeof(char), TAMANHOREGISTRO, arquivoDados);	
			fwrite(registro, sizeof(char), TAMANHOREGISTRO, saida);
		}

		fseek(arquivoDados,(TAMANHOREGISTRO*(i+1))+T_CABECALHO,SEEK_SET);
		fseek(saida,(TAMANHOREGISTRO*(i+1))+T_CABECALHO,SEEK_SET);
	}	


	fechaArquivo(arquivoDados);
	fechaArquivo(saida);
}



void fechaArquivo(FILE *fp){
	fclose(fp);
}

FILE * abreArquivo(char *nome_arquivo){
	FILE * fp;			
	fp = fopen(nome_arquivo, "r+");
	return fp;
}


bool existeReg(int RRN, FILE * fp){ //verifica se o registro é valido (isto é, não foi excluído)

	long int posicao_atual = ftell(fp);  					// guardamos a posicao atual do arquivo	
	fseek(fp,(RRN*TAMANHOREGISTRO)+T_CABECALHO,SEEK_SET);	// achamos o registro pedido

	char c;
	c = fgetc(fp);		// primeiro char do registro

	fseek(fp,posicao_atual,SEEK_SET);		// retornando o ponteiro do arquivo

	if(c == '*'){
		return false;
	}

	return true;

}


Registro *reg( FILE *fp, int RRN){ //retorna um registro no RRN passado
								 // OBS garantir que o registro existe e o ponteiro do arquivo é válido;

	long int posicao_atual = ftell(fp);
	fseek(fp, TAMANHOREGISTRO*RRN + T_CABECALHO, SEEK_SET);							 	

	Registro* reg = (Registro *)malloc(sizeof(Registro));

	fread(&(reg->codEscola), sizeof(int), 1, fp); // 4 bytes
	fread(reg->dataInicio, sizeof(char), 10, fp);  // 10*1 byte
	fread(reg->dataFinal, sizeof(char), 10, fp); // 10*1 byte

	reg->dataInicio[10] = reg->dataFinal[10] = '\0';

	fread(&(reg->indicador_tamanho_escola), sizeof(int), 1, fp); // 4 bytes
	reg->nomeEscola = realloc(reg->nomeEscola, (reg->indicador_tamanho_escola+1) * sizeof(char)); // REALLOCA A STRING COM O TAMANHO DO CAMPO +1 PARA CABER O \0
	fread(reg->nomeEscola, sizeof(char), (reg->indicador_tamanho_escola), fp); // tam_nome*1 byte

	fread(&(reg->indicador_tamanho_municipio), sizeof(int), 1, fp); // 4 bytes
	reg->municipio = realloc(reg->municipio, (reg->indicador_tamanho_municipio+1) * sizeof(char)); // REALLOCA A STRING COM O TAMANHO DO CAMPO +1 PARA CABER O \0
	fread(reg->municipio, sizeof(char), (reg->indicador_tamanho_municipio), fp); // tam_nome*1 byte

	fread(&(reg->indicador_tamanho_endereco), sizeof(int), 1, fp); // 4 bytes
	reg->endereco = realloc(reg->endereco, (reg->indicador_tamanho_endereco+1) * sizeof(char)); // REALLOCA A STRING COM O TAMANHO DO CAMPO +1 PARA CABER O \0
	fread(reg->endereco, sizeof(char), (reg->indicador_tamanho_endereco), fp); // tam_nome*1 byte

	fseek(fp, posicao_atual, SEEK_SET);	 // voltamos para posicao inicial da funcao

	return reg;
}

long int tamArquivo(FILE *fp){ // retorna o tamanho total do arquivo

	long int posicao_atual = ftell(fp); // posicao atual do arquivpo

	fseek(fp, 0, SEEK_END); 
	long int tamanho_arquivo = ftell(fp);  

	fseek(fp, posicao_atual, SEEK_SET); // voltando posicao atual do arquivo

	return tamanho_arquivo;
}

bool proxRegistro(FILE *fp){ // avança um registro no arquivo "fp"
				
	// obs: o ponteiro do arquivo deve estar no primeiro byte de um registro !!!

	fseek(fp, TAMANHOREGISTRO, SEEK_CUR); // somamos o tamanho do registro no registro atual

	if(ftell(fp) <= tamArquivo(fp)){

		return true;
	}else{

		return false;
	}
}


void free_reg(Registro *reg){

	if(reg == NULL){
		printf("O registro não existe :(\n");
	}else{
		if(reg->nomeEscola != NULL)
			free(reg->nomeEscola);
		if(reg->municipio != NULL)
			free(reg->municipio);
		if(reg->endereco != NULL)
			free(reg->endereco);
		free(reg);
	}	

}		