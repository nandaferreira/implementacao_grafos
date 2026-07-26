#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "grafo.h"
#include "estatistica.h"
#include "desafios.h"

int main() 
{
    int op = 0;
    Grafo* grafo = NULL;
    bool ehDirecionado = false;

    while (op != 10)
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
        printf("9. Desafios Extras\n");
        printf("10. Sair\n");
        printf("\nEscolha uma operacao: ");
        
        if (scanf("%d", &op) != 1) {
            while (getchar() != '\n');
            op = 0;
            continue;
        }

        switch (op)
        {
            case 1: {
                char arquivo[100];
                printf("=== Digite o nome do arquivo juntamente com sua extensao ===\n");
                scanf("%99s", arquivo);
                
                if (grafo != NULL) {
                    libera_grafo(grafo);
                }
                
                grafo = carrega_arquivo(arquivo);
                if (grafo != NULL) {
                    // Por padrão, assume-se não direcionado. 
                    // Se quiser, pode perguntar ao usuário ou detectar pelo arquivo.
                    ehDirecionado = false;
                }
                break;
            }

            case 2: {
                if (grafo == NULL) {
                    printf("\n[ERRO] Nenhum grafo carregado! Primeiro escolha a opcao 1.\n");
                } else {
                    mostra_grafo(grafo);
                }
                break;
            }

            case 3: {
                if (grafo == NULL) {
                    printf("\n[ERRO] Nenhum grafo carregado!\n");
                } else {
                    // A função busca_profundidade() deve ser ajustada para receber o grafo.
                    // Se a assinatura for diferente, adapte aqui.
                    busca_profundidade(grafo);
                }
                break;
            }

            case 4: {
                if (grafo == NULL) {
                    printf("\n[ERRO] Nenhum grafo carregado!\n");
                } else {
                    busca_largura(grafo);
                }
                break;
            }

            case 5: {
                if (grafo == NULL) {
                    printf("\n[ERRO] Nenhum grafo carregado!\n");
                } else {
                    // Chama a ordenação topológica passando o grafo
                    ordenacao_topologica(grafo);
                }
                break;
            }

            case 6: {
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
                    
                    while (getchar() != '\n'); 
                    printf("\nPressione ENTER para voltar ao menu principal...");
                    getchar();
                }
                break;
            }

            case 7: { // Dijkstra
                if (grafo == NULL) {
                    printf("\n[ERRO] Nenhum grafo carregado!\n");
                }
                else {
                    dijkstraMenu(grafo);
                }
                break;
            }

            case 8: {
                if (grafo == NULL) {
                    printf("\n[ERRO] Nenhum grafo carregado! Primeiro escolha a opcao 1.\n");
                } else {
                    exibirRelatorioEstatisticas(grafo, ehDirecionado);
                }
                break;
            }

            case 9: { // Desafios Extras - submenu
                int subOp = 0;
                while (subOp != 5) {
                    printf("\n=== DESAFIOS EXTRAS ===\n");
                    printf("1. Deteccao de Ciclos (DFS)\n");
                    printf("2. Componentes Fortemente Conexos (Kosaraju)\n");
                    printf("3. Caminho Critico (DAG)\n");
                    printf("4. Desempenho de Grafo com 1000+ vértices\n");
                    printf("5. Voltar ao menu principal\n");
                    printf("\nEscolha uma opcao: ");
                    
                    if (scanf("%d", &subOp) != 1) {
                        while (getchar() != '\n');
                        subOp = 0;
                        continue;
                    }

                    switch (subOp) {
                        case 1: {
                            if (grafo == NULL) {
                                printf("\n[ERRO] Nenhum grafo carregado! Escolha a opcao 1 do menu principal primeiro.\n");
                                break;
                            }
                            printf("\n=== DETECCAO DE CICLOS ===\n");
                            printf("Tipo do grafo: %s\n", ehDirecionado ? "Direcionado" : "Nao Direcionado");
                            bool possuiCiclo = tem_ciclo(grafo, ehDirecionado);
                            if (possuiCiclo) {
                                printf("\n[RESULTADO] O grafo POSSUI CICLO(S)!\n");
                            } else {
                                printf("\n[RESULTADO] O grafo NAO possui ciclos (Aciclico).\n");
                            }
                            break;
                        }
                        case 2: {
                            if (grafo == NULL) {
                                printf("\n[ERRO] Nenhum grafo carregado! Escolha a opcao 1 do menu principal primeiro.\n");
                                break;
                            }
                            if (!ehDirecionado) {
                                printf("\n[AVISO] Kosaraju e' mais adequado para grafos direcionados.\n");
                                printf("Deseja continuar mesmo assim? (s/n): ");
                                char resp;
                                scanf(" %c", &resp);
                                if (resp != 's' && resp != 'S') {
                                    break;
                                }
                            }
                            kosaraju(grafo);
                            break;
                        }
                        case 3: {
                            if (grafo == NULL) {
                                printf("\n[ERRO] Nenhum grafo carregado! Escolha a opcao 1 do menu principal primeiro.\n");
                                break;
                            }
                            if (!ehDirecionado) {
                                printf("\n[AVISO] Caminho Critico requer um grafo direcionado aciclico (DAG).\n");
                                printf("Deseja continuar mesmo assim? (s/n): ");
                                char resp;
                                scanf(" %c", &resp);
                                if (resp != 's' && resp != 'S') {
                                    break;
                                }
                            }
                            caminho_critico(grafo);
                            break;
                        }
                        case 4: {
                            testarDesempenho();
                            break;
                        }
                        case 5: {
                            printf("\nVoltando ao menu principal...\n");
                            break;
                        }
                        default: {
                            printf("\nOpcao invalida!\n");
                            break;
                        }
                    }
                    // Limpa o buffer após cada operação
                    while (getchar() != '\n');
                    if (subOp != 5) {
                        printf("\nPressione ENTER para continuar...");
                        getchar();
                    }
                }
                break;
            }

            case 10: {
                printf("\nSaindo...\n");
                break;
            }

            default: {
                printf("\nOpcao invalida! Tente novamente.\n");
                break;
            }
        }
    } 

    if (grafo != NULL) {
        libera_grafo(grafo);
        grafo = NULL;
    }

    return 0;
}