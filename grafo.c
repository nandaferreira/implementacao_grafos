//"corpo" das funções

#include <stdio.h>
#include <stdlib.h>
//#include <string.h>

typedef struct No{
    int destino;
    int peso;
    struct No *prox;
}No;


typedef struct {
    int V; //numero de vertices
    int A; //numero de arestas
    No **lista; //lista de adjacencia
}Grafo;