#include "../../headers/cp/bfs_utils.h"


// Los contenidos de T no se modifican
// Usa BFS
// Tira error si tdepth es una profundidad mayor a la profundidad de T
vector<Node*> getLevelChilds(Node* T, int tdepth) {
    vector<Node*> levelChilds;
    queue<Node*> nodesQueue;
    nodesQueue.push(T);
    int cdepth = 0; // current_depth
    while (!nodesQueue.empty() &&  cdepth < tdepth) {
        int levelSize = nodesQueue.size(); // Cantidad de nodos en esta altura
        for (int i = 0; i < levelSize; i++) {
            Node* cnode = nodesQueue.front(); // current_node
            nodesQueue.pop();
            for (Entry &entry : cnode->Entries)
                nodesQueue.push(entry.a);
        }
        cdepth++;
    }

    // Los arboles en la cola serán aquellos con profundidad cdepth
    levelChilds.reserve(nodesQueue.size());
    while(!nodesQueue.empty()) {
        levelChilds.push_back(const_cast<Node*>(nodesQueue.front()));
        nodesQueue.pop();
    }
    return levelChilds;
}

// Otorga hojas y la altura del arbol
pair<vector<Node*>, int> getLeaves(Node* T) {
    vector<Node*> leaves;
    queue<Node*> nodesQueue;
    int H = -1;  // Inicialmente consideramos que el arbol esta vacio
    if (T == nullptr)
        return {{},H};
    nodesQueue.push(T);
    while (!nodesQueue.empty()) {
        int levelSize = nodesQueue.size(); // Cantidad de nodos en esta altura
        for (int i = 0; i < levelSize; i++) {
            Node* cnode = nodesQueue.front(); // current_node
            nodesQueue.pop();
            if (cnode->Entries[0].a != nullptr) { // Entonces no es una hoja
                for (Entry &entry : cnode->Entries)
                    nodesQueue.push(entry.a);
            }
            else
                leaves.push_back(const_cast<Node*> (cnode));                
        }
        H++;
    }
    return {leaves,H};
}


/* Version modificada de `getLevelChilds` tal que también incluya los niveles superiores*/
vector<vector<Node*>> getNodesUpToDepth(Node* T, int tdepth) {
    vector<vector<Node*>> nodesByLevel;
    nodesByLevel.reserve(tdepth + 1);
    queue<Node*> nodesQueue;
    nodesQueue.push(T);
    int cdepth = 0; // current_depth

    while (!nodesQueue.empty() && cdepth <= tdepth) {
        int levelSize = nodesQueue.size(); // Number of nodes at this current level
        vector<Node*> currentLevelNodes; // To store nodes of the current level

        for (int i = 0; i < levelSize; i++) {
            Node* cnode = nodesQueue.front();
            nodesQueue.pop();
            currentLevelNodes.push_back(const_cast<Node*>(cnode));

            // Add children to the queue if not yet reached the tdepth
            if (cdepth < tdepth) {
                for (Entry &entry : cnode->Entries) {
                    if (entry.a != nullptr) {
                        nodesQueue.push(entry.a);
                    }
                }
            }
        }

        // Add the current level's nodes to the main list
        nodesByLevel.push_back(currentLevelNodes);
        cdepth++;
    }

    return nodesByLevel;
}