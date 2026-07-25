
#ifndef ESTATISTICA_H
#define ESTATISTICA_H

#include "grafo.h"
#include "estatistica.h"
#include <stdbool.h>

//Estrutura para agrupar as métricas das Estatísticas do grafo

typedef struct{

    int numVertices;
    int numArestas;
    int* graus;
    int ehConexo;
    int temCiclos;
    int ehDirecionado;
    float densidade;
} EstatisticasGrafo;

//Protótipos das funções 
EstatisticasGrafo calcularEstatisticas(Grafo* grafo, bool ehDirecionado);
void exibirRelatorioEstatisticas(Grafo* g, bool ehDirecionado);
void liberarEstatisticas(EstatisticasGrafo* est);

#endif

