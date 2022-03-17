#include "declaracoes.h"

montador::montador(string str){
    this->file_path = str;      // file_path da instancia recebe o file_path informado ao instanciar o objeto
	this->label_macro; 
	this->linhas;
	this->tokens;
	this->macro;
	this->LabelEQUS;
	this->ValorEQUS;
	this->listaCONSTSPACE;
};
void montador::caixa_alta(){ // devolve toda a linha em caixa alta
	int i;
	for (i = 0; i < this->linha_inicial.length(); i++){
		this->linha_inicial[i] = toupper(this->linha_inicial[i]);
	}
};
void montador::remove_comentarios(){
	this->linha_inicial = this->linha_inicial.substr(0, linha_inicial.find(';'));
};
void montador::remove_espacosDesnecessarios(string linha_temporaria){ // remove espacos, tabulacoes
	string token;
	istringstream iss(linha_temporaria); // retira os espacos e gera os tokens
	linha_temporaria = "";
	while (iss >> token){
		if (token.find(',') == string::npos){ 	// ignora a virgula do COPY
			this->tokens.push_back(token); 		// coloca no vetor de strings TOKENS
			linha_temporaria += token + " "; 
		}
		else{
			this->tokens.push_back(token.substr(0, token.size()-1));
			linha_temporaria += token + " "; // coloca na linha temporária o token
		}
	}
	linha_temporaria = linha_temporaria.substr(0, linha_temporaria.size()-1); // remove o espaço extra do final
	this->linha_temporaria = linha_temporaria;
};
void montador::processaLinha(){
	this->caixa_alta(); // Devolve linha em maiúsculo
	this->remove_comentarios();
	this->remove_espacosDesnecessarios(this->linha_inicial);
};
void montador::faz_listas(){ // setar as listas de diretivas e instrucoes
	int i;
	this->diretivas.push_back("SPACE");		this->diretivas.push_back("MACRO");		this->diretivas.push_back("EQU");
    this->diretivas.push_back("CONST");		this->diretivas.push_back("ENDMACRO");	this->diretivas.push_back("IF");
    this->instrucoes.push_back("ADD"); 		this->opcode.push_back("1");
    this->instrucoes.push_back("SUB");		this->opcode.push_back("2");
    this->instrucoes.push_back("MULT"); 	this->opcode.push_back("3");
    this->instrucoes.push_back("DIV"); 		this->opcode.push_back("4");
    this->instrucoes.push_back("JMP"); 		this->opcode.push_back("5");
    this->instrucoes.push_back("JMPN"); 	this->opcode.push_back("6");
    this->instrucoes.push_back("JMPP"); 	this->opcode.push_back("7");
    this->instrucoes.push_back("JMPZ"); 	this->opcode.push_back("8");
    this->instrucoes.push_back("COPY");		this->opcode.push_back("9");
    this->instrucoes.push_back("LOAD");		this->opcode.push_back("10");
    this->instrucoes.push_back("STORE");	this->opcode.push_back("11");
    this->instrucoes.push_back("INPUT");	this->opcode.push_back("12");
    this->instrucoes.push_back("OUTPUT");	this->opcode.push_back("13");
    this->instrucoes.push_back("STOP");		this->opcode.push_back("14");
};

void montador::faz_tabela_simbolos(string label, int endereco){
	int i;
	label = label.substr(0, label.find(':')); // tira os :
	for (i = 0; i < this->simbolo.size(); i++){
		if (label.compare(this->simbolo[i]) == 0){
			cout << "simbolo[i] " << this->simbolo[i] << '\n';
				linhas += "ERRO SEMANTICO: ROTULOS REPETIDOS NA LINHA " + to_string(endereco) + '\n';
				this->rotulo_igual = 1;
		}
	}
	this->simbolo.push_back(label);
	this->posicao_simbolo.push_back(endereco);
};

void montador::token_invalido(string token, string linha){ 
	linhas += token + '\n';
	linhas += "Erro LEXICO no token " + token + " na linha " + linha + "\n";
	this->token_errado = 1; // nao deixa ir pra segunda passagem
};

void montador::simbolos_invalidos(){
	int i, j, token, instrucao, diretiva, endereco = 0, achou, label, simbolo, aux;
	vector <int> contador;

	for (token = 0; token < this->tokens.size(); token++){
		achou = 0;
		if (this->tokens[token].find(":") != string::npos){ // se o token for um rótulo, ignora
			achou = 1;
		} 
		for (instrucao = 0; instrucao < this->instrucoes.size(); instrucao++){
			if (this->tokens[token].compare(this->instrucoes[instrucao]) == 0){
				achou = 1;
				if (this->tokens[token].compare("COPY") == 0){
					contador.push_back(endereco); // coloca no contador 
					endereco += 3; // pula 3 enderecos de memoria
				}
				else if (this->tokens[token].compare("STOP") == 0){
					contador.push_back(endereco);
					endereco += 1; // pula 1 endereco de memoria
				}
				else{ // instrucoes normais com 1 argumento
					contador.push_back(endereco);
					endereco += 2; // pula 2 enderecos de memoria
				}
			}
		}

		for (diretiva = 0; diretiva < this->diretivas.size(); diretiva++){
			if (this->tokens[token].compare(this->diretivas[diretiva]) == 0){
				achou = 1;
				contador.push_back(endereco);
				endereco += 1; // pula 1 endereco de memoria
				if (this->tokens[token].compare("MACRO") == 0 || this->tokens[token].compare("ENDMACRO") == 0 || this->tokens[token].compare("EQU") == 0 || this->tokens[token].compare("IF") == 0)
					endereco -= 1; // se for alguma dessas diretivas, não pula o espaço na memoria
				if (this->tokens[token].compare("CONST") == 0){
					token++;
				}
			}
		}
		for (simbolo = 0; simbolo < this->simbolo.size(); simbolo++){ // lista de simbolos
			if (this->tokens[token].compare(this->simbolo[simbolo]) == 0){
				achou = 1;
			}
		}
		cout << " contador = [";
		for (aux = 0; aux < contador.size(); aux++)
			cout << contador[aux] << " ";
		cout << "]\n";

		if (achou == 0){
			cout << "nao encontrou essa label: " << this->tokens[token] << '\n';
			this->label_invalida.push_back(this->tokens[token]);
			this->label_invalida.push_back(to_string(contador[contador.size()-1]));
		}
	}
	if (this->label_invalida.size() != 0){
		for (i = 0; i < this->label_invalida.size(); i+=2)
			this->linhas += "Erro LEXICO na label '" + this->label_invalida[i] + "' na linha " + label_invalida[i+1] + '\n';

		this->simbolo_errado = 1;
	}
};

void montador::proximo_token_rotulo(string token){ 
	if (token.find(":") != string::npos)
		this->doisRotulos = 1;
};

void montador::gera_linhas(){
	int i, k, token, instrucao, diretiva, endereco = 0, aux, achou = 0;
	vector <int> contador; // contador de linhas

	for (token = 0; token < this->tokens.size(); token++){
		achou = 0;
		if (this->tokens[token].find(":") != string::npos){ // se o token for um rótulo 
			achou = 1;
			if (this->is_tradutor == 1){
				this->proximo_token_rotulo(this->tokens[token+1]);
				if(this->doisRotulos == 0){
					this->faz_tabela_simbolos(this->tokens[token], endereco); // salva na tabela de simbolos a label e o endereco
				}
				else{
					contador.push_back(endereco);
					linhas += "ERRO LEXICO: 2 ROTULOS NA MESMA LINHA -> LINHA " + to_string(contador[contador.size()-1]);
					break;
				}
			}
			else{
				this->linhas += this->tokens[token] + " ";
			}
		}
		else{		
			if (this->tokens[token].compare("IF") == 0 && this->erroIFsemEQ == 1 && this->sem_endmacro == 0 ){
					this->linhas += "ERRO SEMANTICO: NAO EXISTE " + this->tokens[token+1]+ '\n';
				break;
			}	
			if (this->tokens[token].compare(this->label_macro) == 0){
				this->linhas += this->tokens[token] + "\n";
				achou = 1;
			}
			for (instrucao = 0; instrucao < this->instrucoes.size(); instrucao++){
				if (this->tokens[token].compare(this->instrucoes[instrucao]) == 0){
					achou = 1;
					if (this->tokens[token].compare("COPY") == 0){
						contador.push_back(endereco); // coloca no contador
						this->linhas += this->tokens[token] + " ";
							this->linhas += this->tokens[token+1] + " " + this->tokens[token+2] + "\n"; // coloca no vetor de linhas do codigo objeto de saida
							token += 2; // pula 2 tokens -> VAI PRO OUTRO TOKEN POR CAUSA DO INCREMENTO DO LOOP
							endereco += 3; // pula 3 enderecos de memoria
					}
					else if (this->tokens[token].compare("STOP") == 0){ 
						contador.push_back(endereco);
						endereco += 1; // pula 1 endereco de memoria
						this->linhas += this->tokens[token] + "\n";
					}
					else{ // instrucoes normais com 1 argumento
						contador.push_back(endereco);
						endereco += 2; // pula 2 enderecos de memoria
						this->linhas += this->tokens[token] + " " + this->tokens[token+1] + "\n";
						token += 1; // pula 1 token
						
					}
				}
			}
			for (diretiva = 0; diretiva < this->diretivas.size(); diretiva++){
				if (this->tokens[token].compare(this->diretivas[diretiva]) == 0){
					achou = 1;
					contador.push_back(endereco);
					endereco += 1; // pula 1 endereco de memoria
					if (this->tokens[token].compare("MACRO") == 0 || this->tokens[token].compare("ENDMACRO") == 0 || this->tokens[token].compare("EQU") == 0 || this->tokens[token].compare("IF") == 0)
						endereco -= 1; // se for alguma dessas diretivas, não pula o espaço na memoria
					
					if (this->tokens[token].compare("CONST") == 0 || this->tokens[token].compare("EQU") == 0 || this->tokens[token].compare("IF") == 0){
						if (this->is_tradutor == 1){
							token++;
							this->listaCONSTSPACE += this->tokens[token] + " "; // e o seu valor logo depois ONE: CONST 1 -> 1
						}
						else
							this->linhas += this->tokens[token] + " " + this->tokens[token+1] + "\n"; // ONE: CONST 1
					}
					else if (this->tokens[token].compare("SPACE") == 0 || this->tokens[token].compare("MACRO") == 0 || this->tokens[token].compare("ENDMACRO") == 0){
						if (this->is_tradutor == 1){
							this->listaCONSTSPACE += "0 "; // AUX: SPACE -> 0
						}
						else 
							this->linhas += this->tokens[token] + "\n"; // AUX: SPACE
					}
				}
			}
		}
		if (this->is_tradutor == 1 && achou == 0){
			contador.push_back(endereco); // coloca no contador a linha do erro
			this->token_invalido(this->tokens[token], to_string(contador[contador.size()-1]));
			break; // sai do for
		}
	}
	if (this->is_tradutor == 1 && this->erroIFsemEQ == 0 && this->simbolo_errado == 0 && this->rotulo_igual == 0)
		this->linhas += this->listaCONSTSPACE + '\n'; // coloca os espaços reservados e as constantes no final do código objeto (após o OPCODE de stop)
};

void montador::substituiMACRO(){
	int i;
	for (i = 0; i < this->tokens.size(); i++){
		if (this->tokens.at(i).compare(this->label_macro) == 0){
			this->tokens.erase(this->tokens.begin()+i); // apaga a label
			this->tokens.insert(this->tokens.begin()+i, this->macro.begin(), this->macro.end()); // coloca a macro no código de tokens
		}
	}
};

int montador::faltaENDMACRO(){
	int i, j;
	for (i = 0; i < this->tokens.size(); i++){
		if(this->tokens[i].compare("MACRO") == 0){ // se existe macro
			for (j = 0; j < this->tokens.size(); j++){ // compara o resto dos tokens pra ver se existe a endmacro
				if (this->tokens[j].compare("ENDMACRO") == 0){
					return 0;
				}
			}
			this->sem_endmacro = 1;
			this->linhas += "ERRO SEMANTICO: FALTA DE ENDMACRO\n"; // se não encontrou
			return 1;
		}
	}
	return 0;
}

int montador::MACROinutil(){
	int i;
	for (i = 0; i < this->tokens.size(); i++){
		if (this->tokens.at(i).compare(this->label_macro) == 0){
			return 0;
		}
	}
	linhas += "ERRO SEMANTICO: MACRO NAO UTILIZADA\n";
	return 1;
}

void montador::processaMACRO(){
	int i, macro = 0;
	vector <int> aux; // variável q armazena as posições no vetor de tokens do início e do final da macro

	for (i = 0; i < this->tokens.size(); i++){

		if (macro == 1){
			this->macro.push_back(this->tokens[i]);
		}
		if (this->tokens.at(i).compare("MACRO") == 0){
			macro = 1;
			this->label_macro = this->tokens.at(i-1); // pegar a label da macro
			this->label_macro = this->label_macro.substr(0, this->label_macro.find(':')); // tira os :
			aux.push_back(i-1); // inicio da macro -> -1 pra pegar o label dela
		}
		if (this->tokens.at(i).compare("ENDMACRO") == 0){
			macro = 0; // para de colocar os tokens na string da macro
			aux.push_back(i+1); // fim da macro -> +1 pq erase retira até o último elemento antes da borda
		}

	}
	if (this->macro.size() != 0){
		this->macro.pop_back(); // tira o endmacro da lista de macro
		this->tokens.erase(this->tokens.begin()+aux[0], this->tokens.begin()+aux[1]); // tira a macro dos tokens
		if (this->MACROinutil() == 0){
			this->substituiMACRO();
		}
	}
	if (this->is_tradutor == 0) // não está traduzindo
		this->gera_linhas();
};
void montador::removeEQU(){
	int i;
	for (i = 0; i < this->tokens.size(); i++){
		if (this->tokens.at(i).compare("EQU") == 0){
			this->LabelEQUS.push_back(this->tokens[i-1]);
			this->ValorEQUS.push_back(this->tokens[i+1]);
			this->tokens.erase(this->tokens.begin()+(i-1), this->tokens.begin()+(i+2));
			i=0; // tirou um equ, renova o contador -> as this->diretivas EQU SEMPRE virão no início do programa
		}
	}	
	for (i = 0; i < this->LabelEQUS.size(); i++)
		this->LabelEQUS[i] = this->LabelEQUS[i].substr(0, this->LabelEQUS[i].find(':')); // tira os :
};
void montador::substituiValorEQU(){
	int i, j;
	for (i = 0; i < this->tokens.size(); i++){
		for (j = 0; j < this->LabelEQUS.size(); j++){
			if(this->tokens[i].compare(this->LabelEQUS[j]) == 0){
				this->tokens[i] = this->ValorEQUS[j];
			}
		}
	}
};

int montador::IFsemEQU(string label){
	int i, labelExiste = 0;

	if (this->is_m == 1 || this->is_tradutor == 1){ // tem q fazer a lista aqui dentro nessas ocasiões, pq n tem q processar EQUeIF
		for (i = 0; i < this->tokens.size(); i++){
			if (this->tokens.at(i).compare("EQU") == 0)
				this->LabelEQUS.push_back(this->tokens[i-1]);
		}
		for (i = 0; i < this->LabelEQUS.size(); i++){
			this->LabelEQUS[i] = this->LabelEQUS[i].substr(0, this->LabelEQUS[i].find(':'));
		}
	}
	for (i = 0; i < this->LabelEQUS.size(); i++){
		if(label == this->ValorEQUS[i]){ // compara se o token está na tabela de valor dos equ, se não tiver, é pq n estava na tabela de labels
			this->erroIFsemEQ = 0;
			return 0; // se estiver, pode deixar
		}
	}	
	this->erroIFsemEQ = 1;
	return 1;		
};

void montador::EQUinutil(){
	int i, j, achouUtilidade = 0;
	vector <string> inuteis;
	
	for (j = 0; j < this->LabelEQUS.size(); j++){
		for (i = 0; i < this->tokens.size(); i++){
			if(this->LabelEQUS[j].compare(this->tokens[i]) == 0)
				achouUtilidade = 1;
		}
		if (achouUtilidade == 0){
			inuteis.push_back(this->LabelEQUS[j]); // salva as labels não utilizadas no código todo
		}
		else achouUtilidade = 0; // para continuar analisando as outras labels
	}
	if (inuteis.size() != 0){ // labels não utilizada em nenhuma parte do código
		linhas += "ERRO SEMANTICO: ";
		if (inuteis.size() == 1){
			linhas += "ROTULO ";
		} else linhas += "ROTULOS ";
		
		for (i = 0; i < inuteis.size(); i++){
			linhas +=  " '" + inuteis[i] + "' ";
		}
		if (inuteis.size() == 1){
			linhas += "NAO UTILIZADO\n";
		} else linhas += "NAO UTILIZADOS\n";
	}
};

void montador::processa_labelsEQU(){ 
	// pega labels a partir dos tokens e substitui no código
	this->removeEQU(); // remove os equs do código e salva o label e o valor dela

	this->EQUinutil(); // verifica o erro de não existir a label de algum EQU no código

	this->substituiValorEQU(); // substitui o valor dos equs que aparecerem no código
};
void montador::processaIF(){
	int i, remove=0, labelNaoExiste = 0;

	for (i = 0; i < this->tokens.size(); i++){
		if (remove == 1){
			remove = 0;
			i=0;
		}
		if (this->tokens.at(i).compare("MACRO") == 0){ // pegar a label da macro pra ser colocada na saida
			this->label_macro = this->tokens.at(i-1);
			this->label_macro = this->label_macro.substr(0, this->label_macro.find(":"));
		}
		if (this->tokens.at(i).compare("IF") == 0){ // caso de excluir a linha do IF
			if(this->tokens.at(i+1).compare("0") == 0){ // se depois do if é 0
				if ((this->tokens[i+2].compare("COPY") == 0)){
					this->tokens.erase(this->tokens.begin()+i, this->tokens.begin()+i+5);
					remove=1; // removeu linha
					continue; // pula todas as outras condições
				}
				else if ((this->tokens[i+2].find(":") != string::npos)){ // encontrar o : na label
					if (this->tokens[i+3].compare("SPACE") == 0) 								// se depois dos : for SPACE
						this->tokens.erase(this->tokens.begin()+i, this->tokens.begin()+i+4);
					
					else if (this->tokens[i+3].compare("COPY") == 0)							// se depois for um COPY com 3 argumentos
						this->tokens.erase(this->tokens.begin()+i, this->tokens.begin()+i+6);

					else																		// se for uma label e depois dois argumentos (normal)
						this->tokens.erase(this->tokens.begin()+i, this->tokens.begin()+i+5);
					
					remove=1;
					continue;
				}
				else{ // se for pra excluir a linha do if e a de baixo com 2 tokens
					this->tokens.erase(this->tokens.begin()+i, this->tokens.begin()+i+4); 
					remove=1;
					continue;
				}
			}	
			else{ 
				if (this->IFsemEQU(this->tokens[i+1]) == 0) // verifica se a label está na lista de equs
					this->tokens.erase(this->tokens.begin()+i, this->tokens.begin()+i+2); // se o if for permanecer a linha de baixo, exclui o if e a label depois dele
			}
		}
	}
};
void montador::processaEQU(){
	int erro = 0, i;

	this->processa_labelsEQU();
	this->processaIF();
	if (this->is_tradutor == 0) // não está traduzindo
		this->gera_linhas();
};

void montador::primeira_passagem(){
	int i;
	
	this->processaMACRO();
	this->processaEQU();
	this->gera_linhas(); // faz a tabela de simbolos
};

void montador::codigo_objeto(){
	int i;
	
	this->linhas.clear();
	for (i = 0; i < this->tokens.size(); i++)
		this->linhas += this->tokens[i] + " ";
	
};
void montador::operandoErrado(){
	int i, j, k, s, flag1 = 0, flag2 = 0, flag3 = 0, copy;
	for (i = 0; i < this->tokens.size(); i++){
		for (j = 0; j < this->instrucoes.size(); j++){
			flag1 = 0;
			flag2 = 0;
			flag3 = 0;
			if (this->tokens[i].compare(this->instrucoes[j]) == 0){ 
				flag1 = 1; // se achou uma instrução
				break;
			}
		}
		if (flag1 == 1){
			copy = 0;
			flag2 = 1;
			flag3 = 1;
			for (k = 0; k < this->simbolo.size(); k++){
				if (this->tokens[i].compare("STOP") == 0){
					flag2 = 0;
					flag3 = 0;
					if (this->tokens[i+1].compare(this->simbolo[k]) == 0){ // stop nao recebe argumento
						linhas += "ERRO SINTATICO: instrucao " + this->tokens[i] + " com quantidade de operando errada na linha " +  to_string(i/2) + '\n';
					}
				}
				if (this->tokens[i].compare("COPY") == 0){
					flag2 = 0;
					if (this->tokens[i+1].compare(this->simbolo[k]) == 0){ // se achou um simbolo na frente
						copy++;
					}
					if (this->tokens[i+2].compare(this->simbolo[k]) == 0){ // se achou outro simbolo na frente
						copy++;
					}
					if (copy == 2){
						flag3 = 0;
						break; // copy ok
					}
				}
				else{ // verificacao de quantidade de operando certa para instrucoes normais
					if (this->tokens[i+1].compare(this->simbolo[k]) == 0){ // se achou um simbolo na frente
						flag2 = 0;
						flag3 = 0;
						break;
					}
				}
			}
			if (flag2 == 1)
				linhas += "ERRO SINTATICO: instrucao " + this->tokens[i] + " com quantidade de operando errada na linha " +  to_string(i/2) + '\n';
			
			if (flag3 == 1 && this->tokens[i].compare("COPY") == 0)
				linhas += "ERRO SINTATICO: instrucao " + this->tokens[i] + " com quantidade de operando errada na linha " +  to_string(i/2) + '\n';
			
		}
	}
};

void montador::segunda_passagem(){ // pega a string linhas e traduz pra codigo objeto	
	int i, j;

	if (this->erroIFsemEQ == 0){ // se não existirem erros de if sem equ
		this->operandoErrado();
	}
	this->tokens.clear(); // um vetor de tokens zerado, pronto pra ser usado
	this->remove_espacosDesnecessarios(linhas);

	for (i = 0; i < this->tokens.size(); i++){
		for (j = 0; j < this->instrucoes.size(); j++){ // tabela de instruções
			if (this->tokens[i].compare(this->instrucoes[j]) == 0)
				this->tokens[i] = this->opcode[j];
		}
		for (j = 0; j < this->simbolo.size(); j++){ // tabela de simbolos
			if (this->tokens[i].compare(this->simbolo[j]) == 0)
				this->tokens[i] = to_string(this->posicao_simbolo[j]);
		}
	}
	this->codigo_objeto();
};