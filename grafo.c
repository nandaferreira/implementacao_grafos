/**
 * @file Grafo.c
 * @brief Arquivo .c de funcoes relacionadas a grafos
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "grafo.h"
// #include <string.h>

/*
 * V A
 * origem destino peso
 * origem destino peso
 *
 */

Grafo* carrega_arquivo(char* nome_arquivo) {
    FILE *fptr = fopen(nome_arquivo, "r");
    if (fptr == NULL) {
        printf("\nArquivo inexistente.\n");
        return NULL;
    }

    printf("\nArquivo carregado.\n");

    // Lê primeira linha: Vértices e Arestas
    int V, A;
    if (fscanf(fptr, "%d %d", &V, &A) != 2) {
        printf("Erro: formato do arquivo inválido\n");
        fclose(fptr);
        return NULL;
    }

    // Aloca o grafo
    Grafo* grafo = (Grafo*)malloc(sizeof(Grafo));
    if (grafo == NULL) {
        fclose(fptr);
        return NULL;
    }
    
    grafo->V = V;
    grafo->A = A;
    
    // Aloca lista de adjacência
    grafo->lista = (No**) calloc(V, sizeof(No*));
    if (grafo->lista == NULL) {
        free(grafo);
        fclose(fptr);
        return NULL;
    }

    // Lê as arestas
    int origem, destino, peso;
    for (int i = 0; i < A; i++) {
        if (fscanf(fptr, "%d %d %d", &origem, &destino, &peso) != 3) {
            printf("Erro: formato de aresta inválido\n");
            fclose(fptr);
            libera_grafo(grafo);
            return NULL;
        }
        adicionar_aresta(grafo, origem, destino, peso);
    }

    fclose(fptr);
    return grafo;
}

void libera_grafo(Grafo* grafo) {
    if (grafo == NULL) return;

    // Libera cada lista de adjacência
    for (int i = 0; i < grafo->V; i++) {
        No* atual = grafo->lista[i];
        while (atual != NULL) {
            No* temp = atual;
            atual = atual->prox;
            free(temp);
        }
    }
    
    // Libera o array de listas e o grafo
    free(grafo->lista);
    free(grafo);
}


void adicionar_aresta(Grafo* grafo, int origem, int destino, int peso) {
    // Adiciona aresta origem -> destino
    No* novo = (No*) malloc(sizeof(No));
    novo->destino = destino;
    novo->peso = peso;
    novo->prox = grafo->lista[origem];
    grafo->lista[origem] = novo;
}

void mostra_grafo(Grafo* grafo) {
    if (grafo == NULL) {
        printf("Grafo vazio!\n");
        return;
    }

    printf("\n=== GRAFO ===\n");
    printf("Vértices: %d, Arestas: %d\n", grafo->V, grafo->A);
    printf("Lista de Adjacência:\n");

    for (int i = 0; i < grafo->V; i++) {
        printf("Vértice %d: ", i);
        No* atual = grafo->lista[i];
        if (atual == NULL) {
            printf("(sem arestas)");
        }
        while (atual != NULL) {
            printf("-> [%d, peso:%d] ", atual->destino, atual->peso);
            atual = atual->prox;
        }
        printf("\n");
    }
    printf("\n");
}

int busca_profundidade()
{
    printf("Busquei em profundidade");
    return 1;
}

int busca_largura()
{
    printf("Busquei em largura");
    return 1;
}

int ordenacao_topologica()
{
    printf("Ordenei topologicamente");
    return 1;
}

int arvore_minima()
{
    printf("Calculei árvore mínima - Prim");
    return 1;
}

int menor_caminho()
{
    printf("Calculei menor caminho - Dijkstra");
    return 1;
}

int estatisticas()
{
    printf("Calculei estatísticas");
    return 1;
}
