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
	
	printf("%s%c \n", palavra_lida,atual);

	return palavra_lida;
}

Arquivo le_dados(char * nome_arquivo) {

	FILE * pa = NULL;


	Arquivo novo;

	//SETANDO O REGISTRO DE CABEÇALHO
	novo.status = '1';
	novo.topo_pilha =-1;

	int n_registros_lidos = 0;
	char auxiliar;
	pa = fopen(nome_arquivo, "r");
	fseek(pa, 0, SEEK_END);
	int tamanho_arquivo = ftell(pa);
	fseek(pa, 0, SEEK_SET);
	novo.registros_lidos = NULL;
	if(pa == NULL){
		printf("Erro ao abrir o arquivo, lamento !\n");
		exit(0);
	}

	while (ftell(pa) < tamanho_arquivo){

		printf("to aqui meu amor\n");
		novo.registros_lidos = realloc(novo.registros_lidos, (n_registros_lidos + 1) * sizeof(Registro));
		
		// acessa o Arquivo no vetor de registros e salva em cada elemento
		// da struct o que lhe é destinado

		fscanf(pa,"%d",&((novo.registros_lidos + n_registros_lidos)->codEscola));
		fgetc(pa);

		// substituido
		// fread(&((novo.registros_lidos + n_registros_lidos)->codEscola), 1, sizeof(int), pa);

		(novo.registros_lidos + n_registros_lidos)->nome_escola = 
		le_tamanho_variavel(pa, &((novo.registros_lidos + n_registros_lidos)->indicador_tamanho_escola));
		(novo.registros_lidos + n_registros_lidos)->municipio = 
		le_tamanho_variavel(pa, &((novo.registros_lidos + n_registros_lidos)->indicador_tamanho_municipio));
		(novo.registros_lidos + n_registros_lidos)->endereco = 
		le_tamanho_variavel(pa, &((novo.registros_lidos + n_registros_lidos)->indicador_tamanho_endereco));

		auxiliar = fgetc(pa);

		if (auxiliar == ';') {

			strcpy(((novo.registros_lidos) + n_registros_lidos)->data_inicio, "0000000000\0");
			printf("data ini bugou%s\n", (novo.registros_lidos + n_registros_lidos)->data_inicio);

		} else {

			(novo.registros_lidos + n_registros_lidos)->data_inicio[0] = auxiliar;
			fread((novo.registros_lidos + n_registros_lidos)->data_inicio+1, 1, 9, pa);	
			(novo.registros_lidos + n_registros_lidos)->data_inicio[10] = '\0';
			fgetc(pa);	
			printf("data ini %s\n", (novo.registros_lidos + n_registros_lidos)->data_inicio);
		}


		auxiliar = fgetc(pa);

		if (auxiliar == '\n') {	
			strcpy(((novo.registros_lidos) + n_registros_lidos)->data_final, "0000000000\0");
			printf("data fim bugou %s\n", (novo.registros_lidos + n_registros_lidos)->data_final);	

		} else {

			(novo.registros_lidos + n_registros_lidos)->data_final[0] = auxiliar;
			fread((novo.registros_lidos + n_registros_lidos)->data_final+1, 1, 9, pa);	
			((novo.registros_lidos + n_registros_lidos)->data_final[10]) = '\0';
			auxiliar = fgetc(pa);
			printf("data fim %s\n", (novo.registros_lidos + n_registros_lidos)->data_final);
		}	


		n_registros_lidos++;
		
	}


	novo.n_registros_lidos = n_registros_lidos;
	return novo;
}


void  arquivo_saida(Arquivo *entrada) {


	FILE * saida,*saida1;


	saida = fopen("saida.bin", "wb");
	//cabecalho
	fwrite(&entrada->status, sizeof(char), 1, saida);
	fwrite(&entrada->topo_pilha, sizeof(int), 1, saida);

	long int posicao_atual;
	int bytes_faltantes;
	char c = '0';

	for (int i = 0; i < entrada->n_registros_lidos; ++i){
		
		fwrite(&(entrada->registros_lidos[i].codEscola), sizeof(int), 1, saida);
		fwrite(&entrada->registros_lidos[i].data_inicio, sizeof(char), 10, saida);
		fwrite(&entrada->registros_lidos[i].data_final, sizeof(char), 10, saida);
		fwrite(&entrada->registros_lidos[i].indicador_tamanho_escola, sizeof(int), 1, saida);
		fwrite(entrada->registros_lidos[i].nome_escola, sizeof(char), entrada->registros_lidos[i].indicador_tamanho_escola, saida);
		fwrite(&entrada->registros_lidos[i].indicador_tamanho_municipio, sizeof(int), 1, saida);
		fwrite(entrada->registros_lidos[i].municipio, sizeof(char), entrada->registros_lidos[i].indicador_tamanho_municipio, saida);
		fwrite(&entrada->registros_lidos[i].indicador_tamanho_endereco, sizeof(int), 1, saida);
		fwrite(entrada->registros_lidos[i].endereco, sizeof(char), entrada->registros_lidos[i].indicador_tamanho_endereco, saida);
		
		posicao_atual = ftell(saida) - T_CABECALHO; // posicao depois do registro inserido (tirando o cabecalho)

		if((posicao_atual % TAMANHOREGISTRO) != 0){ // se não estivermos em um multiplo do tamanho do registro, temos que completar o registro

			bytes_faltantes = (TAMANHOREGISTRO*(i+1)) - (posicao_atual); // qntd de bytes para preencher = tamanho do registro - posicao final do registro atual;

			fwrite(&c, sizeof(char), bytes_faltantes, saida); // preenchendo o fim do arquivo
		}

		// fprintf(saida1, "%d", entrada->registros_lidos[i].codEscola); // gravando codigo da escola	
 	// 	fprintf(saida1, "%s%s", entrada->registros_lidos[i].data_inicio,entrada->registros_lidos[i].data_final); // gravando as datas	
 	// 	fprintf(saida1, "%d", entrada->registros_lidos[i].indicador_tamanho_escola); // gravando indicador de tamanho do nome da escola
 	// 	fprintf(saida1, "%s", entrada->registros_lidos[i].nome_escola); // gravando nome da escola
 	// 	fprintf(saida1, "%d", entrada->registros_lidos[i].indicador_tamanho_municipio); // gravando indicador de tamanho do nome da escola
 	// 	fprintf(saida1, "%s", entrada->registros_lidos[i].municipio); // gravando nome da escola
 	// 	fprintf(saida1, "%d", entrada->registros_lidos[i].indicador_tamanho_endereco); // gravando indicador de tamanho do nome da escola
 	// 	fprintf(saida1, "%s", entrada->registros_lidos[i].endereco); // gravando nome da escola

	}	
	
	fclose(saida);
	
}

void exibe_registros(){ // função 2


	FILE * saida;
	saida = fopen("saida2.bin", "rb");
	
	fseek(saida, 0, SEEK_END);
	int tamanho_arquivo = ftell(saida);
	fseek(saida, 0, SEEK_SET);

	fseek(saida, T_CABECALHO, SEEK_SET); // PULA OS 5 BYTES DO REGISTRO DE CABEÇALHO


	for(int i = 0; ftell(saida) < tamanho_arquivo; i++){ // enquanto houver linhas linhas para ler
		
		ImprimeRegistro(saida,ftell(saida));
		fseek(saida, TAMANHOREGISTRO, SEEK_CUR);
		
	}


	fclose(saida);
}

void func3_auxiliar(FILE* saida, char* nome_campo, char* val_campo){

	int b_inicial = T_CABECALHO + sizeof(int) + (2*T_DATA);
	int tamanho;
	char *str;

	fseek(saida, 0, SEEK_END);
	int tamanho_arquivo = ftell(saida);
	fseek(saida, 0, SEEK_SET);
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

void ImprimeRegistro(FILE* saida, int b_inicial){ // imprime o registro do byte inicial passado por argumento e retorna o ponteiro para este byte
	fseek(saida, 0, SEEK_END);
	int tamanho_arquivo = ftell(saida);
	fseek(saida, 0, SEEK_SET);

	if(tamanho_arquivo <= b_inicial){
		printf("Falha no processamento do arquivo\n");
		return;
	}

	fseek(saida, b_inicial, SEEK_SET);
	char c = fgetc(saida);
	fseek(saida, b_inicial, SEEK_SET);

	/*if(c == '*'){
		return ;
	}*/
	int codEscola; 
	
	char data_ini[11]; // OS 10 BYTES DA DATA MAIS O \0
	char data_fim[11];
	
	int tam_nome;
	char* nome_escola = NULL; 
	
	int tam_municipio;
	char* municipio = NULL;
	
	int tam_endereco;
	char* endereco = NULL;

	fread(&codEscola, sizeof(int), 1, saida); // 4 bytes
	fread(data_ini, sizeof(char), 10, saida);  // 10*1 byte
	fread(data_fim, sizeof(char), 10, saida); // 10*1 byte

	data_ini[10] = data_fim[10] = '\0';

	fread(&tam_nome, sizeof(int), 1, saida); // 4 bytes
	nome_escola = realloc(nome_escola, (tam_nome+1) * sizeof(char)); // REALLOCA A STRING COM O TAMANHO DO CAMPO +1 PARA CABER O \0
	fread(nome_escola, sizeof(char), tam_nome, saida); // tam_nome*1 byte

	fread(&tam_municipio, sizeof(int), 1, saida); // 4 bytes
	municipio = realloc(municipio, (tam_municipio+1) * sizeof(char)); // REALLOCA A STRING COM O TAMANHO DO CAMPO +1 PARA CABER O \0
	fread(municipio, sizeof(char), tam_municipio, saida);  // tam_municipio*1 byte

	fread(&tam_endereco, sizeof(int), 1, saida);
	endereco = realloc(endereco, (tam_endereco+1) * sizeof(char)); // REALLOCA A STRING COM O TAMANHO DO CAMPO +1 PARA CABER O \0	
	fread(endereco, sizeof(char), tam_endereco, saida); // tam_endereco*1 byte

	printf("%d %s %s %d %s %d %s %d %s\n", codEscola,data_ini,data_fim,tam_nome,nome_escola,tam_municipio, municipio, tam_endereco, endereco);

	fseek(saida, b_inicial, SEEK_SET);

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


Registro *reg(int RRN, FILE *fp){ //retorna um registro no RRN passado
								 // OBS garantir que o registro existe e o ponteiro do arquivo é válido;

	long int posicao_atual = ftell(fp);
	fseek(fp, TAMANHOREGISTRO*RRN + T_CABECALHO, SEEK_SET);							 	

	Registro* reg = (Registro *)malloc(sizeof(Registro));

	fread(&(reg->codEscola), sizeof(int), 1, saida); // 4 bytes
	fread(reg->data_inicio, sizeof(char), 10, saida);  // 10*1 byte
	fread(reg->data_fim, sizeof(char), 10, saida); // 10*1 byte

	reg->data_ini[10] = reg->data_fim[10] = '\0';

	fread(&(reg->indicador_tamanho_escola), sizeof(int), 1, saida); // 4 bytes
	reg->nome_escola = realloc(reg->nome_escola, (reg->indicador_tamanho_escola+1) * sizeof(char)); // REALLOCA A STRING COM O TAMANHO DO CAMPO +1 PARA CABER O \0
	fread(reg->nome_escola, sizeof(char), (reg->indicador_tamanho_escola), saida); // tam_nome*1 byte

	fread(&(reg->indicador_tamanho_municipio), sizeof(int), 1, saida); // 4 bytes
	reg->municipio = realloc(reg->municipio, (reg->indicador_tamanho_municipio+1) * sizeof(char)); // REALLOCA A STRING COM O TAMANHO DO CAMPO +1 PARA CABER O \0
	fread(reg->municipio, sizeof(char), (reg->indicador_tamanho_municipio), saida); // tam_nome*1 byte

	fread(&(reg->indicador_tamanho_endereco), sizeof(int), 1, saida); // 4 bytes
	reg->endereco = realloc(reg->endereco, (reg->indicador_tamanho_endereco+1) * sizeof(char)); // REALLOCA A STRING COM O TAMANHO DO CAMPO +1 PARA CABER O \0
	fread(reg->endereco, sizeof(char), (reg->indicador_tamanho_endereco), saida); // tam_nome*1 byte

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

	if(ftell(fp) < tamArquivo(fp)){

		return true;
	}else{

		return false;
	}
}


void free_reg(Registro *reg){

	if(reg == NULL){
		printf("O registro não existe :(\n");
	}else{
		free(reg->nome_escola);
		free(reg->municipio);
		free(reg->endereco);
		free(reg);
	}	

}		