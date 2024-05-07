#include "../../headers/general/cluster_utils.h"
#include "../../headers/general/mtree_struct.h"

// Unorg. headers
vector<Point> randomSample(int k, const vector <Point> &P, optional<int> seed = nullopt);
int nearestSample(const Point p, const vector<Point> &F);
pair<vector<int>, int> getHeights(const vector<Node*> T_j);
// Obtiene los subtrees de altura h
vector<Node*> getLevelChilds(const Node* T, int tdepth);
vector<Node*> getLeaves(const Node* T);
bool compareNodes(const Node* &n1, const Node* &n2);

bool compareNodes(const Node* &n1, const Node* &n2) {
    return n1->Entries[0].p < n2->Entries[0].p; // Porque las entradas estan ordenadas
}

vector<Point> randomSample(int k, const vector <Point> &P, optional<int> seed = nullopt) {
    // Utilizaremos una tecnica de sampleo aleatorio eficiente (O(n)), conocida como "Reservoir Sampling"
    vector<Point> reservoir(k);
    mt19937 gen;  // random number generator
    if (seed.has_value())
        gen.seed(seed.value());
    else {
        random_device rd;
        gen.seed(rd());  // semilla aleatoria
    }
    for (int i = 0; i < k; i++)
        reservoir[i] = P[i];
    for (int i = k; i < P.size(); i++) {
        uniform_int_distribution<> dis(0,i);
        int j = dis(gen);
        if (j < k)
            reservoir[j] = P[i];
    }
    return reservoir;
}

int nearestSample(const Point p, const vector<Point> &F) {
    int idx = 0;
    double minDist = DBL_MAX;
    for (int i = 0; i < F.size(); i++) {
        double distance = dist(p, F[i]);
        if (distance < minDist) {
            minDist = distance;
            idx = i;
        }
    }
    return 0;
}
// punteros no nulos
pair<vector<int>, int> getHeights(const vector<Node*> T_j) {
    int n = T_j.size();
    int h = INT_MAX;
    vector<int> heights(n);
    for (int i = 0; i < n; i++) {
        int H = 0;
        Node *current_node  = T_j[i]; // El arbol i-esimo
        while (current_node->Entries[0].a != nullptr) { // Las hojas de un M-tree estan a misma altura
            H++;
            current_node = (current_node->Entries[0].a);
        }
        heights[i] = H;
        if (H < h)
            h = H;
    }
    return {heights, h};
}

// Los contenidos de T no se modifican
// Usa BFS
// Tira error si tdepth es una profundidad mayor a la profundidad de T
vector<Node*> getLevelChilds(const Node* T, int tdepth) {
    vector<Node*> levelChilds;
    queue<const Node*> nodesQueue;
    nodesQueue.push(T);
    int cdepth = 0; // current_depth
    while (!nodesQueue.empty() &&  cdepth < tdepth) {
        int levelSize = nodesQueue.size(); // Cantidad de nodos en esta altura
        for (int i = 0; i < levelSize; i++) {
            const Node* cnode = nodesQueue.front(); // current_node
            nodesQueue.pop();
            for (const Entry &entry : cnode->Entries)
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


vector<Node*> getLeaves(const Node* T) {
    vector<Node*> leaves;
    queue<const Node*> nodesQueue;
    nodesQueue.push(T);
    while (!nodesQueue.empty()) {
        int levelSize = nodesQueue.size(); // Cantidad de nodos en esta altura
        for (int i = 0; i < levelSize; i++) {
            const Node* cnode = nodesQueue.front(); // current_node
            nodesQueue.pop();
            if (cnode->Entries[0].a != nullptr) { // Entonces no es una hoja
                for (const Entry &entry : cnode->Entries)
                    nodesQueue.push(entry.a);
            }
            else
                leaves.push_back(const_cast<Node*> (cnode));                
        }
    }
    return leaves;
}

Node *Algorithm_CP(const vector<Point> &P) {
    int n = P.size();
    if (n <= B) {
        Node *T = new Node;
        for (const Point &p : P)
            T->Entries.emplace_back(p, nullopt, nullptr);
        sort(T->Entries.begin(), T->Entries.end()); // Ordenamos las entradas. Esto permite facilidad de busqueda
        return T;
    }
    
    L2:

    int k = min(B, (int)ceil(n/B));
    // Sera nuestro F
    vector<Point> samples = move(randomSample(k, P));
    // Asumamos samples ordenados

    // Etapa de asociación
    // Seran nuestros F_j
    vector<vector<Point>> clusters(k);

    for (const Point &p : P)
        clusters[nearestSample(p, samples)].push_back(p);

    // Etapa de redistribución
    
    vector<vector<Point>>::iterator it = clusters.begin();
    while (it != clusters.end()) {
        if (it->size() < b) {
            // A partir de aca los vectores samples y clusters no coinciden en indice
            int it_idx = it - clusters.begin();
            samples.erase(samples.begin() + it_idx); // Se borra punto del sample
            // Movemos los puntos de este cluster hacia otro
            vector<Point> pointsTmp = move(*it); // Transferimos puntos
            it = clusters.erase(it); // Borramos el cluster asociado al sample y obtenemos el sgte clust
            // Ahora los vectores vuelven a coincidir en indice
            for (const Point &p : pointsTmp)
                clusters[nearestSample(p, samples)].push_back(p);                
        }
        else
            it++;
    }
    if (samples.size() == 1) 
        goto L2;

    vector<Node*> T_j(clusters.size());
    for (int i = 0; i < clusters.size(); i++)
        T_j[i] = Algorithm_CP(clusters[i]);
    

    /// Hasta ahora creo que todo esta bien. Los vectores F, F_j y T_j son biyectivos
    
    // Para almacenar los subarboles al quitar raices
    vector<Entry> rootEntries;
    vector<Node*>::iterator it2 = T_j.begin();
    while (it2!= T_j.end()) {
        if ((*it2)->Entries.size() < b) {
            int it2_idx = it2 - T_j.begin();
            samples.erase(samples.begin() + it2_idx);// Borramos pf_j
            rootEntries.reserve(rootEntries.size() + (*it2)->Entries.size());
            rootEntries.insert(rootEntries.end(), (*it2)->Entries.begin(), (*it2)->Entries.end()); // Antes de borrar la raiz, copiamos sus entradas
            delete *it2; // Liberamos la memoria del nodo raiz
            it2 = T_j.erase(it2); // Borramos raiz y obtenemos siguiente iterador
        }
        else
            it2++;
    }
    // Ahora agregamos los puntos y subarboles
    T_j.reserve(T_j.size() + rootEntries.size());
    samples.reserve(samples.size() + rootEntries.size());
    for (const Entry &entry : rootEntries) {
        samples.push_back(entry.p);
        T_j.push_back(entry.a);
    }
    vector<Entry>().swap(rootEntries); // Liberamos espacio

    // Etapa de balanceamiento (Invariante: F, T_j, heights coinciden en indices)

    pair<vector<int>, int> heights_and_min = move(getHeights(T_j));
    vector<int> heights = move(heights_and_min.first);
    int h = move(heights_and_min.second);
    vector<Node*> T_prime;

    // En este for solo se mantendra que T_j, heights coinciden en indices
    for(int i = 0; i < T_j.size(); i++) {
        if (heights[i] == h)
            T_prime.push_back(move(T_j[i])); // Transferimos el puntero
        else {
            int depth = heights[i] - h;
            vector<Node*> subtrees = getLevelChilds(T_j[i], depth); // de izq a derecha
            T_prime.reserve(T_prime.size() + subtrees.size());
            T_prime.insert(T_prime.end(), subtrees.begin(), subtrees.end());
            // Insertamos los puntos de la raiz en F (i.e. samples)
            samples.reserve(samples.size() + T_j[i]->Entries.size());
            for (const Entry &entry : T_j[i]->Entries)
                samples.push_back(entry.p);
            delete T_j[i]; // Liberamos memoria de la raiz, ya que ya extrajimos los puntos
        }
    }

    // Ahora T_prime y F (samples) coinciden en los indices ?
    vector<Node*>().swap(T_j); //liberamos el espacio de T_j   
    Node *T_sup = Algorithm_CP(samples);
    vector<Node*> leaves = move(getLeaves(T_sup)); // Las hojas de T_sup, ordenadas de izq a derecha
    //sort(samples.begin(), samples.end());
    // Como en esta implementacion las entradas estan ordenadas, podemos unir eficientemente
    sort(T_prime.begin(), T_prime.end(), compareNodes);
    sort(leaves.begin(), leaves.end(), compareNodes);
}