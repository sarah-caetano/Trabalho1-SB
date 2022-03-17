#ifndef MONTADOR_H
#define MONTADOR_H

#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <sstream>
#include <fstream>
using namespace std;

class montador{
public:
    montador(string str);
    ~montador();
    void monta();
    string file_path;

    // processamento de linha e salvamento
    void processaLinha();
    void caixa_alta();
    void remove_comentarios();
    void remove_espacosDesnecessarios(string linha_temporaria);

    // funcoes pra EQU e IF
    void processaEQU();
    void processa_labelsEQU(); 
    void removeEQU();
    void substituiValorEQU();
    void processaIF();
    void EQUinutil();
    int IFsemEQU(string label);

    // funcoes pra MACRO
    void processaMACRO();
    void substituiMACRO();
    int faltaENDMACRO();
    int MACROinutil();

    // funcoes do tradutor
    void faz_tabela_simbolos(string label, int endereco);
    void gera_linhas();
    void primeira_passagem();
    void segunda_passagem();
    void faz_listas();
    void codigo_objeto();
    void simbolos_invalidos();
    void token_invalido(string token, string linha);
    void proximo_token_rotulo(string token);
    void rotulo_repetido();
    void operandoErrado();

    int is_tradutor = 0; // VARIAS coisas: lista de simbolos, de const e sppace no final das linhas de saída
    int is_m = 0; // também é pra função de EQUsemIF, gerar a lista das labels de EQU
    int erroIFsemEQ = 0; // sem essa flag, quando tem o erro de IFsemEQU ele ainda colocava a listaCONSTSPACE no final das linhas de saida
    int token_errado = 0;
    int simbolo_errado = 0;
    int rotulo_igual = 0;
    int sem_endmacro = 0;
    int doisRotulos = 0; // flag para indicar q foram encontrados dois rótulos em uma mesma linha e não deixar fazer o código obj
    string label_macro, linhas, linha_inicial, linha_temporaria, listaCONSTSPACE;
    vector <string> tokens; // linhas em tokens
    vector <string> macro; // linhas da macro
    vector <string> LabelEQUS; // associar labels a ints
    vector <string> ValorEQUS;
    vector <string> diretivas;
    vector <string> instrucoes;
    vector <string> simbolo;
    vector <int> posicao_simbolo;
    vector <string> opcode;
    vector <string> label_invalida; // vetor que contem a label invalida e sua linha

};

#endif