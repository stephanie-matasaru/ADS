#pragma once
#include <vector>

template <typename K>
struct BTreeNode {
    int  n;                           
    bool leaf;                         
    std::vector<K>          keys;      
    std::vector<BTreeNode*> children;  

    explicit BTreeNode(bool isLeaf) : n(0), leaf(isLeaf) {}

    ~BTreeNode() {
        for (auto* c : children)
            delete c;
    }

    void diskWrite() const { /* no-op */ }
    void diskRead()  const { /* no-op */ }
};