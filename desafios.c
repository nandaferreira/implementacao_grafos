#include <stdlib.h>
#include<stdbool.h>
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


