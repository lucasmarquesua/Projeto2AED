#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Graph.h"
#include "IndicesSet.h"

// Função auxiliar para criar grafos aleatórios
Graph* CreateRandomGraph(int n, double prob) {
    Graph* g = GraphCreate(n, 0, 0); // Não orientado, não pesado
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if ((double)rand() / RAND_MAX < prob) {
                GraphAddEdge(g, i, j);
            }
        }
    }
    return g;
}

int main() {
    // Semente para aleatoriedade
    srand(time(NULL));

    printf("N_VERTICES, TEMPO_SEC\n");

    // Testar para 5, 10, 15, 20... vértices
    // ATENÇÃO: 25 ou 30 pode demorar MUITO tempo (exponencial)
    int tamanhos[] = {5, 10, 15, 20, 22, 25}; 
    int num_tamanhos = 6;

    for (int i = 0; i < num_tamanhos; i++) {
        int n = tamanhos[i];
        
        // Criar grafo aleatório (densidade 30%)
        Graph* g = CreateRandomGraph(n, 0.3);

        // Medir tempo
        clock_t start = clock();
        
        IndicesSet* solution = GraphComputeMinDominatingSet(g);
        
        clock_t end = clock();
        double time_spent = (double)(end - start) / CLOCKS_PER_SEC;

        printf("%d, %f\n", n, time_spent);

        // Limpeza
        IndicesSetDestroy(&solution);
        GraphDestroy(&g);
    }

    return 0;
}