#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h> 
#include "grafo.h"
#include "desafios.h"
#include <time.h>
#include <limits.h>
#define INF INT_MAX

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
    printf("Numero de componentes: %d\n", numComponentes);

    // Agrupa os vertices por componente para exibicao
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
        printf("\n[ERRO] O grafo contem ciclo! Nao e um DAG.\n");
        return;
    }

    // Inicializa distancias como -infinito (usamos -1)
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
    printf("\n=== CAMINHO CRITICO (Maior Distancia) ===\n");
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


// ============================================
// FUNÇÃO: Gerar grafo aleatório para teste
// ============================================
Grafo* gerarGrafoTeste(int V, int A) {
    printf("Gerando grafo com %d vertices e %d arestas...\n", V, A);
    
    Grafo* grafo = (Grafo*)malloc(sizeof(Grafo));
    if (grafo == NULL) return NULL;
    
    grafo->V = V;
    grafo->A = 0;
    grafo->lista = (No**)calloc(V, sizeof(No*));
    if (grafo->lista == NULL) {
        free(grafo);
        return NULL;
    }
    
    bool** matriz = (bool**)calloc(V, sizeof(bool*));
    for (int i = 0; i < V; i++) {
        matriz[i] = (bool*)calloc(V, sizeof(bool));
    }
    
    // Garante conexidade
    for (int i = 0; i < V - 1; i++) {
        int peso = (rand() % 20) + 1;
        adicionar_aresta(grafo, i, i + 1, peso);
        matriz[i][i + 1] = true;
        matriz[i + 1][i] = true;
        grafo->A++;
    }
    
    // Adiciona arestas aleatórias
    int tentativas = 0;
    while (grafo->A < A && tentativas < A * 10) {
        int origem = rand() % V;
        int destino = rand() % V;
        
        if (origem == destino || matriz[origem][destino]) {
            tentativas++;
            continue;
        }
        
        int peso = (rand() % 20) + 1;
        adicionar_aresta(grafo, origem, destino, peso);
        matriz[origem][destino] = true;
        matriz[destino][origem] = true;
        grafo->A++;
        tentativas = 0;
    }
    
    for (int i = 0; i < V; i++) {
        free(matriz[i]);
    }
    free(matriz);
    
    printf("Grafo gerado com sucesso!\n");
    return grafo;
}

// ============================================
// FUNÇÕES DE TESTE PARA CADA OPERAÇÃO
// ============================================

void executarTesteDFS(Grafo* grafo, int V) {
    printf("Executando DFS para todos os vertices...\n");
    clock_t inicio = clock();
    
    // USA A VERSAO SILENCIOSA!
    executarDFSCompleta(grafo);
    
    clock_t fim = clock();
    double tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    printf("Tempo DFS: %.4f segundos\n", tempo);
    printf("Media por vertice: %.6f segundos\n", tempo / V);
}

void executarTesteBFS(Grafo* grafo, int V) {
    printf("Executando BFS para todos os vertices...\n");
    clock_t inicio = clock();
    
    // USA A VERSAO SILENCIOSA!
    executarBFSCompleta(grafo);
    
    clock_t fim = clock();
    double tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    printf("Tempo BFS: %.4f segundos\n", tempo);
    printf("Media por vertice: %.6f segundos\n", tempo / V);
}


void executarTesteTopologica(Grafo* grafo, int V) {
    printf("Executando Ordenacao Topologica...\n");
    clock_t inicio = clock();
    
    ordenacao_topologica(grafo);
    
    clock_t fim = clock();
    double tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    printf("Tempo Ordenacao Topologica: %.4f segundos\n", tempo);
}

void executarTesteCiclo(Grafo* grafo, int V) {
    printf("Executando Deteccao de Ciclos...\n");
    clock_t inicio = clock();
    
    // ARMAZENA O RESULTADO para evitar otimizacao
    bool resultado = tem_ciclo(grafo, false);
    
    clock_t fim = clock();
    double tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    
    // EXIBE O RESULTADO
    printf("Resultado: %s\n", resultado ? "POSSUI CICLO" : "NAO POSSUI CICLO");
    printf("Tempo Deteccao de Ciclos: %.6f segundos\n", tempo);
    printf("Media por vertice: %.6f segundos\n", tempo / V);
}

void executarTesteCaminhoCritico(Grafo* grafo, int V) {
    printf("Executando Caminho Critico...\n");
    clock_t inicio = clock();
    
    caminho_critico(grafo);
    
    clock_t fim = clock();
    double tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    printf("Tempo Caminho Critico: %.4f segundos\n", tempo);
}

// ============================================
// FUNÇÕES SILENCIOSAS PARA TESTE DE DESEMPENHO
// ============================================

// DFS silenciosa (sem prints)
static void dfsSilencioso(Grafo* grafo, int vertice, bool* visitado) {
    visitado[vertice] = true;
    
    No* atual = grafo->lista[vertice];
    while (atual != NULL) {
        int vizinho = atual->destino;
        if (!visitado[vizinho]) {
            dfsSilencioso(grafo, vizinho, visitado);
        }
        atual = atual->prox;
    }
}

// BFS silenciosa (sem prints)
static void bfsSilencioso(Grafo* grafo, int inicio, bool* visitado) {
    int V = grafo->V;
    int* fila = (int*)malloc(V * sizeof(int));
    int frente = 0, tras = 0;
    
    visitado[inicio] = true;
    fila[tras++] = inicio;
    
    while (frente < tras) {
        int vertice = fila[frente++];
        
        No* atual = grafo->lista[vertice];
        while (atual != NULL) {
            int vizinho = atual->destino;
            if (!visitado[vizinho]) {
                visitado[vizinho] = true;
                fila[tras++] = vizinho;
            }
            atual = atual->prox;
        }
    }
    
    free(fila);
}

// Função para executar DFS em todos os vértices (para teste)
void executarDFSCompleta(Grafo* grafo) {
    int V = grafo->V;
    bool* visitado = (bool*)calloc(V, sizeof(bool));
    
    for (int i = 0; i < V; i++) {
        if (!visitado[i]) {
            dfsSilencioso(grafo, i, visitado);
        }
    }
    
    free(visitado);
}

// Função para executar BFS em todos os vértices (para teste)
void executarBFSCompleta(Grafo* grafo) {
    int V = grafo->V;
    bool* visitado = (bool*)calloc(V, sizeof(bool));
    
    for (int i = 0; i < V; i++) {
        if (!visitado[i]) {
            bfsSilencioso(grafo, i, visitado);
        }
    }
    
    free(visitado);
}

// Versão silenciosa do Kosaraju (sem prints)
static void kosarajuSilencioso(Grafo* g) {
    if (g == NULL) return;
    
    int V = g->V;
    bool* visitado = (bool*)calloc(V, sizeof(bool));
    int* pilha = (int*)malloc(V * sizeof(int));
    int topo = 0;
    
    // Primeira passagem
    for (int i = 0; i < V; i++) {
        if (!visitado[i]) {
            dfs_preenche_ordem(g, i, visitado, pilha, &topo);
        }
    }
    
    // Construir transposto
    Grafo* gT = (Grafo*)malloc(sizeof(Grafo));
    gT->V = V;
    gT->A = g->A;
    gT->lista = (No**)calloc(V, sizeof(No*));
    
    for (int u = 0; u < V; u++) {
        No* atual = g->lista[u];
        while (atual != NULL) {
            int v = atual->destino;
            int peso = atual->peso;
            No* novo = (No*)malloc(sizeof(No));
            novo->destino = u;
            novo->peso = peso;
            novo->prox = gT->lista[v];
            gT->lista[v] = novo;
            atual = atual->prox;
        }
    }
    
    // Segunda passagem (sem prints)
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
    
    // Libera memória
    free(visitado);
    free(pilha);
    free(visitado2);
    free(componente);
    
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

// Função de teste usando versão silenciosa
void executarTesteKosarajuSilencioso(Grafo* grafo, int V) {
    printf("Executando Kosaraju (Componentes Fortemente Conexos)...\n");
    clock_t inicio = clock();
    
    kosarajuSilencioso(grafo);  // Sem prints
    
    clock_t fim = clock();
    double tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    printf("Tempo Kosaraju: %.6f segundos\n", tempo);
}

// ============================================
// FUNÇÃO PRINCIPAL: Menu de Teste de Desempenho
// ============================================
void testarDesempenho() {
    printf("\n╔══════════════════════════════════════════════════════════════════╗\n");
    printf("║           TESTE DE DESEMPENHO COM GRAFOS DE 1000+ VERTICES      ║\n");
    printf("╚══════════════════════════════════════════════════════════════════╝\n");
    printf("\n");
    
    // Seleção do tamanho do grafo
    printf("Escolha o tamanho do grafo para teste:\n");
    printf("1. 1000 vertices, 5000 arestas\n");
    printf("2. 2000 vertices, 10000 arestas\n");
    printf("3. 3000 vertices, 15000 arestas\n");
    printf("4. 4000 vertices, 20000 arestas\n");
    printf("5. 5000 vertices, 25000 arestas\n");
    printf("6. Voltar\n");
    printf("\nEscolha uma opcao: ");
    
    int opTamanho;
    if (scanf("%d", &opTamanho) != 1 || opTamanho < 1 || opTamanho > 6) {
        while (getchar() != '\n');
        printf("\n[ERRO] Opcao invalida!\n");
        return;
    }
    
    if (opTamanho == 6) return;
    
    // Define tamanhos
    int tamanhos[][2] = {
        {1000, 5000},
        {2000, 10000},
        {3000, 15000},
        {4000, 20000},
        {5000, 25000}
    };
    
    int V = tamanhos[opTamanho - 1][0];
    int A = tamanhos[opTamanho - 1][1];
    
    // Gera o grafo
    printf("\n");
    Grafo* grafo = gerarGrafoTeste(V, A);
    if (grafo == NULL) {
        printf("Erro ao gerar grafo!\n");
        return;
    }
    
    // Menu de operações para testar
    int opTeste = 0;
    while (opTeste != 8) {
        printf("\n╔══════════════════════════════════════════════════════════════════╗\n");
        printf("║  Grafos: %d vertices, %d arestas                                ║\n", V, grafo->A);
        printf("╠══════════════════════════════════════════════════════════════════╣\n");
        printf("║  Escolha a operacao para testar:                                ║\n");
        printf("║  1. Busca em Profundidade (DFS)                                ║\n");
        printf("║  2. Busca em Largura (BFS)                                     ║\n");
        printf("║  3. Ordenacao Topologica                                       ║\n");
        printf("║  4. Deteccao de Ciclos                                         ║\n");
        printf("║  5. Kosaraju (Componentes Fortemente Conexos)                  ║\n");
        printf("║  6. Caminho Critico (DAG)                                      ║\n");
        printf("║  7. Executar TODOS os testes                                   ║\n");
        printf("║  8. Voltar                                                     ║\n");
        printf("╚══════════════════════════════════════════════════════════════════╝\n");
        printf("Escolha uma opcao: ");
        
        if (scanf("%d", &opTeste) != 1) {
            while (getchar() != '\n');
            printf("\n[ERRO] Opcao invalida!\n");
            continue;
        }
        
        printf("\n");
        
        switch (opTeste) {
            case 1:
                executarTesteDFS(grafo, V);
                break;
            case 2:
                executarTesteBFS(grafo, V);
                break;
            case 3:
                executarTesteTopologica(grafo, V);
                break;
            case 4:
                executarTesteCiclo(grafo, V);
                break;
            case 5:
                executarTesteKosarajuSilencioso(grafo, V);
                break;
            case 6:
                executarTesteCaminhoCritico(grafo, V);
                break;
            case 7: {
                printf("╔══════════════════════════════════════════════════════════════════╗\n");
                printf("║           EXECUTANDO TODOS OS TESTES                            ║\n");
                printf("║           Isso pode levar varios minutos...                     ║\n");
                printf("╚══════════════════════════════════════════════════════════════════╝\n\n");
                
                executarTesteDFS(grafo, V);
                printf("\n");
                executarTesteBFS(grafo, V);
                printf("\n");
                executarTesteTopologica(grafo, V);
                printf("\n");
                executarTesteCiclo(grafo, V);
                printf("\n");
                executarTesteKosarajuSilencioso(grafo, V);
                printf("\n");
                executarTesteCaminhoCritico(grafo, V);
                break;
            }
            case 8:
                printf("Voltando...\n");
                break;
            default:
                printf("\n[ERRO] Opcao invalida!\n");
        }
        
        if (opTeste != 8) {
            printf("\nPressione ENTER para continuar...");
            while (getchar() != '\n');
            getchar();
        }
    }
    
    libera_grafo(grafo);
}