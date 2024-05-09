#pragma once

#include "../general/cluster_utils.h"

/**
 * Encuentra el cluster más cercano a `c1` dentro de un conjunto de clusters. La complejidad temporal es O(n)
 * 
 * @param c1 cluster al cual se le quiere encontrar su vecino más cercano
 * @param clusters vector del conjunto de clusters en el cual se busca el vecino más cercano
 * 
 * @return índice del vecino más cercano en `clusters`
 */
int nearestNeighbour(Cluster c1, vector<Cluster> clusters);