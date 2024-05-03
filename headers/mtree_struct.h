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
    Node root;
    int height;
};