#include <algorithm>
#include <random>
#include <vector>
#include<iostream>
#include<cmath>
#include <set>
#include<bits/stdc++.h>
#include "../../headers/general/mtree_struct.h"
#include "../../headers/general/mtree_funcs.h" 
#include <ctime>


/******************************************************
 ******************************************************
 ************     FUNCIONES AXUILIARES     ************
 ******************************************************
 ******************************************************/

double dist(Point p, Point q) {
    double dist = pow((p.x-q.x),2) + pow((p.y-q.y),2);
    return dist;
}

// se define por conveniencia una memoria que guarde las distancias para solo tener que calcularlas una vez
// esta es un mapa que usa como llaves los pares de puntos en cuestión, así aseguramos un acceso esficiente 
// y seguro a los datos
typedef map<pair<Point, Point>, double> Cache;

int findClosestSample(const Point& point, const vector<Point>& samples, Cache& cache) {
    int idx = 0;
    double minDistance = DBL_MAX;

    for (int i = 0; i < samples.size(); i++) {
        pair<Point, Point> points = make_pair(point, samples[i]);
        if (cache.find(points) == cache.end()) {
            cache[points] = dist(point, samples[i]);
        }

        double distance = cache[points];
        if (distance < minDistance) {
            minDistance = distance;
            idx = i;
        }
    }

    return idx;
}


// Que carajos es esto rob....
void subTrees(vector<MTree> &trees, vector<Point>& samples, int i) {
    for (const Entry& entry : trees[i].root->Entries) {
        MTree tree;
        Node* root;
        Entry newEntry = {entry.p, nullopt, nullptr};

        root->Entries.push_back(entry);
        tree.root = root;

        samples.push_back(entry.p);
        trees.push_back(tree);

        tree.height = 1;
    }
}

// Que mierda? Porq
vector<MTree> findHeight(Node& node, int height, int h) {
    vector<MTree> res;

    if (height == h) {
        MTree tree;
        tree.root = &node;
        tree.height = height;
        res.push_back(tree);
    } 
    
    else {
        for (const Entry& entry : node.Entries) {
            if (entry.a != nullptr) {
                vector<MTree> sub_res = findHeight(*(entry.a), height - 1, h);
                res.insert(res.end(), sub_res.begin(), sub_res.end());
            }
        }
    }
    return res;
}

void setCR(Node& node, Cache& cache) {
    for (auto& entry : node.Entries) {
        if (entry.a != nullptr) {
            setCR(*(entry.a), cache);

            double maxDist = 0;
            for (const auto& subEntry : entry.a->Entries) {
                double distToSubEntry = cache[{entry.p, subEntry.p}];

                if (subEntry.cr) distToSubEntry += *subEntry.cr;
                if (distToSubEntry > maxDist) maxDist = distToSubEntry;
            }
            entry.cr = maxDist;
        } 
        
        else entry.cr = 0; // nullopt
        
    }
}

/*******************************************************
 *******************************************************
 ***************     CIACCIA PATELLA     ***************
 *******************************************************
 *******************************************************/

MTree ciaccia_patella(Point P[], int n) {
    
    // 1 - si |P| <= B se insertan todos los puntos en un árbol y se retorna
    if (n <= B) {
        MTree T;

        for (int i = 0; i < n; i++) {
            T.root->Entries[i].p = P[i];
            T.root->Entries[i].cr = nullopt;
            T.root->Entries[i].a = nullptr;
        }

        cout << "paso 1 completo\n";
        return T;
    }

    cout << "paso 1 completo\n";

L2:
    // 2 - creación de k samples aleatorios
    int nB = ceil(n / B);
    int k = min(B, nB);
    vector<Point> samples;
    Cache cache;

    for (int i = 0; i < k; i++) {
        samples[i] = P[rand() % n];
    }

    cout << "paso 2 completo\n";

    // 3 - asignar a cada punto su sample más cercano, creando k clusters
    // se utiliza vectores en vez de arreglo estático pues no sabemos qué tamaño puede 
    // tener cada cluster, ya que podría tener desde ningún punto a todos los puntos
    vector<Point> clusters[k];

    // aplicamos la función auxiliar para encontrar el sample más cercano de cada punto
    for (int i = 0; i < n; i++) {
        int idx = findClosestSample(P[i], samples, cache);
        clusters[idx].push_back(P[i]);
    }
    cout << "paso 3 completo\n";

    // 4 - si |F_j| < b redistribuir
    // revisamos los tamaños de los clusters
    for (int i = 0; i < k; i++) {
        if (clusters[i].size() < b) {

            // para cada punto de un cluster de tamaño insuficiente, buscamos ahoro su nuevo sample
            // más cercano, notar que no podemos usar directamente la función auxiliar anterior
            // pues necesitamos ignorar el sample que ya tiene asignado: utilizamos el cache para
            // no tener que recalcular y tener acceso directo a las nuevas distancias candidatas
            for (const Point& p : clusters[i]) {
                int idx;
                double minDistance = DBL_MAX;

                for (int j = 0; j < k; j++) {
                    if (j != i) {
                        pair<Point, Point> points = make_pair(p, samples[i]);
                        double distance = cache[points];
                        if (distance < minDistance) {
                            minDistance = distance;
                            idx = j;
                        }
                    }
                }

                // meter el punto en su nuevo cluster correspondiente
                clusters[idx].push_back(p);
            }

            clusters[i].clear();

            // reajustar los elementos en sus respectivas estruturas así como disminuir en 1 la 
            // cantidad k, ahora debemos retroceder el índice i del ciclo en 1 pues hemos eliminado
            // el anterior elemento i
            samples.erase(samples.begin() + i);
            for (int j = i; j < k - 1; j++) {
                clusters[j] = std::move(clusters[j + 1]);
            }

            k--;
            i--;
        }
    }

    cout << "paso 4 completo\n";

    // 5 - si |F| = 1, volver al paso 2
    // recordar que F es el conjunto de samples, por tanto su tamaño es k. para volver al
    // paso 2 se utiliza una etiqueta
    if (k == 1) goto L2;

    cout << "paso 5 completo\n";

    // 6 - se realiza recursivamente el algoritmo para cada F_j obteniendo el árbol T_j
    // se crea un arreglo de M-trees para almacenar los T_j que en total serán k
    vector<MTree> T_j;

    for (int i = 0; i < k; i++) T_j.push_back(ciaccia_patella(clusters[i].data(), clusters[i].size()));

    cout << "paso 6 completo\n";

    // 7 - si la raíz es de tamaño menor a b, se quita, se eliminan los puntos y se trabaja co
    // los subárboles nuevos y se añaden los puntos a F
    for (int i = 0; i < k; i++) {
        int p = T_j[i].root->Entries.size();

        if (p < b) {
            subTrees(T_j, samples, i); 
            samples.erase(samples.begin() + i);
            T_j.erase(T_j.begin() + i);

            // k += p - 1; NO SÉ SI ESTO VA A NO AHÍ SE VE
            i--;
        }
    }

    cout << "paso 7 completo\n";

    // 8 - se define h como la altura mínima de los T_j y T' vacío
    int h = INT_MAX;
    vector<MTree> T_prime;

    for (int i = 0; i < T_j.size(); i++) {
        int altura = 0;
        Node* node = T_j[i].root;

        while (node != nullptr && node->Entries[0].a != nullptr) {
            altura++;
            node = node->Entries[0].a;
        }
        
        T_j[i].height = altura;
    }

    for (int i = 0; i < k; i++) {
        if (T_j[i].height < h) h = T_j[i].height;
    }

    cout << "paso 8 completo\n";

    // 9 - añadir elementos a T'
    for (int i = 0; i < T_j.size(); i++) {
        if (T_j[i].height == h) {
            T_prime.push_back(T_j[i]);
        }
        else {
            samples.erase(samples.begin() + i);

            vector<MTree> subtrees_h = findHeight(*(T_j[i].root), T_j[i].height, h);
            T_prime.insert(T_prime.end(), subtrees_h.begin(), subtrees_h.end());
            
            for (auto& subtree : subtrees_h) samples.push_back(subtree.root->Entries[0].p);

        }
    }

    cout << "paso 9 completo\n";

    // 10
    MTree T_sup = ciaccia_patella(samples.data(), samples.size());

    cout << "paso 10 completo\n";

    // 11
    for (MTree &tree : T_prime) {
        for (Entry &entry : T_sup.root->Entries) {
            if (entry.a != nullptr && entry.p == tree.root->Entries[0].p) {
                entry.a = tree.root;
                break;
            }
        }
    }
    
    cout << "paso 11 completo\n";

    // 12 - se setean los radios cobertores para cada entrada en el
    setCR(*T_sup.root, cache);

    cout << "paso 12 completo\n";
    
    // 13 :') 
    return T_sup;
}

int main(){
    Point P[10];
    for(int i = 0; i < 10; i++){
        P[i].x = i;
        P[i].y = i;
    }
    MTree T = ciaccia_patella(P, 10);
    return 0;
}