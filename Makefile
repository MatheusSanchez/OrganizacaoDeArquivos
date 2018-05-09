# Compilador gcc
CP = gcc
# Parâmetros de compilação
P_CO = -c -lm -g3 -std=gnu99

main: manipula_arquivo.o  main.o 
	$(CP) manipula_arquivo.o main.o -o main 

all: manipula_arquivo.o  main.o 
	$(CP) manipula_arquivo.o main.o -o main 

manipula_arquivo.o: manipula_arquivo.c
	$(CP) $(P_CO) manipula_arquivo.c

main.o: main.c
	$(CP) $(P_CO) main.c


run: main
	./main 1 entrada.csv

run2: main
	./main 2

run_codEscola: main
	./main 3 'codEscola' 35000024

run_dataInicio: main
	./main 3 'dataInicio' 01/02/2012

run_dataFinal: main
	./main 3 'dataFinal' 25/12/2012

run_nomeEscola: main
	./main 3 'nomeEscola' 'GAVIAO PEIXOTO BRIGADEIRO'

run_nomeMunicipio: main
	./main 3 'municipio' 'OSASCO'

run_nomeEndereco: main
	./main 3 'endereco' 'RUA MOGEIRO'		

run_RRN: main
	./main 4 1	

run5: main
	./main 5 1

run6: main
	./main 6 49678012 0 0 'EE DISCIPLINA' 'SAO CARLOS' ''

run7: main
	./main 7 1 35000012 12/12/0012 0 'EE DISCIPLINA' 'SAO CARLOS' '123456'

run8: main
	./main 8

run9: main
	./main 9

clean: 
	rm -rf *o main *bin

clean_all:	
	rm -rf *o
