/**
 * @file Grafo.h
 * @brief Arquivo .h para grafos
 *
 *
 */

#ifndef GRAFO_H
#define GRAFO_H

typedef struct No
{
  int destino;
  int peso;
  struct No *prox;
} No;

typedef struct
{
  int V;      // Número de vértices
  int A;      // Número de arestas
  No **lista; // Lista de adjacência
} Grafo;



/**
 * @brief Leitura de Grafos no formato:
 * V A
 * origem destino peso
 * origem destino peso
 * ...
 *
 */

 Grafo *carrega_arquivo(char* nome_arquivo);

 /**
 * @brief Libera memória de grafo carregado
 *
 */

 void libera_grafo(Grafo* grafo);


/**
 * @brief Mostra Grafo
 *
 */

void mostra_grafo(Grafo* grafo);


 // PRECISA DE DEFINIÇÃO DE FUNÇÃO
 /**
 * @brief Adiciona aresta para inserção ordenada nas listas de adjacência
 *
 */
void adicionar_aresta(Grafo* grafo, int origem, int destino, int peso);

/**
 * @brief Busca em Profundidade (DFS)
 *
 */

int busca_profundidade();

/**
 * @brief Busca em Largura (BFS)
 *
 */

int busca_largura();

/**
 * @brief Ordenação Topológica
 *
 */

int ordenacao_topologica();


/**
 * @brief Árvore Geradora Mínima - Prim
 *
 */

int arvore_minima();

/**
 * @brief Menor Caminho - Dijkstra
 *
 */

int menor_caminho();

/**
 * @brief Printa Estatísticas
 *
 */

int estatisticas();


#endif