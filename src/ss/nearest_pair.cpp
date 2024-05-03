#include "../../headers/ss/nearest_pair.h"


pair<int,int> nearestPairBrute(const vector <Cluster> &C) {
    double min = DBL_MAX;
    pair<int,int> nearest;
    int n = C.size();
    for (int i = 0; i < n; ++i) {
        for (int j = i+1; j < n; ++j) {
            double distance = dist(C[i], C[j]);
            if (distance < min) {
                min = distance;
                nearest = {i,j};
            }
        }
    }
    
    return nearest;
}
