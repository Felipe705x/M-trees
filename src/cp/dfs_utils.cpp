#include "../../headers/cp/dfs_utils.h"

vector<Point> getOuterPoints_recursive(const Entry &entry, const Point &q, const double &r) {
    if (entry.a == nullptr) {
        if (dist(entry.p, q) > r * r)
            return {{entry.p}};
        else
            return {};
    }

    if (sqrt(dist(entry.p, q)) > (entry.cr.value() + r)) {
        int child_size = entry.a->Entries.size();
        vector<vector<Point>> unjoined_points(child_size);
        for (int i = 0; i < child_size; i++) {
            unjoined_points[i] = getOuterPoints_recursive(entry.a->Entries[i], q, r);
        }

        // Calculate the total number of points to reserve space in ans
        vector<Point> ans;
        size_t total_size = 0;
        for (const auto& child_points : unjoined_points) {
            total_size += child_points.size();
        }
        ans.reserve(total_size);

        // Insert all points into ans
        for (const auto& child_points : unjoined_points) {
            ans.insert(ans.end(), child_points.begin(), child_points.end());
        }

        return ans;
    } else {
        return {};
    }
}


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
        StackFrame &top = stack.top();
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
                for (Point &pt : top.points) {
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