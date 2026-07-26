#ifndef GRAFO_H
#define GRAFO_H

#include <stdbool.h>

/*=========================
    Estruturas
==========================*/

typedef struct No {
    int destino;
    int peso;
    struct No *prox;
} No;

typedef struct {
    int V;          // Número de vértices
    int A;          // Número de arestas
    No **lista;     // Lista de adjacência
} Grafo;


/*=========================
    Manipulação do Grafo
==========================*/

// Carrega um grafo de um arquivo
Grafo *carrega_arquivo(char *nome_arquivo);

// Libera toda a memória utilizada pelo grafo
void libera_grafo(Grafo *grafo);

// Exibe a lista de adjacência
void mostra_grafo(Grafo *grafo);

// Insere uma aresta
void adicionar_aresta(Grafo *grafo, int origem, int destino, int peso);


/*=========================
    Algoritmos de Busca
==========================*/

// Busca em Profundidade (DFS)
void busca_profundidade(Grafo *grafo);

// Busca em Largura (BFS)
void busca_largura(Grafo *grafo);


/*=========================
    Ordenação Topológica
==========================*/

int ordenacao_topologica(Grafo *grafo);


/*=========================
    Árvore Geradora Mínima
==========================*/

// Função auxiliar do Prim
int encontrarMinChave(int *chave, bool *naAVG, int V);

// Algoritmo de Prim
void primAVG(Grafo *grafo, int verticeInicial);


/*=========================
    Menor Caminho
==========================*/

// Algoritmo de Dijkstra
void menor_caminho(Grafo *grafo);

#endif
