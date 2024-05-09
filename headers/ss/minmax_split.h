#pragma once

#include "../general/cluster_utils.h"

/**
 * Calcula el radio cobertor de un cluster dado, considerando su medoide como referencia central
 * 
 * @param cluster cluster al cual se le calcula el radio cobertor
 * 
 * @return radio cobertor como una distancia eucleidiana, elevada al cuadrado
*/
double coveringRadius(const Cluster &cluster);


pair<Cluster, Cluster> minMaxSplit(const Cluster &c);
