#include "../../headers/general/cluster_utils.h"
#include "../../headers/general/mtree_struct.h"
#include "../../headers/general/MSearch.h"
#include "../../headers/general/experiment.h"
#include <fstream>
#include <chrono>

using std::ofstream;
using std::chrono::duration_cast;
using std::chrono::nanoseconds;
typedef std::chrono::high_resolution_clock Clock;


// Archivo main que utiliza las funciones definidas para el experimento para correr todas las iteraciones de este
// guarda los resultados en results2.csv en un formato conveniente. se conservarn los resultados originales en results.csv
int main() {
    ofstream file("results2.csv");
    file << "Method;Query;n;Disk Accesses;Query Time;Construction Time\n";

    vector<Query> queries = rQueries();

    for (int i = 10; i < 26; i++) {
        unsigned long long n = (1ULL << i);
        vector<Point> points = rPoints(n);

        MTree tree_CP;
        MTree tree_SS;
        
        auto start_CP = Clock::now();
        tree_CP.construct(points, "CP");
        auto end_CP = Clock::now();
        auto constructTime_CP = duration_cast<nanoseconds>(end_CP - start_CP).count();

        for (int j = 0; j < 100; j++) {
            Query query = queries[j];

            auto queryStart = Clock::now();
            int result_CP = MSearchTree(tree_CP, query);
            auto queryEnd = Clock::now();
            auto queryTime_CP = duration_cast<nanoseconds>(queryEnd - queryStart).count();

            file << "CP;" << j << ";" << i << ";" << result_CP << ";" << queryTime_CP << ";" << constructTime_CP << "\n";
        }

        tree_CP.destroy();
        
        
        auto start_SS = Clock::now();
        tree_SS.construct(points, "SS");
        auto end_SS = Clock::now();
        auto constructTime_SS = duration_cast<nanoseconds>(end_SS - start_SS).count();

        for (int j = 0; j < 100; j++) {
            Query query = queries[j];

            auto queryStart = Clock::now();
            int result_SS = MSearchTree(tree_SS, query);
            auto queryEnd = Clock::now();
            auto queryTime_SS = duration_cast<nanoseconds>(queryEnd - queryStart).count();

            file << "SS;" << j << ";" << i << ";" << result_SS << ";" << queryTime_SS << ";" << constructTime_SS << "\n";
        }

        tree_SS.destroy();
        cout << "ciclo " << i << " terminado" << endl;
    }

    file.close();
    return 0;
}