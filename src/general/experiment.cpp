#include "../../headers/general/cluster_utils.h"
#include "../../headers/general/mtree_struct.h"
#include "MSearch.cpp"
#include <iostream>
#include <cstdio>
#include <fstream>

int seed = 42;



vector<Query> randomQueries(pair<double, double> range = make_pair((0.0), (1.0)), std::optional<int> seed = nullopt, double r = 0.02) {
    mt19937 gen;
    if (seed.has_value())
        gen.seed(seed.value());
    else {
        random_device rd;
        gen.seed(rd());
    }
    vector<Query> vector_query(100);
    uniform_real_distribution<> dis(range.first, range.second);
    for (int i=0; i<100; i++)
        vector_query[i] = {{dis(gen), dis(gen)}, r};
    return vector_query;
}


int main() {
    ofstream file("results.csv");
    file << "Method;Query;n;Disk Accesses;Query Time;Construction Time\n";

    vector<Query> queries = randomQueries({0.0, 1.0}, seed);
    
    for (int i = 10; i < 26; i++) {
        unsigned long long n = (1ULL << i);
        vector<Point> points = randomPoints(n, {0.0, 1.0}, seed);

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