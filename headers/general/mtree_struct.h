#pragma once

#include "cluster_utils.h"


struct Node;

struct Query {
    Point q;
    double r;
};

struct Entry {
    Point p;
    optional<double> cr;
    Node *a;
    bool operator<(const Entry& other) const;
    Entry(const Point& p, optional<double> cr, Node* a) : p(p), cr(cr), a(a) {}
};

const int B = 4096 / sizeof(Entry);
const int b = B / 2;

struct Node {
    vector<Entry> Entries;
    Node() : Entries() {
        Entries.reserve(B);
    }
};


struct MTree {
    Node* root;
    int size; 
    // Construye el arbol dado un vector de puntos (distintos) y un string del metodo de construcción("SS" o "CP")
    void construct(vector <Point> &points, const string method);

    // Genera un archivo con los nodos por nivel
    void printf_mtree(string method);

    
    // Metodo para imprimir el arbol en la terminal. No es util para arboles grandes, dada las limitaciones del tamaño de la terminal
    friend ostream& operator<<(ostream& os, const MTree& mt);
    
    // Inicializa el MTree como vacío
    MTree() : root(nullptr), size(0) {}

    // Destruye el arbol
    void destroy();
};