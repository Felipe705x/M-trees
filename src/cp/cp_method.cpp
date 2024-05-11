#include "../../headers/cp/bfs_utils.h"
#include "../../headers/cp/dfs_utils.h"
#include "../../headers/cp/sample_utils.h"
#include "../../headers/cp/simple_funcs.h"


Node *Algorithm_CP(vector<Point> &P) {

    int n = P.size();

    /***************************
    * Paso 1
    ***************************/

    if (n <= B) {
        Node *T = new Node;
        for (Point &p : P)
            T->Entries.emplace_back(p, nullopt, nullptr);
        std::sort(T->Entries.begin(), T->Entries.end()); // Ordenamos las entradas. Esto permite facilidad de busqueda en el futuro (y se ve bonito)
        return T;
    }
    
    L2:

    /***************************
    * Paso 2
    ***************************/

    double nB = static_cast<double>(n)/B;
    int k = min(B, (int)ceil(nB));

    
    // Sera nuestro F
    vector<Point> samples = move(randomSample(k, P));


    /***************************
    * Paso 3
    ***************************/

    // Seran nuestros F_j
    vector<vector<Point>> clusters(k);
    for (Point &p : P)
        clusters[nearestSample(p, samples)].push_back(p);


    /***************************
    * Paso 4 (Redistribución)
    ***************************/

    vector<vector<Point>>::iterator it = clusters.begin();
    while (it != clusters.end()) {
        if (it->size() < b) {
            int it_idx = it - clusters.begin();

            /* Paso 4.1 */
            samples.erase(samples.begin() + it_idx); // Se borra punto del sample

            vector<Point> pointsTmp = move(*it); // Transferimos puntos
            it = clusters.erase(it); // Borramos el cluster asociado al sample y obtenemos el sgte clust

            /* Paso 4.2 */
            for (Point &p : pointsTmp)
                clusters[nearestSample(p, samples)].push_back(p);                
        }
        else
            it++;
    }

    /***************************
    * Paso 5
    ***************************/
    if (samples.size() == 1)
        goto L2;


    /***************************
    * Paso 6
    ***************************/
    vector<Node*> T_j(clusters.size());
    for (int i = 0; i < clusters.size(); i++)
        T_j[i] = Algorithm_CP(clusters[i]);
    
    
    
    /***************************
    * Paso 7 (Chequeo de Underfill)
    ***************************/

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

    /***************************
    * Paso 8 (Balanceamiento)
    ***************************/

    pair<vector<int>, int> heights_and_min = move(getHeights(T_j));
    vector<int> heights = move(heights_and_min.first);
    int h = move(heights_and_min.second);
    vector<Node*> T_prime;


    /***************************
    * Paso 9
    ***************************/

    // Antes nuestro conjunto F era `samples`. Ahora sera la variable `F`. En esta se agregan los puntos de `samples` que son validos segun la etapa de balanceamiento.
    // Esto permite hacer inserciones en O(1) a `F` en lugar de hacer borrados O(n) en `samples`
    vector<Point> F; 

    for(int i = 0; i < T_j.size(); i++) {
        if (heights[i] == h) {
            T_prime.push_back(move(T_j[i])); // Transferimos el puntero
            F.push_back(samples[i]);        // El punto es valido, lo agregamos a F
        }
        else {
            /* Paso 9.1-análogo */
            // El punto en T_j no es valido, no lo agregamos a F

            /* Paso 9.2 */
            int depth = heights[i] - h;
            vector<vector<Node*>> aboveSubtrees  = getNodesUpToDepth(T_j[i], depth-1); // Obtenemos los niveles anteriores a los subarboles para poder liberar memoria (evitar mleaks)
            
            /* Paso 9.3 */
            for (Node* &st_root : aboveSubtrees[depth-1]) {  // Iteramos sobre los nodos de altura h+1
                F.reserve(F.size() + st_root->Entries.size());
                T_prime.reserve(T_prime.size() + st_root->Entries.size());
                for (Entry &entry : st_root->Entries) {
                    F.push_back(entry.p);                   // Agregamos los puntos pertinentes a F
                    T_prime.push_back(entry.a);             // Agregamos los hijos de los nodos de altura h+1, que necesariamente seran los subarboles de altura h
                }
            }

            // Liberamos la memoria asociada a los nodos padre de los subarboles de altura h encontrados
            for (vector<Node*> levels : aboveSubtrees) {
                for (Node* node : levels)
                    delete node;
            }
        }
    }

    vector<Node*>().swap(T_j); //liberamos el espacio de T_j
    vector<Point>().swap(samples);  //liberamos el espacio de samples (ya que tenemos a la variable `F` que es definitiva)

    
    /***************************
    * Paso 10
    ***************************/

    Node *T_sup = Algorithm_CP(F);

    pair<vector<Node*>, int> leaves_and_height = move(getLeaves(T_sup));
    vector<Node*> leaves = move(leaves_and_height.first);
    int leaves_height = move(leaves_and_height.second);
    
    /***************************
    * Paso 11
    ***************************/

    // Adjuntación de raices de T' a hojas de T_sup. Como las entradas estan ordenadas, podemos realizar esto eficientemente

    std::sort(T_prime.begin(), T_prime.end(), compareNodes);  // O(|F|log|F|), ordenamos segun puntos de las primeras entradas
    vector<bool> marked(T_prime.size(), false); // marca que raices ya fueron adjuntadas a las hojas
    for (Node* &H_j : leaves) {  // Por cada hoja
        for (Entry &f_j : H_j->Entries) { // Por cada entrada dentro de dicha hoja
            int correspondingRoot_idx =  Node_binarySearch(f_j, T_prime, marked); // Busqueda binaria doble para encontrar la entrada correspondiente a la raiz
            f_j.a = T_prime[correspondingRoot_idx];
            marked[correspondingRoot_idx] = true;
        } 
    }

    // liberamos espacio
    vector<Node*>().swap(T_prime); 
    vector<bool>().swap(marked); 

    /***************************
    * Paso 12
    ***************************/

    // Seteamos radios cobertores de las -antiguas- hojas de Tsup
    for(Node* &H_j : leaves) {
        for (Entry &f_j : H_j->Entries) {
            vector<Node *> new_leaves = move(getLeaves(f_j.a)).first;  // Las nuevas hojas del arbol Tsup luego del paso 11 (equivalentmente, nos referimos a Tsup como T)
            vector<Point> pts;

            pts.reserve(B*new_leaves.size());
            for (Node* &nH_j : new_leaves) {
                for (Entry &nf_j : nH_j->Entries)
                    pts.push_back(nf_j.p);
            }
            f_j.cr = calculateCR(pts, f_j.p, nullopt);   // Calculamos radio cobertor de las antiguas hojas
        }
    }

    // Seteamos radios cobertores del resto de Tsup (recordar que la parte inferior del arbol no fue modificada, solo basta modificar la region superior)

    // Es importante recordar que solo nos interesa actualizar los radios, por lo que una estrategia inteligente es solo acceder a los subarboles que se encuantran AFUERA del antiguo radio cobertor
    // De esta tarea se encarga `getOuterPoints`, que es iterativa y utiliza una modificación del algoritmo DFS

    vector<vector<Node*>> levels;
    if (leaves_height > 0)
        levels = getNodesUpToDepth(T_sup, leaves_height-1);
    int level_size = levels.size() - 1;
    for (int i = level_size; i > 0; i--) {
        vector<Node*> clevel = levels[i];
        for (Node* &N_j : clevel) { // Por cada nodo en este nivel
            for (Entry &entry : N_j->Entries) {
                vector<Point> outer_points = move(getOuterPoints(entry, entry.p, entry.cr.value()));
                entry.cr = calculateCR(outer_points, entry.p, entry.cr.value());
            }
        }
    }

    /***************************
    * Paso 13
    ***************************/

    return T_sup;

}

int main() {
    clock_t start = clock();
    int n = pow(2, 15);
    cout << "generating random points ..." << endl;
    vector<Point> points = randomPoints(n);
    MTree mt;
    mt.construct(points, "CP");
    clock_t end = clock();
    double seconds = static_cast<double>(end - start) / CLOCKS_PER_SEC;
    cout << endl;
    cout << "Time taken papito: " << seconds << " seconds"  << endl;
    mt.printf_mtree("CP");
    
    return 0;
}