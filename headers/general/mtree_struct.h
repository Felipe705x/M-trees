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

    // Valor: -2 si no se ha calculado la altura, -1 si el arbol esta vacío
    int height;
    
    // Construye el arbol dado un vector de puntos (distintos) y un string del metodo de construcción("SS" o "CP")
    void construct(const vector <Point> &points, const string method);

    friend ostream& operator<<(ostream& os, const MTree& mt);

    // Inicializa el MTree como vacío
    MTree() : root(nullptr), height(-2) {}
};