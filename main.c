#include <stdio.h>
#include <stdlib.h>
#include "grafo.h"

int main()
{

    int op = 0;
    Grafo* grafo = NULL;

    while (op != 9)
    {
        printf("=== SISTEMA DE GRAFOS ===\n");
        printf("\n1. Carregar grafo de arquivo \n");
        printf("2. Mostrar grafo (lista de adjacencia) \n");
        printf("3. Busca em Profundidade (DFS)\n");
        printf("4. Busca em Largura (BFS) \n");
        printf("5. Ordenacao Topologica\n");
        printf("6. Arvore Geradora Minima (Prim) \n");
        printf("7. Menor Caminho (Dijkstra) \n");
        printf("8. Estatisticas do grafo \n");
        printf("9. Sair\n");
        printf("\n Escolha uma operacao: ");
        scanf("%d", &op);

        switch (op)
        {
        case 1: // funcao de carregar grafo de arquivo
            char *arquivo;
            printf("=== Digite o nome do arquivo juntamente com sua extensão ===\n");
            scanf("%s", arquivo);
            grafo = carrega_arquivo(arquivo);
            op = 0;
            break;

        case 2: // funcao de mostrar grafo
            if(grafo == NULL){
                printf("Erro: Nenhum grafo carregado! Primeiro escolha a opção 1.\n");
            }
            mostra_grafo(grafo);
            op = 0;
            break;

        case 3:
        {
        } // funcao DFS

        case 4:
        {
        } // Busca em BFS

        case 5:
        {
        } // Ordenacao topologia

        case 6:
        {
        } // Arvore geradora minima (algoritmo de Prim)

        case 7:
        {
        } // Algoritmo de menor caminho (Dijkstra)

        case 8:
        {
        } // Estatisticas do grafo

        case 9:
        {
            printf("\nSaindo...\n");
            break;
        }
        }
    }

    if(grafo != NULL){
        free(grafo);
    }

    return 0;
}