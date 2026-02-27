/*
** file: maxh.h
**
** max Heap (binary tree on array)
** UC: 21046 - EDAF @ UAb
** e-fólio B 2023-24
**
** Aluno: Bruno Ricardo de Sá Ferreira
*/

// Defina:
//   Em maxh.h as classes da estrutura de dados
//   Em maxh.cpp a implementação dos métodos das classes da estrutura de dados
// Não altere o nome das classes nem dos atributos obrigatórios!

// definir int max heap de inteiros em array
class IMAXH {
private:
    int* v;//ponteiro para um vetor que armazena os elementos do heap
    int n;//numero de elementos atualizados no heap
    int nv;//numero maximo de elementos estabelecidos no heap
    
public:
    IMAXH(int nmax = 15);//construtor que define a capacidade maxima de elementos no heap
    ~IMAXH();//destrutor que libera a memoria alocada no heap
    //getters e setters para o vetor v
    int* getV() const { return v; }
    void setV(int* newV) { v = newV; }
    //getters e setters para o numero de elementos na variavel n
    int getN() const { return n; }
    void setN(int newN) { n = newN; }
    // """" para a dimensao do heap
    int getNv() const { return nv; }
    void setNv(int newNv) { nv = newNv; }
    //metodos
    void insert(int item);//insere item no heap
    void print_max();//imprime maior item no heap
    void print();//imprime todos os elementos no heap
    void dim();//imprimir quantos elementos estao presentes no heap
    void dim_max();//imprime a capacidade que o heap tem
    void clear();//limpa o heap
    void deleteMax();//apaga o maior item do heap
    void heapify(int i);//metodo para ajustar o heap atraves do indice definido i
    void buildMaxHeap();// metodo que constroi um heap maximo atravez do vetor atual
    void redim_max(int newNv);// redefinir a capacidade do heap
    void heapify_up(int i);//ajuste do heap movento ele para cima
};