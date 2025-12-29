//
// Algoritmos e Estruturas de Dados --- 2025/2026
//
// Joaquim Madeira - Nov 2025
//
// GraphDominatingSets - Computing Vertex Dominating Sets for UNDIRECTED graphs
//

// Student Name :
// Student Number :
// Student Name :
// Student Number :

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
  uint16_t v = IndicesSetGetFirstElem(allVertices);
  while (v != -1) {
    // Se o vértice não está dominado
    if (!IndicesSetContains(vertSet, v)) {
      // Obter vértices adjacentes a v
      IndicesSet* adjacents = GraphGetSetAdjacentsTo(g, v);
      
      // Verificar se algum vértice adjacente está no conjunto dominante
      int isDominated = 0;
      uint16_t adj = IndicesSetGetFirstElem(adjacents);
      while (adj != -1) {
        if (IndicesSetContains(vertSet, adj)) {
          isDominated = 1;
          break;
        }
        adj = IndicesSetGetNextElem(adjacents);
      }
      
      IndicesSetDestroy(&adjacents);
      
      // Se encontrou um vértice não dominado, não é conjunto dominante
      if (!isDominated) {
        IndicesSetDestroy(&allVertices);
        return 0;
      }
    }
    
    v = IndicesSetGetNextElem(allVertices);
  }
  
  IndicesSetDestroy(&allVertices);
  return 1;
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
  uint16_t range = GraphGetVertexRange(g);
  uint16_t n = GraphGetNumVertices(g);
  
  // Se o grafo não tem vértices
  if (n == 0) {
    return IndicesSetCreateEmpty(range);
  }
  
  // Obter todos os vértices do grafo
  IndicesSet* allVertices = GraphGetSetVertices(g);
  
  // Converter para array para facilitar o acesso por índice
  uint16_t vertices[n];
  uint16_t idx = 0;
  uint16_t v = IndicesSetGetFirstElem(allVertices);
  while (v != -1) {
    vertices[idx++] = v;
    v = IndicesSetGetNextElem(allVertices);
  }
  
  IndicesSet* result = NULL;
  
  // Busca exaustiva: testar todos os subconjuntos não vazios
  // Tentar por tamanhos crescentes para encontrar o menor primeiro
  for (uint16_t size = 1; size <= n && result == NULL; size++) {
    // Usar máscara de bits para gerar todos os subconjuntos de tamanho 'size'
    for (unsigned long mask = 0; mask < (1UL << n); mask++) {
      // Contar bits setados
      int bitCount = 0;
      unsigned long temp = mask;
      while (temp) {
        bitCount += temp & 1;
        temp >>= 1;
      }
      
      if (bitCount == size) {
        // Criar conjunto candidato
        IndicesSet* candidate = IndicesSetCreateEmpty(range);
        for (uint16_t i = 0; i < n; i++) {
          if (mask & (1UL << i)) {
            IndicesSetAdd(candidate, vertices[i]);
          }
        }
        
        // Verificar se é conjunto dominante
        if (GraphIsDominatingSet(g, candidate)) {
          result = candidate;  // Encontrou o menor conjunto dominante
          break;
        }
        
        IndicesSetDestroy(&candidate);
      }
    }
  }
  
  IndicesSetDestroy(&allVertices);
  
  // Fallback: se não encontrou (não deve acontecer para grafos não vazios)
  if (result == NULL) {
    result = GraphGetSetVertices(g);
  }
  
  return result;
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
  uint16_t range = GraphGetVertexRange(g);
  uint16_t n = GraphGetNumVertices(g);
  
  // Se o grafo não tem vértices
  if (n == 0) {
    return IndicesSetCreateEmpty(range);
  }
  
  // Obter pesos dos vértices
  double* weights = GraphComputeVertexWeights(g);
  
  // Obter todos os vértices do grafo
  IndicesSet* allVertices = GraphGetSetVertices(g);
  
  // Converter para array para facilitar o acesso por índice
  uint16_t vertices[n];
  uint16_t idx = 0;
  uint16_t v = IndicesSetGetFirstElem(allVertices);
  while (v != -1) {
    vertices[idx++] = v;
    v = IndicesSetGetNextElem(allVertices);
  }
  
  IndicesSet* bestSet = NULL;
  double bestWeight = DBL_MAX;
  
  // Busca exaustiva: testar todos os subconjuntos não vazios
  unsigned long totalSubsets = 1UL << n;
  
  for (unsigned long mask = 1; mask < totalSubsets; mask++) {
    // Criar conjunto candidato
    IndicesSet* candidate = IndicesSetCreateEmpty(range);
    double currentWeight = 0.0;
    
    // Calcular peso total do conjunto
    for (uint16_t i = 0; i < n; i++) {
      if (mask & (1UL << i)) {
        uint16_t vertex = vertices[i];
        IndicesSetAdd(candidate, vertex);
        currentWeight += weights[vertex];
      }
    }
    
    // Verificar se é conjunto dominante
    if (GraphIsDominatingSet(g, candidate)) {
      // Se for melhor que o melhor encontrado até agora
      if (currentWeight < bestWeight) {
        bestWeight = currentWeight;
        if (bestSet != NULL) {
          IndicesSetDestroy(&bestSet);
        }
        bestSet = candidate;
      } else {
        IndicesSetDestroy(&candidate);
      }
    } else {
      IndicesSetDestroy(&candidate);
    }
    
    // Otimização: se encontrou um conjunto com peso 0 (impossível com pesos positivos)
    // ou se o peso atual já é maior que o melhor, pode pular alguns casos
    // (implementação básica não tem poda agressiva)
  }
  
  // Libertar memória
  free(weights);
  IndicesSetDestroy(&allVertices);
  
  // Fallback: se não encontrou (não deve acontecer para grafos não vazios)
  if (bestSet == NULL) {
    bestSet = GraphGetSetVertices(g);
  }
  
  return bestSet;
}
