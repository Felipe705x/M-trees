#include "../../headers/cp/sample_utils.h"

vector<Point> randomSample(int k, vector <Point> &P, optional<int> seed) {
    // Utilizaremos una tecnica de sampleo aleatorio eficiente (O(n)), conocida como "Reservoir Sampling"
    vector<Point> reservoir(k);
    mt19937 gen;  // random number generator
    if (seed.has_value())
        gen.seed(seed.value());
    else {
        random_device rd;
        gen.seed(rd());  // semilla aleatoria
    }
    for (int i = 0; i < k; i++)
        reservoir[i] = P[i];
    for (int i = k; i < P.size(); i++) {
        uniform_int_distribution<> dis(0,i);
        int j = dis(gen);
        if (j < k)
            reservoir[j] = P[i];
    }
    return reservoir;
}


int nearestSample(Point p, vector<Point> &F) {
    int idx = 0;
    double minDist = DBL_MAX;
    for (int i = 0; i < F.size(); i++) {
        double distance = dist(p, F[i]);
        if (distance < minDist) {
            minDist = distance;
            idx = i;
        }
    }
    return idx;
}