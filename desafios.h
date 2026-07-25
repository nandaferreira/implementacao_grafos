#include <stdlib.h>
#include<stdbool.h>

// Estados para detecção de ciclo em grafo direcionado
#define BRANCO 0
#define CINZA 1
#define PRETO 2

/**
 * @brief Verifica se o grafo possui ciclos (direcionado ou não-direcionado)
 * @param g Ponteiro para o Grafo
 * @param ehDirecionado true se o grafo for direcionado, false se não-direcionado
 * @return true se contiver ciclo, false caso contrário
 */
bool tem_ciclo(Grafo* g, bool ehDirecionado);