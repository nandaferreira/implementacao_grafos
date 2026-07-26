#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "estatistica.h"

//----->Funções Auxiliares: Conectividade<------//

//OBS: Utilizando busca em profundidade (DFS) para verificar a conectividade do grafo

static void dfsConexo(Grafo* g, int u, bool* visitado, int* visitadosCount){
    visitado[u]=true;
    (*visitadosCount)++;


    No* p = g->lista[u];
      while(p!=NULL){
        int v= p->destino;
        if(visitado[v]==false){
          dfsConexo(g, v, visitado, visitadosCount);
        }
    p= p->prox;
    }
}

static bool verificarConectividade(Grafo* g){
    
    if(g==NULL || g->V==0) return false;

    bool* visitado = (bool*) calloc(g->V, sizeof(bool));
    int visitadosCount=0;

    //Inicia a busca DFS a partir do véritice O
    dfsConexo(g, 0, visitado, &visitadosCount);

    free(visitado);

    //Se a busca alcançar todos os vértices, o grafo é conexo 
    return visitadosCount == g->V;
}

//----->Funções Auxiliares: Detecção de Ciclos<------//

//Ciclo para Grafo não Direcionado (Usa o nó Pai)

static bool dfsCicloNaoDirecionado(Grafo *g, int u, int pai, bool* visitado){
    visitado[u] = true;

    No* p = g->lista[u];
    while (p != NULL){
        int v = p->destino;

        if (visitado[v] == false){
            if (dfsCicloNaoDirecionado(g, v, u, visitado)) return true;
        }
        else if (v != pai){
            return true;
        }
        p = p->prox;
    }
    return false;
}

//Ciclo para Grafo Direcionado(usa pilha de recursão em 3 estados)
//estado: 0 = não visitado, 1 = visitando, 2 = visitado

static bool dfsCicloDirecionado(Grafo* g, int u, int* estado){
    estado[u] = 1;

    No* p = g->lista[u];
    while (p != NULL){
        int v = p->destino;

        if (estado[v] == 1){
            return true; // Aresta de retorno encontrada = ciclo detectado
        }

        if (estado[v] == 0 && dfsCicloDirecionado(g, v, estado)){
            return true;
        }
        p = p->prox;
    }

    estado[u] = 2; // Marca como visitado
    return false;
}

static bool verificarCiclos(Grafo* g, bool ehDirecionado){
    if (g == NULL || g->V == 0) return false;

    if (ehDirecionado == false){
        bool* visitado = (bool*) calloc(g->V, sizeof(bool));
        if (visitado == NULL) return false;

        for (int i = 0; i < g->V; i++){
            if (visitado[i] == false){
                if (dfsCicloNaoDirecionado(g, i, -1, visitado)){
                    free(visitado);
                    return true;
                }
            }
        }

        free(visitado);
    } else {
        int* estado = (int*) calloc(g->V, sizeof(int));
        if (estado == NULL) return false;

        for (int i = 0; i < g->V; i++){
            if (estado[i] == 0){
                if (dfsCicloDirecionado(g, i, estado)){
                    free(estado);
                    return true;
                }
            }
        }

        free(estado);
    }

    return false;
}

//Função Principal para calcular as estatísticas do grafo
EstatisticasGrafo calcularEstatisticas(Grafo* g, bool ehDirecionado){
    EstatisticasGrafo est= {0};

    if (g==NULL|| g->V==0) return est;

    //1. número de vértices e arestas
    est.numVertices=g->V;
    est.numArestas=g->A;
    est.ehDirecionado=ehDirecionado;

    //2. Grau de cada vértice
    est.graus=(int*) calloc(g->V, sizeof(int));
    for(int u=0; u<g->V; u++){
        No* p=g->lista[u];
        while(p!=NULL){
            est.graus[u]++;
            p=p->prox;
        }
    }

    //3. Verificadno se grafo é conexo
    est.ehConexo=verificarConectividade(g);

    //4. Verificando se grafo tem ciclos
    est.temCiclos=verificarCiclos(g, ehDirecionado);

    //5.Densidade do grafo
    if(g->V>1){
        if(ehDirecionado){
            est.densidade=(float)g->A/(g->V*(g->V-1));

        }else{
            est.densidade=(float)(2*g->A)/(g->V*(g->V-1));
        }
    }else{
        est.densidade=0.0f;
    }   
    return est;
}

//Libera o vetor de graus alocado dinamicamente

void liberarEstatisticas(EstatisticasGrafo* est){
    if(est!=NULL && est->graus !=NULL){
        free(est->graus);
        est->graus=NULL;
    }
}

void exibirRelatorioEstatisticas(Grafo* g, bool ehDirecionado){
    if (g==NULL){
        printf("Grafo nulo. Não é possível calcular estatísticas.\n");
        return;
    }

    EstatisticasGrafo est = calcularEstatisticas(g, ehDirecionado);

    printf("\n===========================================\n");
    printf("    Relatorio de Estatisticas do Grafo:      \n");
    printf("=============================================\n");
 
    printf("Numero de vertices: %d\n", est.numVertices);
    printf("Numero de arestas: %d\n", est.numArestas);
    printf("Grafo eh direcionado? %s\n", est.ehDirecionado ? "Sim" : "Nao");
    printf("Grafo eh conexo? %s\n", est.ehConexo ? "Sim" : "Nao");
    printf("Grafo tem ciclos? %s\n", est.temCiclos ? "Sim" : "Nao");
    printf("Densidade do grafo: %.4f %.4f\n", est.densidade, est.densidade*100);
    printf("-----------------------------------------------\n");
    printf("Grau de cada vertice:\n");
    for (int i=0; i< est.numVertices; i++){
        printf("Vertice %d: Grau = %d\n", i, est.graus[i]);
    }
    printf("-----------------------------------------------\n");

    liberarEstatisticas(&est);
}