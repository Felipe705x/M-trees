#include "../headers/nearest_neighbour.h"



int nearestNeighbour(Cluster c1, vector<Cluster> clusters)  {
    int nearest;
    double minDistance = DBL_MAX;  // Initialize with the largest possible double value
    
    for(int i = 0; i < clusters.size(); i++) {
        double distance = dist(c1, clusters[i]);
        if (distance < minDistance) {
            minDistance = distance;
            nearest = i;
        }
    }
    // tambien retorno cualquier nearest
    return nearest;
}
