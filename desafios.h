#ifndef DESAFIO_H
#define DESAFIO_H

#include <stdbool.h>
#include "grafo.h"

// Estados para detecção de ciclo (já existentes)
#define BRANCO 0
#define CINZA  1
#define PRETO  2

// Protótipos existentes
bool tem_ciclo(Grafo* g, bool ehDirecionado);

//Kosaraju para componentes fortemente conexos
/**
 * @brief Encontra e exibe os componentes fortemente conexos usando Kosaraju.
 * @param g Ponteiro para o Grafo (deve ser direcionado).
 */
void kosaraju(Grafo* g);

//Caminho Crítico (maior distância em DAG)
/**
 * @brief Calcula e exibe o caminho crítico (maior distância) em um DAG.
 * @param g Ponteiro para o Grafo (deve ser direcionado e acíclico).
 */
void caminho_critico(Grafo* g);

#endif
