#include "../headers/nearest_pair.h"
#include "../headers/mtree_struct.h"
#include "../headers/nearest_neighbour.h"
#include "../headers/minmax_split.h"
#include <ctime>


vector<Cluster> Cluster_SS(const vector<Point> &points) {

    cout << "memory allocaction ..." << endl;
    vector<Cluster> C;
    C.reserve(C.size());
    vector<Cluster> C_out;
    C_out.reserve(B);

    cout << "filling C with {p} clusters ..." << endl;
    for (const Point &point : points)
        C.push_back(clusterize(point));

    cout << "while |C| > 1 loop ..." << endl;
    while (C.size() > 1) {
        pair<int, int> closest_index;
        // Primary bottleneck: Implemented as O(n^2), a O(nlogn) algorithm is known but has memory issues on this machine.
        closest_index = nearestPairBrute(C);
        Cluster c1, c2;
        int i1, i2;

        if (C[closest_index.first].points.size() > C[closest_index.second].points.size())
            i1 = closest_index.first, i2 = closest_index.second;
        else 
            i1 = closest_index.second, i2 = closest_index.first;
        c1 = move(C[i1]), c2 = C[i2];   // c1 siempre se saca de C, lo movemos

        if ((c1.points.size() + c2.points.size()) <= B) {
            if (i1 > i2) swap(i1,i2); // Necesitamos borrar primero el indice más grande (para que los indices sigan siendo validos)no i
            C.erase(C.begin() + i2);
            C.erase(C.begin() + i1);
            C.push_back(clustUnion(c1, c2));
        } 
        else {
            C.erase(C.begin() + i1); // Eliminamos c1 de C
            C_out.push_back(move(c1));
        }
    }

    Cluster c = move(C[0]); // El ultimo cluster
    C.pop_back();

    cout << "if |C_out| > 0 ..." << endl;
    Cluster c_neighbour;
    bool isDefined = true;
    if (C_out.size() > 0) {
        int nearest_neigbour_index = nearestNeighbour(c, C_out); // O(n)
        c_neighbour = move(C_out[nearest_neigbour_index]);
        C_out.erase(C_out.begin() + nearest_neigbour_index);
    }
    else
        isDefined = false;


    if (isDefined)  // Si c_neigbour es no vacio
        c = clustUnion(c, c_neighbour);

    if (c.points.size() <= B)
        C_out.push_back(c);
    else {
        cout << "MinMax Split in progress ..." << endl;
        pair<Cluster, Cluster> split_clust = minMaxSplit(c);
        C_out.push_back(split_clust.first);
        C_out.push_back(split_clust.second);
    }
    C_out.shrink_to_fit(); // Para reducir espacio no utilizado. O(B) = O(1)
    return C_out;
}

Entry OutputLeaf(const Cluster &C_in) {
    Node *a = new Node; // Guardar C (apuntado por a) en heap
    double r = 0;
    for (const Point &p : C_in.points) {
        a->Entries.emplace_back(p, nullopt, nullptr);
        r = max(r, sqrt(dist(C_in.medoid, p)));
    }
    if (a->Entries.size() < B)
        vector<Entry>(a->Entries.begin(), a->Entries.end()).swap(a->Entries); // Libera espacio no utilizado

    return {C_in.medoid, r , a};
}

Entry OutputInternal(const vector<Entry> &C_mra) {
    Node *A = new Node;  // Guardar C (apuntado por A) en heap
    double R = 0;
    int n = C_mra.size();
    vector<Point> C_in(n);
    for (int i=0; i<n; i++)
        C_in[i] = C_mra[i].p;
    Point G = calculateMedoid(C_in);
    for (const Entry &entry : C_mra) {
        A->Entries.push_back(entry);
        R = max(R, sqrt(dist(G, entry.p)) + entry.cr.value());
    }
    if (A->Entries.size() < B)
        vector<Entry>(A->Entries.begin(), A->Entries.end()).swap(A->Entries); // Libera espacio no utilizado

    return {G, R, A};
}




int main() {
    clock_t start = clock();
    int n = pow(2,5);
    cout << "generating random points ..." << endl;
    const vector<Point> points = randomPoints(n);
    cout << "Outputleaf process initiated!" << endl;
    Entry entry = OutputLeaf(clusterize(points));
    cout << "Medoid is: " << entry.p << endl;
    cout << "r is: " << entry.cr.value() << endl;
    Node* node = entry.a;
    cout << "Node size should satisfy: " << b << "<= size:" << node->Entries.size() << "<=" << B << endl;
    for (const Entry &e : node->Entries) {
        if ((e.a == NULL) && (e.cr == nullopt))
            cout << e.p;
        else
            cout << "ERROR";
    }
    clock_t end = clock();
    double seconds = static_cast<double>(end - start) / CLOCKS_PER_SEC;
    cout << endl;
    cout << "Time taken: " << seconds << " seconds"  << endl;
    return 0;
}