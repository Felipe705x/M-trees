#include "../../headers/general/cluster_utils.h"
#include "../../headers/general/mtree_struct.h"
#include <fstream>
#include <chrono>

using std::ofstream;
using std::chrono::duration_cast;
using std::chrono::nanoseconds;
typedef std::chrono::high_resolution_clock Clock;

int seed = 42;

vector<Point> rPoints(const unsigned long long n) {
    vector<Point> vector_point(n);
    mt19937 gen(seed);
    uniform_real_distribution<> dis(0.0, 1.0);
    for (unsigned long long i = 0; i < n; i++)
        vector_point[i] = {dis(gen), dis(gen)};
    return vector_point;
}

vector<Query> rQueries() {
    double r = 0.02;
    vector<Query> vector_query(100);
    mt19937 gen(seed);
    uniform_real_distribution<> dis(0.0, 1.0);
    for (int i = 0; i < 100; i++)
        vector_query[i] = {{dis(gen), dis(gen)}, r};
    return vector_query;
}