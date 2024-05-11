#include "../../headers/cp/simple_funcs.h"

bool compareNodes(Node* &n1, Node* &n2) {
    return n1->Entries[0].p < n2->Entries[0].p; // Porque las entradas estan ordenadas
}




// Retorna el indice del nodo en T_prime en el que se encuentra la entrada cuyo punto es `f_j.p`.
// Se utiliza primero una busqueda binaria para encontrar los nodos candidatos a contener `f_j.p` (retornando prematuramente si se llega a encontrar en esta etapa).
// Luego se hace busqueda binaria dentro de cada nodo candidato hasta encontrar `f_j.p`.
int Node_binarySearch(Entry &f_j, vector<Node*> &T_prime, vector<bool> &marked) {
    int n = T_prime.size();
    int idx = -1;
    vector <pair<Node*, int>> nodeCandidates;
    int low = 0, high = n-1;
    
    
    // Buscamos incluir aquellos nodos cuyo rango de puntos pueda contener f_j.p (i.e. Entries[0] <= f_j <= Entries[n-1])
    while (low <= high) {
        int mid = low + (high - low)/2;
        if (T_prime[mid]->Entries[0].p == f_j.p)
            return mid;
        if(T_prime[mid]->Entries[0].p < f_j.p) {
            nodeCandidates.reserve(nodeCandidates.size() + mid - low + 1);
            for (int i = low; i <= mid; i++) {
                if (f_j.p == T_prime[i]->Entries.back().p)
                    return i;
                if((f_j.p < T_prime[i]->Entries.back().p) && !marked[i])
                    nodeCandidates.push_back({T_prime[i], i});
            }
            
            low = mid + 1;
        }
        else
            high = mid - 1;
    }

    // ahora hacemos busqueda binaria dentro de cada candidato
    for (pair<Node*, int> &candNode : nodeCandidates) {
        low = 0;
        high = candNode.first->Entries.size() - 1;
        while (low <= high) {
            int mid = low + (high - low)/2;
            if (candNode.first->Entries[mid].p == f_j.p)
                return candNode.second;
            if (candNode.first->Entries[mid].p < f_j.p)
                low = mid + 1;
            else
                high = mid - 1;
        }
    }
    return -1; // No se encontro el punto. Esto no deberia pasar, es un error.
}

double calculateCR(vector<Point> points, Point center, optional<double> oldCR) {
    double maxRadius;
    if (oldCR.has_value())
        maxRadius = oldCR.value();
    else
        maxRadius = 0.0;
    for (Point &p : points) {
        double distance = dist(p, center);
        if (distance > maxRadius)
            maxRadius = distance;
    }
    return maxRadius;
}