#include <stdio.h>
//#include <stdlib.h>
//#include "grafo.h"

int main(){
    
    
    int op = 0;
    
    while(op != 9){
        printf("=== SISTEMA DE GRAFOS ===\n");
        printf("\n1. Carregar grafo de arquivo \n");
        printf("2. Mostrar grafo (lista de adjacencia) \n");
        printf("3. Busca em Profundidade (DFS)");
        printf("4. Busca em Largura (BFS) \n");
        printf("5. Ordenacao Topologica\n");
        printf("6. Arvore Geradora Minima (Prim) \n");
        printf("7. Menor Caminho (Dijkstra) \n");
        printf("8. Estatisticas do grafo \n");
        printf("9. Sair\n");
        printf("\n Escolha uma operacao: ");
        scanf("%d", &op);

        switch(op){
            case 1:{} // funcao de carregar grafo de arquivo
            
            case 2:{} //funcao de mostrar grafo

            case 3:{} //funcao DFS

            case 4:{} // Busca em BFS

            case 5:{} // Ordenacao topologia

            case 6:{} // Arvore geradora minima (algoritmo de Prim)

            case 7:{} // Algoritmo de menor caminho (Dijkstra)

            case 8:{} // Estatisticas do grafo

            case 9:{
                printf("\nSaindo...\n");
                break;
            } 
        }

    }
    
    return 0;

}