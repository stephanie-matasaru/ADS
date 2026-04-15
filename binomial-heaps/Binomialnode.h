#pragma once
struct BinomialNode {
    int           key;
    int           degree;
    BinomialNode* parent;
    BinomialNode* child;
    BinomialNode* sibling;

    explicit BinomialNode(int k)
        : key(k), degree(0),
          parent(nullptr), child(nullptr), sibling(nullptr) {}
};