#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Graph.h"
#include "GraphDominatingSets.h"
#include "IndicesSet.h"

// Gera um grafo aleatório não orientado
Graph* CreateRandomGraph(int n, double prob, int weighted) {
    Graph* g = GraphCreate(n, 0, weighted); // 0 = Não orientado
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if ((double)rand() / RAND_MAX < prob) {
                if (weighted) {
                    // Pesos aleatórios entre 1.0 e 10.0
                    double w = 1.0 + ((double)rand() / RAND_MAX) * 9.0;
                    GraphAddWeightedEdge(g, i, j, w);
                } else {
                    GraphAddEdge(g, i, j);
                }
            }
        }
    }
    return g;
}

int main() {
    srand(time(NULL));
    
    printf("--- INICIO DOS TESTES ---\n");
    printf("Vertices, Tempo_MinDominating(s), Tempo_MinWeight(s)\n");

    // Vamos testar de 5 até 25 vértices (mais que isso vai demorar muito)
    int tamanhos[] = {5, 10, 15, 20, 22, 24, 26}; 
    int num_steps = 7;

    for (int i = 0; i < num_steps; i++) {
        int n = tamanhos[i];
        
        // 1. Teste Não Pesado (MinDominatingSet)
        Graph* g1 = CreateRandomGraph(n, 0.5, 0);
        
        clock_t start = clock();
        IndicesSet* res1 = GraphComputeMinDominatingSet(g1);
        clock_t end = clock();
        double time1 = (double)(end - start) / CLOCKS_PER_SEC;
        
        IndicesSetDestroy(&res1);
        GraphDestroy(&g1);

        // 2. Teste Pesado (MinWeightDominatingSet)
        Graph* g2 = CreateRandomGraph(n, 0.5, 1);
        
        start = clock();
        IndicesSet* res2 = GraphComputeMinWeightDominatingSet(g2);
        end = clock();
        double time2 = (double)(end - start) / CLOCKS_PER_SEC;
        
        IndicesSetDestroy(&res2);
        GraphDestroy(&g2);

        printf("%d, %f, %f\n", n, time1, time2);
        fflush(stdout); // Garante que vês o output mesmo se crashar
    }

    printf("--- FIM DOS TESTES ---\n");
    return 0;
}