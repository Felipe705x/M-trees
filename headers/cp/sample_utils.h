#pragma once


#include "../general/cluster_utils.h"


/**
 * Sampling (sin repetición) basado en el método "Reservoir Sampling"
 * 
 * La complejidad es O(n) y cada elemento de la población tiene la misma probabilidad de ser incluidos en el sample final
*/
vector<Point> randomSample(int k, vector <Point> &P, optional<int> seed = nullopt);

/* Retorna indice del sample mas cercano*/
int nearestSample(Point p, vector<Point> &F);