#include "../headers/nearest_pair.h"
#include "../headers/mtree_struct.h"
#include "../headers/nearest_neighbour.h"
#include "../headers/minmax_split.h"
#include "../headers/mtree_funcs.h"
#include <ctime>


vector<Cluster> Cluster_SS(const vector<Point> &C_in) {

    cout << "memory allocaction ..." << endl;
    vector<Cluster> C;
    C.reserve(C_in.size());
    vector<Cluster> C_out;
    C_out.reserve(C_in.size()/b);  // En el peor caso, C_out tendra n/b clusters de tamaño b cada uno

    cout << "filling C with {p} clusters ..." << endl;
    for (const Point &point : C_in)
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

        if ((c1.points.size() + c2.points.size()) <= B) { // equiv. |c1 U c2|<= B (todos los puntos son distintos, dado distribución uniforme)
            if (i1 > i2) swap(i1,i2); // Necesitamos borrar primero el indice más grande (para que los indices sigan siendo validos)
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
    C_out.shrink_to_fit(); // Para reducir espacio no utilizado. O(1) per no garantiza reducción de tamaño
    return C_out;
}

Entry OutputLeaf(const Cluster &C_in) {
    Node *a = new Node; // Guardar C (apuntado por a) en heap
    double r = 0;
    for (const Point p : C_in.points) {
        a->Entries.emplace_back(p, nullopt, nullptr);
        r = max(r, sqrt(dist(C_in.medoid, p)));
    }
    if (a->Entries.size() < B)
        vector<Entry>(a->Entries.begin(), a->Entries.end()).swap(a->Entries); // Libera espacio no utilizado
    sort(a->Entries.begin(), a->Entries.end()); // Opcional, permite busquedas binarias en cada nodo
    return {C_in.medoid, r, a};
}

Entry OutputLeaf(const vector<Point> &C_in) {
    Node *a = new Node; // Guardar C (apuntado por a) en heap
    Point g = calculateMedoid(C_in);
    double r = 0;
    for (const Point p : C_in) {
        a->Entries.emplace_back(p, nullopt, nullptr);
        r = max(r, sqrt(dist(g, p)));
    }
    if (a->Entries.size() < B)
        vector<Entry>(a->Entries.begin(), a->Entries.end()).swap(a->Entries); // Libera espacio no utilizado
    sort(a->Entries.begin(), a->Entries.end());  // Opcional, permite busquedas binarias en cada nodo
    return {g, r, a};
}


Entry OutputInternal(const vector<Entry> &C_mra) {
    Node *A = new Node;  // Guardar C (apuntado por A) en heap
    double R = 0;
    int n = C_mra.size();
    vector<Point> C_in(n);
    for (int i=0; i<n; i++)
        C_in[i] = C_mra[i].p;
    Point G = calculateMedoid(C_in);
    for (const Entry entry : C_mra) {
        A->Entries.push_back(entry);
        R = max(R, sqrt(dist(G, entry.p)) + entry.cr.value());
    }
    if (A->Entries.size() < B)
        vector<Entry>(A->Entries.begin(), A->Entries.end()).swap(A->Entries); // Libera espacio no utilizado
    sort(A->Entries.begin(), A->Entries.end()); // Opcional, permite busquedas binarias en cada nodo
    return {G, R, A};
}

// Retorna las entradas {(g,r,a)|(g,r,a) in C.Entries ^ g in c.points}
// Utilizando que el nodo C tiene las entradas ordenadas:
// El algoritmo es O(n + m + mlogm) donde n y m son los tamaños de C y c, respectivamente
vector<Entry> intersectEntry(const Node &C, Cluster &c) {
    vector<Entry> s;
    sort(c.points.begin(), c.points.end()); //  O(mlogm)
    int n = C.Entries.size(), m = c.points.size();
    s.reserve(min(n,m)); // La intersección tendrá a lo más min(n,m) elementos
    int i = 0, j = 0; // Indices para recorrer ambos vectores  (C.Entries y c.points, respectivamente)
    
    while (i < n && j < m) {
        if (C.Entries[i].p == c.points[j]) {
            s.push_back(C.Entries[i]);
            i++;
            j++;
        }
        else if (C.Entries[i].p < c.points[j])
            i++;
        else
            j++;
    }
    
    s.shrink_to_fit(); // O(1)
    return s;  // Notar que s estará ordenado
}

Node *Algorithm_SS(vector<Point> C_in) {
    if (C_in.size() <= B)
        return OutputLeaf(C_in).a;
    cout << "Greater than B points..." << endl;
    vector<Cluster> C_out = move(Cluster_SS(C_in));
    Node C;
    for (const Cluster &c : C_out)
        C.Entries.emplace_back(OutputLeaf(c));
    C_in.clear();   // C_in = {}
    while (C.Entries.size() > B) {
        for (const Entry &entry : C.Entries)
            C_in.push_back(entry.p);
        C_out = move(Cluster_SS(C_in));
        vector<vector<Entry>> C_mra;
        C_mra.reserve(C_out.size());
        for (Cluster &c : C_out)
            C_mra.emplace_back(intersectEntry(C, c));
        C.Entries.clear();
        for (const vector<Entry> &s : C_mra)
            C.Entries.push_back(OutputInternal(s));
    }
    return OutputInternal(C.Entries).a;
}

int main() {
    clock_t start = clock();
    int n = B+1;
    cout << "generating random points ..." << endl;
    const vector<Point> points = randomPoints(n);
    cout << "Outputleaf process initiated!" << endl;
    MTree mt;
    mt.construct(points, "SS");
    cout << endl;
    cout << mt << endl;
    clock_t end = clock();
    double seconds = static_cast<double>(end - start) / CLOCKS_PER_SEC;
    cout << endl;
    cout << "Time taken: " << seconds << " seconds"  << endl;
    return 0;
}