#pragma once

#include "cluster_utils.h"


struct Node;

struct Query {
    Point q;
    double r;
};

struct Entry {
    Point p;
    double cr;
    Node *a;
};

const int B = 4096 / sizeof(Entry);
const int b = B / 2;

struct Node {
    Entry L[B];
};


struct MTree {
    Node root;
};