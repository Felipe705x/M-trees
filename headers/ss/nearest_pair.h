#pragma once

#include "../general/cluster_utils.h"

/**
 * Encuentra el par de clusters más cercanos dentro de un conjunto de clusters. \n
 * La complejidad temporal del algoritmo es O(nlogn)
 * 
 * @param C vector del conjunto de cluster
 * 
 * @return par de indices correspondientes al par mas cercano encontrado en el conjunto
*/
pair <int,int> nearestPair(const vector<Cluster> &C);


/**
 * Encuentra el par de clusters más cercanos dentro de un conjunto de clusters. \n
 * La complejidad temporal del algoritmo es O(n^2)
 * 
 * @param C vector del conjunto de cluster
 * 
 * @return par de indices correspondientes al par mas cercano encontrado en el conjunto
 */
pair <int,int> nearestPairBrute(const vector<Cluster> &C);