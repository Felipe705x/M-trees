#include "M-tree.h"

// Función recursiva de búsqueda en M-Tree
void MSearch(Node node, Query query, vector<Point>& result) {
    for (int i = 0; i < B; i++) {
        if (node.L[i].a == nullptr) {
            // Nodo hoja
            if (dist(node.L[i].p, query.q) <= query.r * query.r) {
                result.push_back(node.L[i].p);
            }
        }
        else {
            // Nodo interno
            if(dist(node.L[i].p, query.q) <= (node.L[i].cr + query.r) * (node.L[i].cr + query.r)) {
                // Buscar nodo hijo recursivamente
                MSearch(*(node.L[i].a), query, result);
            }
        }
    }
}

//Función búsqueda en M-Tree, entrega vector de puntos
vector<Point> MSearch(MTree tree, Query query) {
    vector<Point> result;
    MSearch(tree.root, query, result);
    return result;
}