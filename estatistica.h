/* ESTATÍSTICAS
 Número de vértices e arestas
• Grau de cada vértice
• Grafo é conexo?
• Grafo tem ciclos?
• Grafo é direcionado?
• Densidade do grafo

*/
#ifndef ESTATISTICA_H
#define ESTATISTICA_H

#include "grafo.h"

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
void exibirEstatisticas(EstatisticasGrafo* est);
void liberarEstatisticas(EstatisticasGrafo* est);

#endif

