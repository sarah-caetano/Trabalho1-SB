# Trabalho1-SB
Sarah Caetano de Almeida

Disciplina de Software Básico

Trabalho 1: Tradutor / Assembler

Esse trabalho consiste em um programa escrito em C++ no Windows versão 21H2, 
edição Windows 10 Education para a tradução da linguagem Assembly hipotética 
vista em sala de aula para a representação em código objeto. 

Foi feito no VS Code e compilado no terminal deste editor. Foi utilizado o compilador
GCC e baixado o MinGW: 
gcc version 6.3.0 (MinGW.org GCC-6.3.0-1), "compilerPath": "C:/MinGW/bin/gcc.exe".

Consiste de análise léxica, sintática e semântica. Ele obtem os parâmetros 
para seu funcionamento pela linha de comando do terminal, sendo o parâmetro:

0 = executável

1 = funcionamento 

    1.1 -p: processa EQU e IF
    
    1.2 -m: processa MACROS
    
    1.3 -o: faz a tradução em duas passagens
    
2 = arquivo de entrada

3 = arquivo de saída

Para a compilação do programa, deve-se utilizar a seguinte linha:
    g++ .\main.cpp .\declaracoes.h .\assembler.cpp -o montador

Para executá-lo:
./montador (funcionamento) (arquivo de entrada) (arquivo de saida)

exemplo para compilar o arquivo AREATRIANGLE.s:
.\montador.exe -o .\AREATRIANGLE.s saida.obj
