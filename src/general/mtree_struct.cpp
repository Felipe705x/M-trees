#include "../../headers/general/mtree_struct.h"
#include "../../headers/general/mtree_funcs.h"
#include <filesystem>

namespace fs = filesystem;

string project_root = "C:/GH_Local/DCCY2S1/M-trees";

bool Entry::operator<(const Entry& other) const {
    return p < other.p;
}

void MTree::construct(vector <Point> &points, const string method) {
    if (method == "SS")
        root = Algorithm_SS(points);
    else if (method == "CP")
        root = Algorithm_CP(points);
    size = points.size();
}

void MTree::printf_mtree(string method) {

    fs::path dirPath = project_root + "/build/results/mtree/" + method;
    std::cout << "Attempting to create directories at: " << fs::absolute(dirPath) << std::endl;
    if (!fs::exists(dirPath)) {
        if (fs::create_directories(dirPath)) {
            std::cout << "Created directory path: " << dirPath << std::endl;
        } else {
            std::cerr << "Failed to create directory path: " << dirPath << std::endl;
            return;
        }
    }


    ostringstream fileName;
    if (size < 1)
        return;
    int pwr = log2(size);
    fileName << project_root << "/build/results/mtree/" << method << "/S_" << pwr << ".csv";
    cout << fileName.str();
    ofstream outFile(fileName.str());

    vector<Node*> leaves;
    queue<Node*> nodesQueue;
    nodesQueue.push(root);
    int H = -1;
    while (!nodesQueue.empty()) {
        int levelSize = nodesQueue.size(); // Cantidad de nodos en esta altura
        for (int i = 0; i < levelSize; i++) {
            Node* cnode = nodesQueue.front(); // current_node
            nodesQueue.pop();
            if (cnode->Entries[0].a != nullptr) { // Entonces no es una hoja
                for (Entry &entry : cnode->Entries) {
                    nodesQueue.push(entry.a);
                    outFile << "p:(" << entry.p.x 
                            << "," << entry.p.y 
                            << "),h:" << H+1 
                            << ",n:" << cnode->Entries.size() 
                            << ",cr:" << entry.cr.value() 
                            <<";\n";
                }
            }
            else
                leaves.push_back(const_cast<Node*> (cnode));
                            
        }
        H++;
    }
    for (Node* leaf : leaves) {
        for (Entry &entry : leaf->Entries) {
            outFile << "p:(" << entry.p.x 
                            << "," << entry.p.y 
                            << "),h:" << H 
                            << ",n:" << 0 
                            << ",cr:" << 0 
                            <<";\n";
        }
    }
    outFile.close();
}

ostream& operator<<(ostream& os, const MTree& mt) {
    if (mt.root == nullptr)
        return os;
    queue <Node*> Queue;
    Queue.push(mt.root);

    while (!Queue.empty()) {
        int levelSize = Queue.size();
        for (int i = 0; i < levelSize; i++) {
            Node *current = Queue.front();
            Queue.pop();
            os << "[";
            for (int j = 0; j < current->Entries.size(); j++) {
                const Entry &entry = current->Entries[j];
                if (entry.cr.has_value())
                    os << fixed << setprecision(1) << "((" << entry.p.x << ", " << entry.p.y << "), " << entry.cr.value() << ")";
                else
                    os << fixed << setprecision(1) << "(" << entry.p.x << ", " << entry.p.y << ")";
                if (entry.a != nullptr)
                    Queue.push(entry.a);
            }
            os << "]";
        }
        os << endl;
        os << endl;
    }
    return os;
}

// Destructor del Mtree, basado en la función `getLeaves`
void MTree::destroy() {
    queue<Node*> nodesQueue;
    if (root == nullptr)
        return;
    nodesQueue.push(root);
    while (!nodesQueue.empty()) {
        int levelSize = nodesQueue.size(); // Cantidad de nodos en esta altura
        for (int i = 0; i < levelSize; i++) {
            Node* cnode = nodesQueue.front(); // current_node
            nodesQueue.pop();
            if (cnode->Entries[0].a != nullptr) { // Entonces no es una hoja
                for (Entry &entry : cnode->Entries)
                    nodesQueue.push(entry.a);
            }
            delete cnode;                
        }
    }
}