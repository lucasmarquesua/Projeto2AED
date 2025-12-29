//
// Algoritmos e Estruturas de Dados --- 2025/2026
//
// Joaquim Madeira - Nov 2025
//
// GraphDominatingSets - Computing Vertex Dominating Sets for UNDIRECTED graphs
//

// Student Name : Lucas Gaspar Marques
// Student Number : 125982
// Student Name : Gonçalo Carapinheira Duarte
// Student Number : 103871

/*** COMPLETE THE GraphIsDominatingSet FUNCTION ***/
/*** COMPLETE THE GraphMinDominatingSet FUNCTION ***/
/*** COMPLETE THE GraphMinWeightDominatingSet FUNCTION ***/

#include "GraphDominatingSets.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
#include "IndicesSet.h"
#include "instrumentation.h"

//
// TO BE COMPLETED
//
// Check if the given set is a dominating set for the graph
// Return 1 if true, or 0 otherwise
//
// A dominating set is a set of graph vertices such that every other
// graph vertex not in the set is adjacent to a graph vertex in the set
//
int GraphIsDominatingSet(const Graph* g, IndicesSet* vertSet) {
  assert(g != NULL);
  assert(GraphIsDigraph(g) == 0);
  assert(IndicesSetIsEmpty(vertSet) == 0);

  //
  // TO BE COMPLETED
  //
  // Para cada vértice no grafo
  // Obter todos os vértices do grafo
   IndicesSet* allVertices = GraphGetSetVertices(g);
    int result = 1; // Assumimos verdadeiro

    int v = IndicesSetGetFirstElem(allVertices);
    while (v != -1) {
        // Se v já está no conjunto, ok
        if (IndicesSetContains(vertSet, v)) {
            v = IndicesSetGetNextElem(allVertices);
            continue;
        }

        // Se não, verificar vizinhos
        int isCovered = 0;
        IndicesSet* neighbors = GraphGetSetAdjacentsTo(g, v);
        
        int neighbor = IndicesSetGetFirstElem(neighbors);
        while (neighbor != -1) {
            if (IndicesSetContains(vertSet, neighbor)) {
                isCovered = 1;
                break;
            }
            neighbor = IndicesSetGetNextElem(neighbors);
        }
        
        IndicesSetDestroy(&neighbors);

        if (!isCovered) {
            result = 0;
            break; // Sai do ciclo, e a limpeza é feita no final
        }

        v = IndicesSetGetNextElem(allVertices);
    }

    IndicesSetDestroy(&allVertices); // Limpeza única aqui
    return result;
}

//
// TO BE COMPLETED
//
// Compute a MIN VERTEX DOMINATING SET of the graph
// using an EXHAUSTIVE SEARCH approach
// Return the/a dominating set
//
IndicesSet* GraphComputeMinDominatingSet(const Graph* g) {
  assert(g != NULL);
  assert(GraphIsDigraph(g) == 0);

  //
  // TO BE COMPLETED
  //
  unsigned int range = GraphGetVertexRange(g);
    
    // Mapear vértices reais para um array contínuo (0 a N-1)
    // Isto resolve o problema de índices não sequenciais ou buracos
    IndicesSet* allVerticesSet = GraphGetSetVertices(g);
    int numVertices = IndicesSetGetNumElems(allVerticesSet);
    
    // Se o grafo for vazio
    if (numVertices == 0) {
        IndicesSetDestroy(&allVerticesSet);
        return IndicesSetCreateEmpty(range);
    }

    // Array para guardar os IDs reais dos vértices
    unsigned int* vertexMap = malloc(numVertices * sizeof(unsigned int));
    int idx = 0;
    int v = IndicesSetGetFirstElem(allVerticesSet);
    while (v != -1) {
        vertexMap[idx++] = v;
        v = IndicesSetGetNextElem(allVerticesSet);
    }
    IndicesSetDestroy(&allVerticesSet);

    // Preparar variáveis para o melhor conjunto encontrado
    IndicesSet* bestSet = NULL;
    int minSize = numVertices + 1; // Começa com um valor impossivelmente alto

    // Iterar por TODOS os subconjuntos possíveis usando uma máscara de bits
    // 1ULL << numVertices cria o número 2^N.
    unsigned long long maxMask = (1ULL << numVertices);

    for (unsigned long long mask = 1; mask < maxMask; mask++) {
        
        // Otimização: Contar bits da máscara (tamanho do subconjunto atual)
        // Se o tamanho atual já for maior ou igual ao melhor encontrado, ignoramos
        int currentSize = 0;
        unsigned long long tempMask = mask;
        while(tempMask) { currentSize += tempMask & 1; tempMask >>= 1; }
        
        if (bestSet != NULL && currentSize >= minSize) {
            continue;
        }

        // Construir o conjunto candidato a partir da máscara
        IndicesSet* candidate = IndicesSetCreateEmpty(range);
        for (int i = 0; i < numVertices; i++) {
            // Se o bit i está a 1, adicionamos o vértice correspondente
            if ((mask >> i) & 1) {
                IndicesSetAdd(candidate, vertexMap[i]);
            }
        }

        // Verificar se é dominante
        if (GraphIsDominatingSet(g, candidate)) {
            // Como já verificámos o tamanho lá em cima, se chegámos aqui
            // é porque este é necessariamente melhor (menor)
            if (bestSet != NULL) {
                IndicesSetDestroy(&bestSet);
            }
            bestSet = candidate; // O candidato passa a ser o bestSet (não destruir!)
            minSize = currentSize;
        } else {
            // Se não serve, libertamos a memória
            IndicesSetDestroy(&candidate);
        }
    }

    free(vertexMap);

    // Fallback de segurança
    if (bestSet == NULL) {
        return GraphGetSetVertices(g);
    }

    return bestSet;
}

//
// TO BE COMPLETED
//
// Compute a MIN WEIGHT VERTEX DOMINATING SET of the graph
// using an EXHAUSTIVE SEARCH approach
// Return the dominating set
//
IndicesSet* GraphComputeMinWeightDominatingSet(const Graph* g) {
  assert(g != NULL);
  assert(GraphIsDigraph(g) == 0);

  //
  // TO BE COMPLETED
  //
  unsigned int range = GraphGetVertexRange(g);

    // Obter todos os vértices reais do grafo
    // Isto é necessário porque os índices podem não ser sequenciais (ex: 1, 5, 20)
    IndicesSet* allVerticesSet = GraphGetSetVertices(g);
    int numVertices = IndicesSetGetNumElems(allVerticesSet);
    
    // Se o grafo for vazio
    if (numVertices == 0) {
        IndicesSetDestroy(&allVerticesSet);
        return IndicesSetCreateEmpty(range);
    }

    // Criar um mapa: índice 0..N-1 -> ID Real do Vértice
    unsigned int* vertexMap = malloc(numVertices * sizeof(unsigned int));
    if (vertexMap == NULL) abort();

    int idx = 0;
    int v = IndicesSetGetFirstElem(allVerticesSet);
    while (v != -1) {
        vertexMap[idx++] = v;
        v = IndicesSetGetNextElem(allVerticesSet);
    }
    IndicesSetDestroy(&allVerticesSet);

    // Pré-calcular os pesos dos vértices para acesso rápido
    double* weights = GraphComputeVertexWeights(g); // Devolve array de tamanho 'range'

    // Variáveis para guardar a melhor solução encontrada
    IndicesSet* bestSet = NULL;
    double minWeight = -1.0; // -1.0 serve como flag de "ainda não encontrei nada"

    // PROCURA EXAUSTIVA (Bitmask)
    // Gera todas as combinações possíveis de vértices (2^numVertices)
    // De notar que o limite é 64 vértices devido ao unsigned long long.
    // Como a complexidade é O(2^N), o tempo esgota-se muito antes de chegarmos aos 64 vértices.
    unsigned long long maxMask = (1ULL << numVertices);

    for (unsigned long long mask = 1; mask < maxMask; mask++) {
        
        double currentWeight = 0.0;
        
        // --- Otimização Preliminar de Peso ---
        // Calcula o peso deste subconjunto ANTES de criar o IndicesSet.
        // Se o peso já for maior que o melhor peso encontrado (minWeight),
        // nem vale a pena perder tempo a criar o conjunto e testar dominância.
        for (int i = 0; i < numVertices; i++) {
            if ((mask >> i) & 1) {
                unsigned int realID = vertexMap[i];
                if (weights[realID] != -1.0) {
                    currentWeight += weights[realID];
                }
            }
        }

        // Se já encontrámos uma solução antes E a atual é mais pesada, ignora.
        if (minWeight != -1.0 && currentWeight >= minWeight) {
            continue;
        }

        // --- Construção e Verificação ---
        // Se passou o teste do peso, construímos o conjunto candidato
        IndicesSet* candidate = IndicesSetCreateEmpty(range);
        for (int i = 0; i < numVertices; i++) {
            if ((mask >> i) & 1) {
                IndicesSetAdd(candidate, vertexMap[i]);
            }
        }

        // Verifica se é um conjunto dominante
        if (GraphIsDominatingSet(g, candidate)) {
            // Se chegou aqui, já sabemos que o peso é menor (devido ao check acima)
            // ou é a primeira solução válida encontrada.
            if (bestSet != NULL) {
                IndicesSetDestroy(&bestSet);
            }
            bestSet = candidate;
            minWeight = currentWeight;
        } else {
            // Se não é dominante, liberta a memória
            IndicesSetDestroy(&candidate);
        }
    }

    // Limpeza de memória auxiliar
    free(vertexMap);
    free(weights);

    // Se o grafo tem vértices mas não encontrámos conjunto (impossível em grafos normais),
    // devolve todos os vértices por segurança.
    if (bestSet == NULL) {
        return GraphGetSetVertices(g);
    }

    return bestSet;
}
