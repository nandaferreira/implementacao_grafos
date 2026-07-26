
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include<stdbool.h>
#include "grafo.h"

#define INF INT_MAX
// #include <string.h>

/*
 * V A
 * origem destino peso
 * origem destino peso
 *
 */
void adicionar_aresta(Grafo *grafo, int origem, int destino, int peso)
{
    if (grafo == NULL ||
        origem < 0 || origem >= grafo->V ||
        destino < 0 || destino >= grafo->V)
    {
        printf("Erro ao inserir aresta (%d -> %d)\n", origem, destino);
        return;
    }

    No *novo = (No *)malloc(sizeof(No));

    if (novo == NULL)
    {
        printf("Erro de memoria.\n");
        return;
    }

    novo->destino = destino;
    novo->peso = peso;
    novo->prox = NULL;

    No **atual = &grafo->lista[origem];

    while (*atual != NULL && (*atual)->destino < destino)
        atual = &((*atual)->prox);

    novo->prox = *atual;
    *atual = novo;
}

Grafo* carrega_arquivo(char* nome_arquivo) {
    FILE *fptr = fopen(nome_arquivo, "r");
    if (fptr == NULL) {
        printf("\nErro: Arquivo '%s' nao encontrado.\n\n", nome_arquivo);
        return NULL;
    }

    printf("\nArquivo carregado com sucesso.\n");

    int V, A;
    if (fscanf(fptr, "%d %d", &V, &A) != 2) {
        printf("Erro: formato do arquivo invalido\n");
        fclose(fptr);
        return NULL;
    }

    Grafo* grafo = (Grafo*) malloc(sizeof(Grafo));
    if (grafo == NULL) {
        fclose(fptr);
        return NULL;
    }
    
    grafo->V = V;
    grafo->A = A;
    
    grafo->lista = (No**) calloc(V, sizeof(No*));
    if (grafo->lista == NULL) {
        free(grafo);
        fclose(fptr);
        return NULL;
    }

    int origem, destino, peso;
    for (int i = 0; i < A; i++) {
        if (fscanf(fptr, "%d %d %d", &origem, &destino, &peso) != 3) {
            printf("Erro: formato de aresta invalido\n");
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

void busca_profundidade(Grafo *grafo)
{
    printf("\nDFS ainda nao implementada.\n");
}

void busca_largura(Grafo *grafo)
{
    printf("\nBFS ainda nao implementada.\n");
}

int ordenacao_topologica(Grafo* grafo) {
    if (grafo == NULL) {
        printf("\n[ERRO] Nenhum grafo carregado!\n");
        return 0;
    }

    int V = grafo->V;
    int *cor = (int*)calloc(V, sizeof(int)); // 0=branco, 1=cinza, 2=preto
    int *pilha = (int*)malloc(V * sizeof(int));
    int topo = 0;
    int *ordem = (int*)malloc(V * sizeof(int));
    int idx = V - 1;
    int temCiclo = 0;

    // DFS recursiva para ordenação topológica
    void dfs(int v) {
        cor[v] = 1; // cinza – em processamento
        No* atual = grafo->lista[v];
        while (atual != NULL) {
            int vizinho = atual->destino;
            if (cor[vizinho] == 1) {
                temCiclo = 1; // ciclo detectado
            } else if (cor[vizinho] == 0) {
                dfs(vizinho);
            }
            atual = atual->prox;
        }
        cor[v] = 2; // preto – finalizado
        ordem[idx--] = v; // insere no final da ordem
    }

    for (int i = 0; i < V; i++) {
        if (cor[i] == 0) dfs(i);
    }

    if (temCiclo) {
        printf("\n[ERRO] O grafo possui ciclo! Não é um DAG.\n");
        free(cor); free(pilha); free(ordem);
        return 0;
    }

    printf("\n=== ORDENAÇÃO TOPOLÓGICA ===\n");
    printf("Ordem: ");
    for (int i = 0; i < V; i++) {
        printf("%d%s", ordem[i], (i == V-1) ? "\n" : " -> ");
    }

    free(cor); free(pilha); free(ordem);
    return 1;
}

//FUNÇÃO AUXILIAR PARA ARVORE GERADORA MINIMA (PRIM)'
int encontrarMinChave(int* chave, bool* naAVG, int V) {
    int min = INF;
    int min_index = -1;

    for (int v = 0; v < V; v++) {
        if (!naAVG[v] && chave[v] < min) {
            min = chave[v];
            min_index = v;
        }
    }
    return min_index;
}
//Árvore Geradora Minima (Prim)
void primAVG(Grafo* g, int verticeInicial)
{
    if (g == NULL)
    {
     printf("Grafo inexistente.\n");
     return;
    }

    if (verticeInicial < 0 || verticeInicial >= g->V)
    {
     printf("Vertice inicial invalido.\n");
      return;
    }
   
    int V= g->V;

    //Alocação  
    int* chave= (int*) malloc(V*sizeof(int));
    int* pai= (int*) malloc(V*sizeof(int));
    bool* naAVG=(bool*) malloc(V*sizeof(bool));
    
    if (chave == NULL || pai == NULL || naAVG == NULL){
     printf("Erro de memoria.\n");

     free(chave);
     free(pai);
     free(naAVG);

     return;
    }
    //Inicialização das variáveis com valores iniciais
    for(int i=0; i<V; i++){
        chave[i]=INF; //Valor infinito
        pai[i]=-1; //Sem pai
        naAVG[i]=false; //Não está na árvore geradora mínima ainda
    }
    //Inicializa o vértice inicial
    chave[verticeInicial]=0; //Custo 0 para o vértice inicial

    //Laço principal do algoritmo de Prim

    for(int count=0; count<V-1; count++){
        //Seleciona o vértice com a menor chave que ainda não está na árvore
        int u= encontrarMinChave(chave, naAVG, V);

        //Trava para grafos Desconexos:
        if((u==-1)|| (chave[u]==INF)){
            break; //Sai do laço se não houver mais vértices acessíveis
        }
        naAVG[u]=true; //Marca o vértice como incluído na árvore

        //Atualiza os valores das chaves e pais dos vértices vizinhos de u
        No* p= g->lista[u];
        while(p!=NULL){
            int v= p->destino;
            int peso= p->peso;
            if(naAVG[v]==false && peso<chave[v]){
                pai[v]=u;
                chave[v]=peso;
            }
            p=p->prox;
        }
    }
        //Impressão da árvore geradora mínima
        printf("\n=== ARVORE GERADORA MINIMA (PRIM) ===\n");
        int pesoTotal=0;
        for(int i=0; i<V; i++){
            if(pai[i]!=-1){
                printf("Aresta: %d - %d, Peso: %d\n", pai[i], i, chave[i]);
                pesoTotal+=chave[i];
            }
        }

        printf("Peso total da Arvore geradora minima: %d\n", pesoTotal);
        free(chave);
        free(pai);
        free(naAVG);
      

    
}

void menor_caminho(Grafo *grafo)
{
    printf("\nDijkstra ainda nao implementado.\n");
}
