#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h> 
#include "grafo.h"
#include "desafios.h"

//-----------------------------------------//
//DESAFIO: DETECÇAO DE CICLOS USANDO DFS
//----------------------------------------//
//Auxiliar para grafo nãoDirecionado

static bool dfsCicloNaoDirecionado (Grafo* g, int v, bool visitado[], int pai){

    visitado[v] = true;

    No* atual= g->lista[v];
    while(atual!=NULL){
        int vizinho= atual->destino;
        //Verifica se o vizinho aida não foi visitado, avança na DFS
        if(visitado[vizinho]==false){
            if(dfsCicloNaoDirecionado(g, vizinho, visitado, v)){
                return true;
            }
            
         }
         //Se o vizinho já foi visitado e não é o pai de onde viemos,ENTÃO HÁ CICLO
        else if(vizinho!=pai){
            return true;
        }

         atual= atual->prox;
    }
    return false;
}   

//Auxiliar para Grafo Direcionado (Usando 3 cores/ pilha de recursoão)
//BRANCO: Não visitado, CINZA: Visitado mas não finalizado, PRETO: Finalizado
static bool dfsCicloDirecionado(Grafo* g, int v, int cor[]){
    cor[v]=CINZA; //CINZA: Em processamento(pilha atual)

    No* atual= g->lista[v];
    while(atual!=NULL){
        int vizinho= atual->destino;

        //Se o vizinho está cor CINZA, encontramos uma aresta de volta=Ciclo
        if(cor[vizinho]==CINZA){
            return true;
        }

        //Se o vizinho ainda não foi visitado(BRANCO), continua a DFS
        if(cor[vizinho]==BRANCO){
            if(dfsCicloDirecionado(g, vizinho, cor)){
                return true;
            }
        }
        atual= atual->prox;
    }
    cor[v]=PRETO; //PRETO: Finalizado
    return false;
}   

//Função Principal para Detecção de Ciclos
bool tem_ciclo(Grafo*g, bool ehDirecionado){

    if(g==NULL) return false;
    
    int V= g->V;
    if(ehDirecionado){
        //GrafoDirecionado: Usando 3 cores
        int*cor =(int*) malloc(V*sizeof(int));
        if(cor ==NULL) return false;

        for(int i=0; i<V; i++){
            cor[i]=BRANCO; //Inicializa todos os vértices como não visitados
        }

        for (int i=0; i<V; i++){
            if(cor[i]==BRANCO){
                if (dfsCicloDirecionado(g, i, cor)){
                    free(cor);
                    return true;//Ciclo Encontrado
                }
            }
        }
        free(cor);
        return false; //Nenhum ciclo encontrado
    }else{

        //GRAFO NÃO DIRECIONADO: Vetor de Visitados
        bool* visitado=(bool*)malloc(V*sizeof(bool));
        if(visitado==NULL) return false;

        for(int i=0; i<V; i++){
            visitado[i]=false;
        }
        for(int i=0; i<V; i++){
            if(visitado[i]==false){
                if(dfsCicloNaoDirecionado(g, i, visitado, -1)){
                    free(visitado);
                    return true; //Ciclo Encontrado
                }
            }
        }
        free(visitado);
        return false; //Não possui ciclos
    }
}


//-----------------------------------------//
//DESAFIO COMPONENTES FORTEMENTES CONEXOS//
//----------------------------------------//


// ------------------------------------------------------------
// AUXILIARES ESTÁTICAS PARA KOSARAJU
// ------------------------------------------------------------

// 1ª DFS: preenche a pilha com a ordem de finalização (pós-ordem)
static void dfs_preenche_ordem(Grafo* g, int v, bool* visitado, int* pilha, int* topo) {
    visitado[v] = true;
    No* atual = g->lista[v];
    while (atual != NULL) {
        int vizinho = atual->destino;
        if (!visitado[vizinho]) {
            dfs_preenche_ordem(g, vizinho, visitado, pilha, topo);
        }
        atual = atual->prox;
    }
    // Após visitar todos os vizinhos, empilha o vértice
    pilha[(*topo)++] = v;
}

// 2ª DFS no grafo transposto: atribui um identificador de componente
static void dfs_transposto(Grafo* gT, int v, bool* visitado, int* componente, int id) {
    visitado[v] = true;
    componente[v] = id;
    No* atual = gT->lista[v];
    while (atual != NULL) {
        int vizinho = atual->destino;
        if (!visitado[vizinho]) {
            dfs_transposto(gT, vizinho, visitado, componente, id);
        }
        atual = atual->prox;
    }
}

// ------------------------------------------------------------
// FUNÇÃO PRINCIPAL KOSARAJU
// ------------------------------------------------------------
void kosaraju(Grafo* g) {
    if (g == NULL) {
        printf("\n[ERRO] Nenhum grafo carregado!\n");
        return;
    }

    int V = g->V;

    // 1. Primeira passagem: ordem de finalização
    bool* visitado = (bool*)calloc(V, sizeof(bool));
    int* pilha = (int*)malloc(V * sizeof(int));
    int topo = 0;

    for (int i = 0; i < V; i++) {
        if (!visitado[i]) {
            dfs_preenche_ordem(g, i, visitado, pilha, &topo);
        }
    }

    // 2. Construir o grafo transposto (inverter todas as arestas)
    Grafo* gT = (Grafo*)malloc(sizeof(Grafo));
    gT->V = V;
    gT->A = g->A; // número de arestas é o mesmo
    gT->lista = (No**)calloc(V, sizeof(No*));

    for (int u = 0; u < V; u++) {
        No* atual = g->lista[u];
        while (atual != NULL) {
            int v = atual->destino;
            int peso = atual->peso;

            // Adiciona aresta v -> u no transposto (inserção no início)
            No* novo = (No*)malloc(sizeof(No));
            novo->destino = u;
            novo->peso = peso;
            novo->prox = gT->lista[v];
            gT->lista[v] = novo;

            atual = atual->prox;
        }
    }

    // 3. Segunda passagem: DFS no transposto na ordem reversa da pilha
    bool* visitado2 = (bool*)calloc(V, sizeof(bool));
    int* componente = (int*)malloc(V * sizeof(int));
    int numComponentes = 0;

    while (topo > 0) {
        int v = pilha[--topo];
        if (!visitado2[v]) {
            dfs_transposto(gT, v, visitado2, componente, numComponentes);
            numComponentes++;
        }
    }

    // 4. Exibição dos resultados
    printf("\n=== COMPONENTES FORTEMENTE CONEXOS (Kosaraju) ===\n");
    printf("Número de componentes: %d\n", numComponentes);

    // Agrupa os vértices por componente para exibição
    int** grupos = (int**)malloc(numComponentes * sizeof(int*));
    int* tamanhos = (int*)calloc(numComponentes, sizeof(int));

    // Primeiro, contar quantos vértices em cada componente
    for (int i = 0; i < V; i++) {
        tamanhos[componente[i]]++;
    }
    // Alocar espaço para cada grupo
    for (int i = 0; i < numComponentes; i++) {
        grupos[i] = (int*)malloc(tamanhos[i] * sizeof(int));
        tamanhos[i] = 0; // reutilizar para preencher
    }
    // Preencher os grupos
    for (int i = 0; i < V; i++) {
        int c = componente[i];
        grupos[c][tamanhos[c]++] = i;
    }

    // Exibir cada componente
    for (int i = 0; i < numComponentes; i++) {
        printf("Componente %d: ", i + 1);
        for (int j = 0; j < tamanhos[i]; j++) {
            printf("%d ", grupos[i][j]);
        }
        printf("\n");
        free(grupos[i]);
    }

    // 5. Liberação de memória
    free(grupos);
    free(tamanhos);
    free(visitado);
    free(pilha);
    free(visitado2);
    free(componente);

    // Liberar o grafo transposto
    for (int i = 0; i < gT->V; i++) {
        No* atual = gT->lista[i];
        while (atual != NULL) {
            No* temp = atual;
            atual = atual->prox;
            free(temp);
        }
    }
    free(gT->lista);
    free(gT);
}


// ------------------------------------------------------------
// AUXILIARES PARA CAMINHO CRÍTICO
// ------------------------------------------------------------

// Função DFS auxiliar para preencher ordem (usada por obter_ordem_topologica)
static void dfs_ordem(Grafo* g, int v, int* cor, int* ordem, int* idx, bool* temCiclo) {
    cor[v] = CINZA;
    No* atual = g->lista[v];
    while (atual != NULL) {
        int vizinho = atual->destino;
        if (cor[vizinho] == BRANCO) {
            dfs_ordem(g, vizinho, cor, ordem, idx, temCiclo);
        } else if (cor[vizinho] == CINZA) {
            *temCiclo = true;
        }
        atual = atual->prox;
    }
    cor[v] = PRETO;
    ordem[(*idx)--] = v;
}

// Obtém a ordem topológica (retorna um vetor alocado dinamicamente)
// Retorna NULL se o grafo tiver ciclo ou se houver erro.
static int* obter_ordem_topologica(Grafo* g, int* tamanho) {
    int V = g->V;
    int* cor = (int*)calloc(V, sizeof(int));
    int* ordem = (int*)malloc(V * sizeof(int));
    int idx = V - 1;
    bool temCiclo = false;

    for (int i = 0; i < V; i++) {
        if (cor[i] == BRANCO) {
            dfs_ordem(g, i, cor, ordem, &idx, &temCiclo);
            if (temCiclo) break;
        }
    }

    free(cor);
    if (temCiclo) {
        free(ordem);
        *tamanho = 0;
        return NULL;
    }

    *tamanho = V;
    return ordem;
}

// ------------------------------------------------------------
// FUNÇÃO PRINCIPAL: CAMINHO CRÍTICO
// ------------------------------------------------------------
void caminho_critico(Grafo* g) {
    if (g == NULL) {
        printf("\n[ERRO] Nenhum grafo carregado!\n");
        return;
    }

    int V = g->V;
    int tamanho;
    int* ordem = obter_ordem_topologica(g, &tamanho);

    if (ordem == NULL) {
        printf("\n[ERRO] O grafo contém ciclo! Não é um DAG.\n");
        return;
    }

    // Inicializa distâncias como -infinito (usamos -1)
    int* dist = (int*)malloc(V * sizeof(int));
    int* pred = (int*)malloc(V * sizeof(int));
    for (int i = 0; i < V; i++) {
        dist[i] = -1;
        pred[i] = -1;
    }

    // A origem é o primeiro vértice da ordem topológica
    int origem = ordem[0];
    dist[origem] = 0;

    // Relaxamento na ordem topológica
    for (int i = 0; i < V; i++) {
        int u = ordem[i];
        if (dist[u] == -1) continue; // vértice inalcançável (não deveria ocorrer em DAG conexo)
        No* atual = g->lista[u];
        while (atual != NULL) {
            int v = atual->destino;
            int peso = atual->peso;
            if (dist[u] + peso > dist[v]) {
                dist[v] = dist[u] + peso;
                pred[v] = u;
            }
            atual = atual->prox;
        }
    }

    // Encontra o vértice com maior distância (ponto final do caminho crítico)
    int fim = origem;
    for (int i = 0; i < V; i++) {
        if (dist[i] > dist[fim]) fim = i;
    }

    // Exibe resultados
    printf("\n=== CAMINHO CRÍTICO (Maior Distância) ===\n");
    printf("Origem: %d\n", origem);
    printf("Destino: %d\n", fim);
    printf("Custo total: %d\n", dist[fim]);

    // Reconstrói o caminho
    int* caminho = (int*)malloc(V * sizeof(int));
    int tam = 0;
    int atual = fim;
    while (atual != -1) {
        caminho[tam++] = atual;
        atual = pred[atual];
    }

    printf("Caminho: ");
    for (int i = tam - 1; i >= 0; i--) {
        printf("%d%s", caminho[i], (i == 0) ? "\n" : " -> ");
    }

    // Libera memória
    free(ordem);
    free(dist);
    free(pred);
    free(caminho);
}
