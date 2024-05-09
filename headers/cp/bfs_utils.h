#pragma once

#include "../general/mtree_struct.h"

vector<Node*> getLevelChilds(Node* T, int tdepth);

pair<vector<Node*>, int> getLeaves(Node* T);

vector<vector<Node*>> getNodesUpToDepth(Node* T, int tdepth);