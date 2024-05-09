#include "../../headers/general/cluster_utils.h"
#include "../../headers/general/mtree_struct.h"
#include "MSearch.cpp"
#include <iostream>
#include <cstdio>
#include <fstream>

int seed = 42;

vector<Point> rPoints(const ull n) {
    vector<Point> vector_point(n);
    mt19937 gen(seed);
    uniform_real_distribution<> dis(0.0, 1);
    for (ull i=0; i<n; i++)
        vector_point[i] = {dis(gen), dis(gen)};
    return vector_point;
}

vector<Query> rQueries() {
    double r = 0.02;
    vector<Query> vector_query(100);
    mt19937 gen(seed);
    uniform_real_distribution<> dis(0.0, 1);
    for (int i=0; i<100; i++)
        vector_query[i] = {{dis(gen), dis(gen)}, r};
    return vector_query;
}

int main() {
    ofstream file("results.csv");
    file << "Method;Query;n;Disk Accesses;Query Time;Construction Time\n";

    vector<Query> queries = rQueries();

    for (int i = 10; i < 26; i++) {
        unsigned long long n = (1ULL << i);
        vector<Point> points = rPoints(n);

        MTree tree_CP;
        MTree tree_SS;

        clock_t start_CP = clock();
        tree_CP.construct(points, "CP");
        clock_t end_CP = clock();
        double constructTime_CP = static_cast<double>(end_CP - start_CP) / CLOCKS_PER_SEC;

        clock_t start_SS = clock();
        tree_SS.construct(points, "SS");
        clock_t end_SS = clock();
        double constructTime_SS = static_cast<double>(end_SS - start_SS) / CLOCKS_PER_SEC;

        for (int j = 0; j < 100; j++) {
            Query query = queries[j];

            clock_t queryStart = clock();
            int result_CP = MSearch(tree_CP, query);
            clock_t queryEnd = clock();
            double queryTime_CP = static_cast<double>(queryEnd - queryStart) / CLOCKS_PER_SEC;

            queryStart = clock();
            int result_SS = MSearch(tree_SS, query);
            queryEnd = clock();
            double queryTime_SS = static_cast<double>(queryEnd - queryStart) / CLOCKS_PER_SEC;

            file << "CP;" << j << ";" << i << ";" << result_CP << ";" << queryTime_CP << ";" << constructTime_CP << "\n";
            file << "SS;" << j << ";" << i << ";" << result_SS << ";" << queryTime_SS << ";" << constructTime_SS << "\n";
        }
    }

    file.close();
    return 0;
}