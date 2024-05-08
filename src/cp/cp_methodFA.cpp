#include "../../headers/general/cluster_utils.h"
#include "../../headers/general/mtree_struct.h"
#include "../../headers/general/mtree_funcs.h"

// Unorg. headers
vector<Point> randomSample(int k, vector <Point> &P, optional<int> seed = nullopt);
int nearestSample(Point p, vector<Point> &F);
pair<vector<int>, int> getHeights(vector<Node*> T_j);
// Obtiene los subtrees de altura h
vector<Node*> getLevelChilds(Node* T, int tdepth);
pair<vector<Node*>, int> getLeaves(Node* T);
bool compareNodes(Node* &n1, Node* &n2);
// abdi
double calculateCR(vector<Point> points, Point center, optional<double> oldCR_squared);
vector<Point> getOuterPoints(Entry &root_entry, Point &q, double &r);


vector<Point> getOuterPoints(Entry &root_entry, Point &q, double &r) {
    struct StackFrame {
        Entry* entry;
        vector<Point> points;
        int child_index = 0;
    };

    vector<Point> result;
    stack<StackFrame> stack;
    stack.push({&root_entry, {}, 0});

    while (!stack.empty()) {
        auto& top = stack.top();
        Entry* entry = top.entry;

        if (entry->a == nullptr) {
            if (dist(entry->p, q) > r * r) {
                result.push_back(entry->p);
            }
            stack.pop();
        } else if (sqrt(dist(entry->p, q)) > (entry->cr.value() + r)) {
            if (top.child_index < entry->a->Entries.size()) {
                stack.push({&(entry->a->Entries[top.child_index]), {}, 0});
                top.child_index++;
            } else {
                for (auto& pt : top.points) {
                    result.push_back(pt);
                }
                stack.pop();
            }
        } else {
            stack.pop();
        }
    }

    return result;
}


// Retorna el indice del nodo en T_prime en el que se encuentra la entrada cuyo punto es `f_j.p`.
// Se utiliza primero una busqueda binaria para encontrar los nodos candidatos a contener `f_j.p` (retornando prematuramente si se llega a encontrar en esta etapa).
// Luego se hace busqueda binaria dentro de cada nodo candidato hasta encontrar `f_j.p`.
int Node_binarySearch(Entry &f_j, vector<Node*> &T_prime, vector<bool> &marked);


double calculateCR(vector<Point> points, Point center, optional<double> oldCR_squared) {
    double maxRadius;
    if (oldCR_squared.has_value())
        maxRadius = oldCR_squared.value();
    else
        maxRadius = 0.0;
    for (Point &p : points) {
        double distance = dist(p, center);
        if (distance > maxRadius)
            maxRadius = distance;
    }
    return sqrt(maxRadius);
}


int Node_binarySearch(Entry &f_j, vector<Node*> &T_prime, vector<bool> &marked) {
    int n = T_prime.size();
    int idx = -1;
    vector <pair<Node*, int>> nodeCandidates;
    int low = 0, high = n-1;
    
    
    // Buscamos incluir aquellos nodos cuyo rango de puntos pueda contener f_j.p (i.e. Entries[0] <= f_j <= Entries[n-1])
    while (low <= high) {
        int mid = low + (high - low)/2;
        if (T_prime[mid]->Entries[0].p == f_j.p)
            return mid;
        if(T_prime[mid]->Entries[0].p < f_j.p) {
            nodeCandidates.reserve(nodeCandidates.size() + mid - low + 1);
            for (int i = low; i <= mid; i++) {
                if (f_j.p == T_prime[i]->Entries.back().p)
                    return i;
                if((f_j.p < T_prime[i]->Entries.back().p) && !marked[i])
                    nodeCandidates.push_back({T_prime[i], i});
            }
            
            low = mid + 1;
        }
        else
            high = mid - 1;
    }

    // Ahora hacemos busqueda binaria dentro de cada candidato
    for (pair<Node*, int> &candNode : nodeCandidates) {
        low = 0;
        high = candNode.first->Entries.size() - 1;
        while (low <= high) {
            int mid = low + (high - low)/2;
            if (candNode.first->Entries[mid].p == f_j.p)
                return candNode.second;
            if (candNode.first->Entries[mid].p < f_j.p)
                low = mid + 1;
            else
                high = mid - 1;
        }
    }
    return -1; // No se encontro el punto. Esto no deberia pasar, es un error.
}



bool compareNodes(Node* &n1, Node* &n2) {
    return n1->Entries[0].p < n2->Entries[0].p; // Porque las entradas estan ordenadas
}

vector<Point> randomSample(int k, vector <Point> &P, optional<int> seed) {
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

int nearestSample(Point p, vector<Point> &F) {
    int idx = 0;
    double minDist = DBL_MAX;
    for (int i = 0; i < F.size(); i++) {
        double distance = dist(p, F[i]);
        if (distance < minDist) {
            minDist = distance;
            idx = i;
        }
    }
    return idx;
}
// punteros no nulos
pair<vector<int>, int> getHeights(vector<Node*> T_j) {
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


// Otorga hojas y la altura de estas
pair<vector<Node*>, int> getLeaves(Node* T) {
    vector<Node*> leaves;
    queue<Node*> nodesQueue;
    nodesQueue.push(T);
    int H = -1;
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

Node *Algorithm_CP(vector<Point> &P) {
    int n = P.size();
    if (n <= B) {
        Node *T = new Node;
        for (Point &p : P)
            T->Entries.emplace_back(p, nullopt, nullptr);
        sort(T->Entries.begin(), T->Entries.end()); // Ordenamos las entradas. Esto permite facilidad de busqueda
        return T;
    }
    
    L2:
    double nB = static_cast<double>(n)/B;
    cout << nB << endl;
    int k = min(B, (int)ceil(nB));

    
    // Sera nuestro F
    vector<Point> samples = move(randomSample(k, P));
    // Asumamos samples ordenados
    cout << samples.size() << endl;
    // Etapa de asociación
    // Seran nuestros F_j
    vector<vector<Point>> clusters(k);

    for (Point &p : P)
        clusters[nearestSample(p, samples)].push_back(p);

    // Etapa de redistribución
    cout << "redistrib_inic" << endl;
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
            for (Point &p : pointsTmp)
                clusters[nearestSample(p, samples)].push_back(p);                
        }
        else
            it++;
    }
    if (samples.size() == 1) {
        cout << "papito" << endl;
        goto L2;
    }
    cout << "abdi pattern" << endl;

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
    for (Entry &entry : rootEntries) {
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
            // Creo que el problema esta cuando entramos aca
            int depth = heights[i] - h;
            vector<Node*> subtrees = getLevelChilds(T_j[i], depth); // de izq a derecha
            int subtree_sizet = subtrees.size(); // Borrar
            T_prime.reserve(T_prime.size() + subtrees.size());
            T_prime.insert(T_prime.end(), subtrees.begin(), subtrees.end());
            // Insertamos los puntos de la raiz en F (i.e. samples)
            samples.reserve(samples.size() + T_j[i]->Entries.size());
            int cnt = 0;
            for (Entry &entry : T_j[i]->Entries) {
                samples.push_back(entry.p);
                cnt++;
            }
            assert(cnt == subtree_sizet && "ERROR GRAVE");
            delete T_j[i]; // Liberamos memoria de la raiz, ya que ya extrajimos los puntos
        }
    }

    std::sort(samples.begin(), samples.end());

    // Remove consecutive duplicates
    auto last = std::unique(samples.begin(), samples.end());
    samples.erase(last, samples.end());
    


    
    vector<Node*>().swap(T_j); //liberamos el espacio de T_j   
    Node *T_sup = Algorithm_CP(samples);
    pair<vector<Node*>, int> leaves_and_height = move(getLeaves(T_sup));
    vector<Node*> leaves = move(leaves_and_height.first);
    int leaves_height = move(leaves_and_height.second);
    

    // a bit of testing
    int abdi = 0;
    for (Node* &H_j : leaves) {  // Por cada hoja
        for (Entry &f_j : H_j->Entries)
            abdi++;
    }
    cout << abdi << "vs." << T_prime.size() << endl;
    assert(abdi == samples.size()&& "otro error");
    assert(abdi == T_prime.size() && "XDDDDDDDDDD");  // ESTE ERROR QLO

    // Adjuntación de raices de T' a hojas de T_sup. Como las entradas estan ordenadas, podemos realizar esto eficientemente

    sort(T_prime.begin(), T_prime.end(), compareNodes);  // O(|F|log|F|), ordenamos segun puntos de las primeras entradas
    vector<bool> marked(T_prime.size(), false);
    for (Node* &H_j : leaves) {  // Por cada hoja
        for (Entry &f_j : H_j->Entries) { // Por cada entrada dentro de dicha hoja
            int correspondingRoot_idx =  Node_binarySearch(f_j, T_prime, marked);
            assert(correspondingRoot_idx != -1 && "Node_binarySearch failed to find a matching node, which is unexpected.");
            f_j.a = T_prime[correspondingRoot_idx];
            marked[correspondingRoot_idx] = true;
        } 
    }
    // liberamos espacio
    vector<Node*>().swap(T_prime); 
    vector<bool>().swap(marked); 

    // Seteamos radios cobertores de las hojas de Tsup
    for(Node* &H_j : leaves) {
        for (Entry &f_j : H_j->Entries) {
            vector<Node *> new_leaves = move(getLeaves(f_j.a)).first;
            vector<Point> pts;
            pts.reserve(B*new_leaves.size());
            for (Node* &nH_j : new_leaves) {
                for (Entry &nf_j : nH_j->Entries)
                    pts.push_back(nf_j.p);
            }
            f_j.cr = calculateCR(pts, f_j.p, nullopt);
        }
    }
    vector<vector<Node*>> levels;
    if (leaves_height > 0)
        levels = getNodesUpToDepth(T_sup, leaves_height-1);
    int level_size = levels.size() - 1;
    for (int i = level_size; i > 0; i--) {
        vector<Node*> clevel = levels[i];
        for (Node* &N_j : clevel) { // Por cada nodo en este nivel
            for (Entry &entry : N_j->Entries) {
                vector<Point> outer_points = move(getOuterPoints(entry, entry.p, entry.cr.value()));
                entry.cr = calculateCR(outer_points, entry.p, entry.cr.value()*entry.cr.value());
            }
        }
    }
    return T_sup;

}

int main() {
    clock_t start = clock();
    int n = pow(2, 20);
    cout << "generating random points ..." << endl;
    vector<Point> points = randomPoints(n);
    cout << "cp" << endl;
    MTree mt;
    mt.construct(points, "CP");
    cout << endl;
   // cout << mt << endl;
    clock_t end = clock();
    double seconds = static_cast<double>(end - start) / CLOCKS_PER_SEC;
    cout << endl;
    cout << "Time taken papito: " << seconds << " seconds"  << endl;
    return 0;
}


// Sospecho error en paso 7 o 9