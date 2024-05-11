#include "../../headers/general/cluster_utils.h"
#include "../../headers/general/mtree_struct.h"

// Función recursiva de búsqueda en M-Tree
int MSearch(Node node, Query query, vector<Point>& result) {
    int counter = 0;
    vector<Entry> entries = node.Entries;  // Acceso a nodo
    counter++;   // Es un acceso a disco
    for (int i = 0; i < entries.size(); i++) {
        if (entries[i].a == nullptr) {
            // Nodo hoja
            if (dist(entries[i].p, query.q) <= query.r) {
                result.push_back(entries[i].p);
            }
        }
        else {
            // Nodo interno
            if(dist(entries[i].p, query.q) <= (entries[i].cr.value() + query.r)) {
                // Buscar nodo hijo recursivamente
                counter += MSearch(*(entries[i].a), query, result);
            }
        }
    }
    return counter;
}

int MSearch(MTree tree, Query query) {
    vector<Point> result;
    int res = MSearch(*tree.root, query, result);
    return res;
}