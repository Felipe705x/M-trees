#include "../../headers/general/mtree_struct.h"
#include "../../headers/general/mtree_funcs.h"


bool Entry::operator<(const Entry& other) const {
    return p < other.p;
}

void MTree::construct(vector <Point> &points, const string method) {
    if (method == "SS")
        root = Algorithm_SS(points);
    else if (method == "CP")
        root = Algorithm_CP(points);
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
                os << fixed << setprecision(3) << "(" << entry.p.x << ", " << entry.p.y << ")";
                
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