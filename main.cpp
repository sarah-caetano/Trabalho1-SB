#include "declaracoes.h"

int main(int argc, char *argv[]){
	montador* Montador = new montador("str");
	string label;
	vector <string> tokens;
	int i, posicao;

	ofstream saida; // escreve no arquivo de saida
	saida.open(argv[3]);
	
	ifstream arquivo(argv[2]); // lê do arquivo passado na linha de comando

	if(arquivo.is_open()){
		while(!arquivo.eof()){
			getline(arquivo, Montador->linha_inicial);
			Montador->processaLinha(); // processa todas as linhas
		}		
		arquivo.close();
	}	
	
	Montador->faz_listas();// coloca no vetor de diretivas e instrucoes
	if (Montador->faltaENDMACRO() == 0){
		if (strcmp(argv[1], "-m") == 0){ // processamento da MACRO
			Montador->is_m = 1;			
			Montador->processaMACRO();
		}
		else if (strcmp(argv[1], "-p") == 0){ // processamento EQU e IF
			Montador->processaEQU();
		}
		else if (strcmp(argv[1], "-o") == 0){ // TRADUTOR DE 2 passagens
			Montador->is_tradutor = 1;
			Montador->primeira_passagem();
			if (Montador->simbolo_errado == 0 &&  Montador->token_errado == 0 && Montador->doisRotulos == 0 && Montador->rotulo_igual == 0){ // Não tem tokens nem simbolos invalidos nem dois rótulos na msma linha
				Montador->segunda_passagem(); // traduz o código
			}
		}
	}
	else Montador->gera_linhas();
	for (i = 0; i < Montador->linhas.size(); i++)
		saida << Montador->linhas[i];

	saida.close();	
	return 0;
}
