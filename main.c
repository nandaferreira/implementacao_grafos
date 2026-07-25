#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "grafo.h"
#include "estatistica.h"

int main() 
{
    int op = 0;
    Grafo* grafo = NULL;
    bool ehDirecionado = false;

    while (op != 9)
    {
        printf("\n=== SISTEMA DE GRAFOS ===\n");
        printf("1. Carregar grafo de arquivo\n");
        printf("2. Mostrar grafo (lista de adjacencia)\n");
        printf("3. Busca em Profundidade (DFS)\n");
        printf("4. Busca em Largura (BFS)\n");
        printf("5. Ordenacao Topologica\n");
        printf("6. Arvore Geradora Minima (Prim)\n");
        printf("7. Menor Caminho (Dijkstra)\n");
        printf("8. Estatisticas do grafo\n");
        printf("9. Sair\n");
        printf("\nEscolha uma operacao: ");
        
        if (scanf("%d", &op) != 1) {
            while (getchar() != '\n'); // Limpa caracteres invalidos
            op = 0;
            continue;
        }

        switch (op)
        {
            case 1: { // Carregar arquivo
                char arquivo[100];
                printf("=== Digite o nome do arquivo juntamente com sua extensao ===\n");
                scanf("%99s", arquivo);
                
                if (grafo != NULL) {
                    libera_grafo(grafo); // Libera o anterior antes de carregar um novo
                }
                
                grafo = carrega_arquivo(arquivo);
                break;
            }

            case 2: { // Mostrar grafo
                if (grafo == NULL) {
                    printf("\n[ERRO] Nenhum grafo carregado! Primeiro escolha a opcao 1.\n");
                } else {
                    mostra_grafo(grafo);
                }
                break;
            }

            case 3: { // DFS
                if (grafo == NULL) {
                    printf("\n[ERRO] Nenhum grafo carregado!\n");
                } else {
                    busca_profundidade();
                }
                break;
            }

            case 4: { // BFS
                if (grafo == NULL) {
                    printf("\n[ERRO] Nenhum grafo carregado!\n");
                } else {
                    busca_largura();
                }
                break;
            }

            case 5: { // Ordenacao Topologica
                if (grafo == NULL) {
                    printf("\n[ERRO] Nenhum grafo carregado!\n");
                } else {
                    ordenacao_topologica();
                }
                break;
            }

            case 6: { // Algoritmo de Prim
                if (grafo == NULL) {
                    printf("\n[ERRO] Nenhum grafo carregado! Escolha a opcao 1 primeiro.\n");
                } else {
                    int verticeInicial;
                    printf("\nDigite o vertice inicial (0 a %d): ", grafo->V - 1);
                    
                    if (scanf("%d", &verticeInicial) == 1) {
                        primAVG(grafo, verticeInicial);
                    } else {
                        printf("\n[ERRO] Entrada invalida!\n");
                    }
                    
                    // Limpa o buffer residual do teclado
                    while (getchar() != '\n'); 
                    
                    printf("\nPressione ENTER para voltar ao menu principal...");
                    getchar(); // Aguarda o ENTER do usuario
                }
                break;
            }

            case 7: { // Dijkstra
                if (grafo == NULL) {
                    printf("\n[ERRO] Nenhum grafo carregado!\n");
                } else {
                    menor_caminho();
                }
                break;
            }

            case 8: { // Estatisticas
                if (grafo == NULL) {
                    printf("\n[ERRO] Nenhum grafo carregado! Primeiro escolha a opcao 1.\n");
                } else {
                    exibirRelatorioEstatisticas(grafo, ehDirecionado);
                }
                break;
            }

            case 9: { // Sair
                printf("\nSaindo...\n");
                break;
            }

            default: {
                printf("\nOpcao invalida! Tente novamente.\n");
                break;
            }
        }
    } 

    // Libera a memoria ao fechar o programa
    if (grafo != NULL) {
        libera_grafo(grafo);
        grafo = NULL;
    }

    return 0;
}