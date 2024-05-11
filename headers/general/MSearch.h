#ifndef MSEARCH_H
#define MSEARCH_H

#include <vector>
#include "mtree_struct.h"

int MSearch(Node node, Query query, std::vector<Point>& result);

int MSearchTree(MTree tree, Query query);

#endif