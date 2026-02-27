/*
** file: main-maxh.cpp
**
** max Heap (binary tree on array)
** UC: 21046 - EDAF @ UAb
** e-fólio B 2023-24
**
** Aluno: Bruno Ricardo de Sá Ferreira
*/

// Defina neste ficheiro:
//   A entrada/saída de dados
//   As instâncias da classe da estrutura de dados
//   A implementação dos comandos através dos métodos da classe
//   Código auxiliar
//   Não utilize variáveis globais!

#include <iostream>
#include <sstream>
#include <string>
#include "maxh.h"
using namespace std;


int main() {
    IMAXH maxheap;//instancia maxheap é criada da classe IMAXH que representa o Heapmaximo, permitenos manipular o heap
    std::string line;//declarada variavel line, para armazenar o que lido na linha de comandos
    while (std::getline(std::cin, line)) {//loop que le as linhas na entrada padrao
        if (line.empty() || line[0] == '#') continue;
        std::stringstream ss(line);
        std::string command;//variavel que armazena o comando
        ss >> command;
        if (command == "insert") {//verifica o comando
            int item;//variavel item usada para armazenar items inseridos no heap
            while (ss >> item) {//loop para extrair numeros
                if (maxheap.getN() == maxheap.getNv()) {//aqui, verificamos se o heap esta cheio comparando as capacidades
                    std::cout << "Comando " << command << ": Heap cheio!\n";
                    break;
                }
                maxheap.insert(item);//chamamos a funcao insert sem usar o algoritmo heapify 
                //int insertedItem = maxheap.insert(item);//chamamos a funçao insert e inserimos item no heap
                //if (insertedItem != -1) {//verifica se o valor retornado pelo metodo insert é valido
                    //maxheap.heapify_up(maxheap.getN() - 1);//aqui chamamos o ultimo elemento do heap, e tentamos manter a prioridade dele
                }
        } else if (command == "print_max") {
            maxheap.print_max();//imprimir maior elemento do heap
        } else if (command == "print") {//imprime todos os elementos do heap
            maxheap.print();
        } else if (command == "dim") {//imprime numero de elementos do heap
            maxheap.dim();
        } else if (command == "dim_max") {//capacidade maxima do heap
            maxheap.dim_max();
        } else if (command == "clear") {//aqui limpamos o heap
            maxheap.clear();
        } else if (command == "delete") {//apagar o maior elemento do heap
            maxheap.deleteMax();
        } else if (command == "heapify_up") {
            maxheap.setN(0);//limpa o heap para nao haver valores repetidos 
            int item;
            while (ss >> item) {
                if (maxheap.getN() == maxheap.getNv()) {//verifica a dimensao do heap
                    std::cout << "Comando " << command << ": Heap cheio!\n";
                    break;
                }
                maxheap.getV()[maxheap.getN()] = item;//se o heap nao estiver cheio inserimos item 
                maxheap.setN(maxheap.getN() + 1);//incrementa o numero de elementos no heap(n) em 1
            }//usamos o comando heapify_up para inserir varios comandos de uma vez 
            maxheap.buildMaxHeap();//construimos o heap
        } else if (command == "redim_max") {//comando para redimensionar o heap
            int newNv;//variavel que armazena o novo valor maximo
            if (ss >> newNv) {//tenta ler o numero na linha de entrada 
                maxheap.redim_max(newNv);//se sim, redimensiona a capacidade do heap
            }
        } else {
            std::cout << "Comando " << command << " é inválido!\n";
        }
    }
    return 0;
}

