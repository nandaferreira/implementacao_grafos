
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include<stdbool.h>
#include "grafo.h"

#define INF INT_MAX
// #include <string.h>

void adicionar_aresta(Grafo *grafo, int origem, int destino, int peso)
{
    //validação de entrada
    if (grafo == NULL ||
        origem < 0 || origem >= grafo->V ||
        destino < 0 || destino >= grafo->V)
    {
        printf("Erro ao inserir aresta (%d -> %d)\n", origem, destino);
        return;
    }

    //aloca espaço para o novo nó da lista de ajacência
    No *novo = (No *)malloc(sizeof(No));

    //verifica se a alocação deu certo
    if (novo == NULL)
    {
        printf("Erro de memoria.\n");
        return;
    }

    //caso sim
    novo->destino = destino;
    novo->peso = peso;
    novo->prox = NULL;

    No **atual = &grafo->lista[origem]; //insere os valores de entrada na lista do vértice de origem, mantendo a ordem por destino 

    while (*atual != NULL && (*atual)->destino < destino) //cada nó inserido de forma ordenada ascendente, do menor para o maior
        atual = &((*atual)->prox);

    novo->prox = *atual;
    *atual = novo;
}

//leitura e construção do grafo a partir do arquivo
Grafo* carrega_arquivo(char* nome_arquivo) {
    FILE *fptr = fopen(nome_arquivo, "r"); //abre na função read
    if (fptr == NULL) { //verifica se existe
        printf("\nErro: Arquivo '%s' nao encontrado.\n\n", nome_arquivo);
        return NULL;
    }

    printf("\nArquivo carregado com sucesso.\n");

    //lê a primeira linha do arquivo
    int V, A;
    if (fscanf(fptr, "%d %d", &V, &A) != 2) {
        printf("Erro: formato do arquivo invalido\n");
        fclose(fptr);
        return NULL;
    }

    //aloca espaço pra estrutura
    Grafo* grafo = (Grafo*) malloc(sizeof(Grafo));
    if (grafo == NULL) {
        fclose(fptr);
        return NULL;
    }
    
    grafo->V = V;
    grafo->A = A;
    
    //inicializia o vetor de lista como nullo, ou seja, de inicio nenhum vertiec tem vizinhos até que insira as arestas
    grafo->lista = (No**) calloc(V, sizeof(No*));
    if (grafo->lista == NULL) {
        free(grafo);
        fclose(fptr);
        return NULL;
    }

    //adicionar arestas
    //verifica se a entrada de aresta é valida. se não, fehca arquivo e libera a estrutura
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

//essa é a função que realiza a busca em profundidade (DFS) recursiva
//recebe o grafo, o vertice inicial e um array com o numero de vertices do grafo
//esse array é usado para marcar a ordem em que cada vértice será visitado
//a função marca todos os vértices como não visitados e em seguida chama a busca recursiva para o vertice inicial 

static void dfs_visita(Grafo *grafo, int vertice, int *visitado)
{
    visitado[vertice] = 1; //marca o vertice como visitado
    printf("%d ", vertice);

    //percorre todos os vizinhos do vertice atual
    for (No *atual = grafo->lista[vertice]; atual != NULL; atual = atual->prox) {
        int vizinho = atual->destino;
        if (!visitado[vizinho]) {
            dfs_visita(grafo, vizinho, visitado); //para cada vizinho nao visitado, chama recursivamente a função para visitar esse vizinho
            //a chamada acontece até que todos os vizinhos do vértice atual tenham sido visitados
        }
    }
}   

//essa função que inicializa o processo de busca em profundidade (DFS) para todos os vértices do grafo
//somente inicializa a busca caso o vertice nao tenha sido visitado e chama a função dfs_visita que realmente faz o calculo da busca
void busca_profundidade(Grafo *grafo)
{
    if (grafo == NULL) {
        printf("\n[ERRO] Nenhum grafo carregado!\n");
        return;
    }

    int *visitado = (int *)calloc(grafo->V, sizeof(int));
    if (visitado == NULL) {
        printf("\n[ERRO] Falha ao alocar memoria para a busca.\n");
        return;
    }

    printf("\nBusca em profundidade (DFS): ");
    for (int i = 0; i < grafo->V; i++) {
        if (!visitado[i]) {
            dfs_visita(grafo, i, visitado);
        }
    }
    printf("\n");

    free(visitado);
}

void busca_largura(Grafo *grafo)
{
    if (grafo == NULL) {
        printf("\n[ERRO] Nenhum grafo carregado!\n");
        return;
    }

    int *visitado = (int *)calloc(grafo->V, sizeof(int)); // inicializa o vertice com 0 (nao visitado). impede que ele seja enfileirado 2 vezes
    if (visitado == NULL) {
        printf("\n[ERRO] Falha ao alocar memoria para a busca.\n");
        return;
    }

    int *fila = (int *)malloc(grafo->V * sizeof(int));
    if (fila == NULL) {
        free(visitado);
        printf("\n[ERRO] Falha ao alocar memoria para a fila.\n");
        return;
    }

    printf("\nBusca em largura (BFS): ");

    //percorre todos os vertices do grafo
    for (int inicio = 0; inicio < grafo->V; inicio++) {
        if (visitado[inicio]) {
            continue; //se ja foi visitado, passa proximo vertice
        }

        int frente = 0, tras = 0; //a fila é reiniciada a cada vertice novo
        fila[tras++] = inicio; //vertice de inicio é enfileirado e marcado imediatamente como visitado
        visitado[inicio] = 1;

        //enquanto tiver vertices na fila
        while (frente < tras) { 
            int vertice = fila[frente++]; //tira o vértice mais antigo da fila
            printf("%d ", vertice); //imprime do mais antigo ao mais recente (na ordem que foram enfileirados)

            //for para percorrer a lista de adjacência do vértice atual e enfileirar os vizinhos não visitados
            for (No *atual = grafo->lista[vertice]; atual != NULL; atual = atual->prox) {
                int vizinho = atual->destino;
                if (!visitado[vizinho]) {
                    visitado[vizinho] = 1;
                    fila[tras++] = vizinho;
                }
            }
        }
    }

    printf("\n");

    //libera as estruturas auxiliares da busca 
    free(fila);
    free(visitado);
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
    printf("Vertices: %d, Arestas: %d\n", grafo->V, grafo->A);
    printf("Lista de Adjacencia:\n");

    for (int i = 0; i < grafo->V; i++) {
        printf("Vertice %d: ", i);
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
        printf("\n[ERRO] O grafo possui ciclo! Nao e um DAG.\n");
        free(cor); free(pilha); free(ordem);
        return 0;
    }

    printf("\n=== ORDENACAO TOPOLOGICA ===\n");
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

// Função auxiliar: encontra o vértice não visitado com menor distância
int procuraMenorDistancia(int* dist, bool* visitado, int V) { // Recebe vetor de distancias, vetor de visitados e n° de vertices do grafo
    int menor = INT_MAX; // menor = infinito
    int indice = -1; // indice do vertice de menor dist.

    for (int i = 0; i < V; i++) { // percorre vertices, peocura de menor dist.
        if (!visitado[i] && dist[i] < menor) { 
            menor = dist[i]; // atualiza menor dist. encontrada para distancia do vertice
            indice = i; // guarda i como melhor candidato ate o momento
        }
    }
    return indice;
}

// Dijkstra usando lista de adjacência
int* menor_caminho(Grafo *grafo, int s, int* pai){ // Recebe grafo, vertice de origem, vetor pai, retorna vetor com as menores distancias da origem ate cada vertice
    int V = grafo->V;
    int* dist = (int*)malloc(V * sizeof(int)); // aloca vetor de distancia
    bool* visitado = (bool*)malloc(V * sizeof(bool)); // aloca vetor p/ marcar vertices visitados

    for (int i = 0; i < V; i++) { // loop para inicializar vertices
        dist[i] = INT_MAX; // dist = infinito
        visitado[i] = false; // nenhum vertice visitado
        pai[i] = -1; // sem pai
    }
    dist[s] = 0; // dist. de unm vertice a ele mesmo é sempre 0

    int cont = V; // contador de quantos vertices faltam ser visitados
    while (cont > 0) {
        int vert = procuraMenorDistancia(dist, visitado, V); // busca entre vertices nao visitados a menor distancia - greedy vertex
        if (vert == -1) break; // não há mais vértices alcançáveis

        visitado[vert] = true;
        cont--;

        No* atual = grafo->lista[vert]; // pega inicio lista de adjacencia do vertice escolhido
        while (atual != NULL) { // percorre vizinhos
            int ind = atual->destino;
            int peso = atual->peso;

            if (!visitado[ind] && dist[vert] != INT_MAX && dist[vert] + peso < dist[ind]) { // Relaxamento
                dist[ind] = dist[vert] + peso;
                pai[ind] = vert;
            }
            atual = atual->prox; // avança p/ prox  nó da lista de adjacencia (vizinho de vert)
        }
    }

    free(visitado);
    return dist;
}

void imprimirCaminho(int* pai, int destino) {
    int caminho[1000]; // tamanho de sobra
    int qtd = 0;

    int v = destino;
    while (v != -1) {
        caminho[qtd++] = v;
        v = pai[v];
    }

    // imprime na ordem correta: da origem até o destino
    for (int i = qtd - 1; i >= 0; i--) {
        printf("%d", caminho[i]);
        if (i > 0) printf(" -> ");
    }
}

void dijkstraMenu(Grafo* grafo) {
    if (grafo == NULL) {
        printf("\n[ERRO] Grafo nao carregado!\n");
        return;
    }

    int V = grafo->V;
    int origem;

    printf("\n=== DIJKSTRA - MENOR CAMINHO ===\n");
    printf("Vertices disponiveis: 0 a %d\n", V - 1);
    printf("Digite o vertice de origem: ");

    if (scanf("%d", &origem) != 1 || origem < 0 || origem >= V) {
        printf("\n[ERRO] Vertice invalido!\n");
        while (getchar() != '\n');
        return;
    }
    while (getchar() != '\n'); // limpa buffer

    int* pai = (int*)malloc(V * sizeof(int));
    int* distancias = menor_caminho(grafo, origem, pai);

    printf("\n=== DIJKSTRA (Origem: %d) ===\n\n", origem);
    printf("Vertice | Distancia | Caminho\n");
    printf("--------|-----------|--------\n");

    for (int i = 0; i < V; i++) {
        if (distancias[i] == INT_MAX) {
            continue; // se não tiver caminho até esse vértice não imprime nada
        }

        if (i == origem) {
            printf("   %d    |     0     |     0\n", i);
        } else {
            printf("   %d    |     %d     | ", i, distancias[i]);
            imprimirCaminho(pai, i);
            printf("\n");
        }
    }

    free(distancias);
    free(pai);

    printf("\nPressione ENTER para voltar ao menu...");
    while (getchar() != '\n');
    getchar();
}
