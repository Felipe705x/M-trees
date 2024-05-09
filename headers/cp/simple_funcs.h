#pragma once

#include "../general/mtree_struct.h"

bool compareNodes(Node* &n1, Node* &n2);


int Node_binarySearch(Entry &f_j, vector<Node*> &T_prime, vector<bool> &marked);

// oldCR_squared es el radio cobertor antiguo, al cuadrado. En caso de que no este definido dicho radio, se pone `nullopt`
double calculateCR(vector<Point> points, Point center, optional<double> oldCR_squared);