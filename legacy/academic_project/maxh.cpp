/*
** file: maxh.cpp
**
** max Heap (binary tree on array)
** UC: 21046 - EDAF @ UAb
** e-fólio B 2023-24
**
** Aluno:Bruno Ricardo de Sá Ferreira
*/

// Defina:
//   Em maxh.h as classes da estrutura de dados
//   Em maxh.cpp a implementação dos métodos das classes da estrutura de dados

// maxh.cpp
#include "maxh.h"
#include <iostream>
#include <algorithm>

IMAXH::IMAXH(int nmax) : v(new int[nmax]), n(0), nv(nmax) {}

IMAXH::~IMAXH() { delete[] v; }

void IMAXH::insert(int item) {//inserir numero no heap
    if (getN() == getNv()) {
        std::cout << "Comando insert: Heap cheio!\n";
        return ;
    }
    v[getN()] = item;//inserimos o item no final do heap, usando getN que obtem o n de elemntos atualmente do heap
    heapify_up(n);;//aqui usamos get e set, para atualizarmos um heap, é um incremento
    n++;//aqui, visto que não podemos usar o algoritmo heapify neste metodo, fazemos o retorno dele
}

void IMAXH::heapify_up(int i) {//Vamos mover os elementos do heap para cima
    //aqui acessamos o todo do heap, indice[0], se i = topo, nao fazemos nada
    if (i == 0) {
        return;
    }
    
    //nao esta no topo, vamos descobrir o pai
    //como a nossa arvore e binaria fazemos um calculo de divisao inteira para descobrir o pai
    //Ex: [0][1][2], = i = 2 // (2-1)/2 = 0.5 entao arrendondamos para zero //pois i é o ultimo elemento
    int parent = (i - 1) / 2;//ja temos o pai

    //Agora como e heapmax a propriedade e: pais devem ser maiores ou iguais a filhos
    //entao vamos comparar o no atual que e filho com o seu pai
    if (v[i] > v[parent]) {//se for maior que o pai
        //fazemos a troca para ajustar o heap
        std::swap(v[i], v[parent]);

        //agora apos a troca, fazemos uma chamada recursiva mas no no pai, pois este
        //ainda pode ser um no filho maior que um novo pai...
        //e a metodo roda ate o indice zero ou ate parent ser menor ou igual ao seu novo pai
        heapify_up(parent);
    }
}

void IMAXH::print_max() {//imprimir o maior valor do heap
    if (n == 0) {
        std::cout << "Comando print_max: Heap vazio!\n";
        return;
    }
    if (n != 0)
        std::cout << "Max= " << v[0] << "\n";//caso contrario, acessa a primeira posicao que e a maior e imprime
}

void IMAXH::print() {//imprimir todos os elementos do heap
    if (n == 0) {
        std::cout << "Comando print: Heap vazio!\n";
        return;
    }//alerta para heap vazio
    std::cout << "Heap=\n";//imprime heap antes de imprimir os elementos
    //numero de elementos do nivel atual 
    int levelSize = 1;
    //vamos rastrear quantos elementos do nivel atual ja foram impressos
    int levelCount = 0;

    for (int i = 0; i < n; ++i) {//itera pelo heap, sendo n o numero total de elementos no heap
        std::cout << v[i];//aqui imprimimos o primeiro elemento do heap
        //apenas identa se não for o ultimo nível
        if (levelCount != levelSize - 1 && i != n - 1) {//aqui verificamos, se level count tem o mesmo tamanho que level size, 
            std::cout << ' ';                           // e se i nao e o ultimo elemento do heap 
        }//caso seja verdadeiro adicionamos espaco
        levelCount++; // e incrementamos levelcount, ou se for a primeira iteracao level count passa a ser 1

        //aqui, vemos se todos os elementos do nivel atual ja foram impressos
        //fazendo caso seja a primeira iteracao, 1 = 1 || i == ultimo elemnto no heap
        if (levelCount == levelSize || i == n - 1) {
            std::cout << std::endl;//se e verdade quebramos a linha para imprimir em formato arvore
            //agora o proximo nivel, como regra das arvores binarias, tera o dobro dos elementos do nivel atual 
            levelSize *= 2; //segundo nivel passa a ser 1*2 , que e dois. 
            //atualizamos level count, para rastrear o print do elementos do nivel a seguir
            levelCount = 0;
        }//voltamos para o inicio do loop, level size agora e 2 e level count e 0, i agora é [1]
    }
}

//aqui imprimimos o numero de elementos no heap
void IMAXH::dim() {
    std::cout << "Heap tem " << n << " itens\n";
}
//imprimimos a capacidade maxima do heap
void IMAXH::dim_max() {
    std::cout << "Heap tem capacidade " << nv << " itens\n";
}
//aqui limpamos o heap
void IMAXH::clear() {
    if (n == 0) {
        std::cout << "Comando clear: Heap vazio!\n";// primeiro verificamos se esta vazio
        return;
    }
    this->setN(0);//caso nao esteja, aplicamos um setter como boa pratica de programacao
}

void IMAXH::deleteMax() {//aqui vamos remover o maior elemento do heap, 
    if (n == 0) {
        std::cout << "Comando delete: Heap vazio!\n";//verificamos, se esta vazio
        return;
    }
    std::swap(v[0], v[n - 1]);// se nao esta vazio, trocamos o elemento de indice [0] com o elemento no ultimo indice [n-1]
    n--;//decrementamos o ultimo elemento, ou seja, apagamo-lo
    heapify(0);//chamamos a funcao heapify para organizar o heap novamente
}

void IMAXH::heapify(int i) {//vamos garantir que os heaps maximos sao formados
    int largest = i;//inciamos o no atual como o maior ate ao momento ex[0] valor 11, nos filhos , 16 e 14
    int left = 2 * i + 1;//aqui calculamos os indices para os nos esquero que e, 2*0 + 1 = 1, equivalente a 16
    int right = 2 * i + 2;//a mesma coisa, equivalente a 14, [2] indice 2...
    if (left < n && v[left] > v[largest])//aqui, verificamos se o filho esquerdo existe e nao esta a acessar uma posicao fora dos indices de n, e verificamos 
        largest = left;//se o filho a esquerda[1] e maior que o pai[0], se, sim, atualizamos largest para [1]
    if (right < n && v[right] > v[largest])//a mesma verificacao
        largest = right;//largest agora e 16 que e maior que 14
    if (largest != i) {//aqui verificamos se largest 16 [1] e diferente do no atual 11[0] o que e verdade 
        std::swap(v[i], v[largest]);//entao fazemos a troca do no atual [0]11 pelo maior filho [1]16
        heapify(largest);// agora é 16[0] e a chamada recursiva e feita para [1]
    }
}

void IMAXH::buildMaxHeap() {
    int lastParent = (n - 1) / 2;//calculamos o pai do ultimo no. Se n = 15, entao n-1/2 = 14/2 = [7]ultimo no pai na posicao i[7]
    for (int i = lastParent; i >= 0; i--)//i recebe o ultimo pai, e itera ate a raiz, decrementando apos cada
        heapify(i);                      //cada chamada da funcao heapify
}

void IMAXH::redim_max(int newNv) {//definir a nova capacidade do heap
    delete[] v;//liberamos a memoria alocada para o ultimo vetor 
    v = new int[newNv];//
    nv = newNv;////atualiza a capacidade do heap para o novo valor fornecido
    n = 0;//limpa o heap 
}