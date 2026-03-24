#pragma once
#include <vector>
#include <queue>
#include <stdexcept>
#include <string>
#include <iostream>

#include "BTreeNode.h"

template <typename K>
class BTree {
public:
    using Node = BTreeNode<K>;

    explicit BTree(int minimumDegree) : t(minimumDegree), root(nullptr) {
        if (t < 2)
            throw std::invalid_argument("Minimum degree t must be >= 2");
        root = new Node(true);
        root->diskWrite();
    }

    ~BTree() { delete root; }

    BTree(const BTree&)            = delete;
    BTree& operator=(const BTree&) = delete;

    int  getT()    const { return t; }
    bool empty()   const { return root == nullptr || root->n == 0; }
    int  size()    const { return countKeys(root); }
    int  height()  const {
        int h = 0;
        Node* cur = root;
        while (cur && !cur->leaf) { cur = cur->children[0]; ++h; }
        return h;
    }

    bool contains(const K& k) const { return searchNode(root, k); }

    void insert(const K& k) {
        Node* r = root;
        if (r->n == 2 * t - 1) {
            Node* s = new Node(false);
            root    = s;
            s->n    = 0;
            s->children.push_back(r);
            splitChild(s, 0, r);
            insertNonfull(s, k);
        } else {
            insertNonfull(r, k);
        }
    }

    void remove(const K& k) {
        if (empty()) throw std::out_of_range("Tree is empty");
        deleteKey(root, k);
        if (root->n == 0 && !root->leaf) {
            Node* oldRoot = root;
            root = oldRoot->children[0];
            oldRoot->children.clear();   
            delete oldRoot;
        }
    }

    std::vector<K> traverse() const {
        std::vector<K> result;
        inOrder(root, result);
        return result;
    }

    K minimum() const {
        if (empty()) throw std::out_of_range("Tree is empty");
        Node* cur = root;
        while (!cur->leaf) cur = cur->children[0];
        return cur->keys[0];
    }

    K maximum() const {
        if (empty()) throw std::out_of_range("Tree is empty");
        Node* cur = root;
        while (!cur->leaf) cur = cur->children[cur->n];
        return cur->keys[cur->n - 1];
    }

    void printTree() const {
        if (empty()) { return; }

        struct Item { Node* node; int level; };
        std::queue<Item> q;
        q.push({root, 0});
        int curLevel = -1;

        while (!q.empty()) {
            auto [node, level] = q.front(); q.pop();

            if (level != curLevel) {
                if (curLevel >= 0) std::cout << "\n";
                curLevel = level;
                std::cout << "  Level " << level << " | ";
            }

            std::cout << "[";
            for (int i = 0; i < node->n; ++i) {
                if (i) std::cout << " | ";
                std::cout << node->keys[i];
            }
            std::cout << "]  ";

            for (auto* child : node->children)
                q.push({child, level + 1});
        }
        std::cout << "\n";
    }

    void clear() {
        delete root;
        root = new Node(true);
    }

private:
    int   t;
    Node* root;

    bool searchNode(Node* x, const K& k) const {
        int i = 0;
        while (i < x->n && k > x->keys[i]) ++i;
        if (i < x->n && k == x->keys[i]) return true;
        if (x->leaf) return false;
        x->children[i]->diskRead();
        return searchNode(x->children[i], k);
    }

    void splitChild(Node* x, int i, Node* y) {
        Node* z = new Node(y->leaf);
        z->n    = t - 1;

        for (int j = 0; j < t - 1; ++j)
            z->keys.push_back(y->keys[j + t]);

        if (!y->leaf)
            for (int j = 0; j < t; ++j)
                z->children.push_back(y->children[j + t]);

        K median  = y->keys[t - 1];
        y->keys.resize(t - 1);
        y->n = t - 1;
        if (!y->leaf) y->children.resize(t);

        x->children.insert(x->children.begin() + i + 1, z);
        x->keys.insert(x->keys.begin() + i, median);
        x->n += 1;

        y->diskWrite();
        z->diskWrite();
        x->diskWrite();
    }

    void insertNonfull(Node* x, const K& k) {
        int i = x->n - 1;
        if (x->leaf) {
            x->keys.push_back(K{});
            while (i >= 0 && k < x->keys[i]) {
                x->keys[i + 1] = x->keys[i];
                --i;
            }
            x->keys[i + 1] = k;
            x->n += 1;
            x->diskWrite();
        } else {
            while (i >= 0 && k < x->keys[i]) --i;
            ++i;
            x->children[i]->diskRead();
            if (x->children[i]->n == 2 * t - 1) {
                splitChild(x, i, x->children[i]);
                if (k > x->keys[i]) ++i;
            }
            insertNonfull(x->children[i], k);
        }
    }

    K getPredecessor(Node* x, int i) const {
        Node* cur = x->children[i];
        while (!cur->leaf) cur = cur->children[cur->n];
        return cur->keys[cur->n - 1];
    }

    K getSuccessor(Node* x, int i) const {
        Node* cur = x->children[i + 1];
        while (!cur->leaf) cur = cur->children[0];
        return cur->keys[0];
    }

    void merge(Node* x, int i) {
        Node* left  = x->children[i];
        Node* right = x->children[i + 1];

        left->keys.push_back(x->keys[i]);
        for (auto& k : right->keys)     left->keys.push_back(k);
        for (auto* c : right->children) left->children.push_back(c);
        left->n = static_cast<int>(left->keys.size());

        x->keys.erase(x->keys.begin() + i);
        x->children.erase(x->children.begin() + i + 1);
        x->n -= 1;

        right->children.clear();
        delete right;

        left->diskWrite();
        x->diskWrite();
    }

    void borrowFromPrev(Node* x, int i) {
        Node* child   = x->children[i];
        Node* sibling = x->children[i - 1];

        child->keys.insert(child->keys.begin(), x->keys[i - 1]);
        child->n += 1;

        if (!sibling->leaf) {
            child->children.insert(child->children.begin(), sibling->children.back());
            sibling->children.pop_back();
        }

        x->keys[i - 1] = sibling->keys.back();
        sibling->keys.pop_back();
        sibling->n -= 1;

        child->diskWrite();
        sibling->diskWrite();
        x->diskWrite();
    }

    void borrowFromNext(Node* x, int i) {
        Node* child   = x->children[i];
        Node* sibling = x->children[i + 1];

        child->keys.push_back(x->keys[i]);
        child->n += 1;

        if (!sibling->leaf) {
            child->children.push_back(sibling->children.front());
            sibling->children.erase(sibling->children.begin());
        }

        x->keys[i] = sibling->keys.front();
        sibling->keys.erase(sibling->keys.begin());
        sibling->n -= 1;

        child->diskWrite();
        sibling->diskWrite();
        x->diskWrite();
    }

    void fill(Node* x, int i) {
        if      (i > 0    && x->children[i - 1]->n >= t) borrowFromPrev(x, i);
        else if (i < x->n && x->children[i + 1]->n >= t) borrowFromNext(x, i);
        else {
            if (i < x->n) merge(x, i);
            else          merge(x, i - 1);
        }
    }

    void deleteKey(Node* x, const K& k) {
        int i = 0;
        while (i < x->n && k > x->keys[i]) ++i;

        if (i < x->n && x->keys[i] == k) {
            if (x->leaf) {
                x->keys.erase(x->keys.begin() + i);
                x->n -= 1;
                x->diskWrite();
            } else {
                Node* lc = x->children[i];
                Node* rc = x->children[i + 1];

                if (lc->n >= t) {
                    K pred     = getPredecessor(x, i);
                    x->keys[i] = pred;
                    deleteKey(lc, pred);
                } else if (rc->n >= t) {
                    K succ     = getSuccessor(x, i);
                    x->keys[i] = succ;
                    deleteKey(rc, succ);
                } else {
                    merge(x, i);
                    deleteKey(x->children[i], k);
                }
            }
        } else {
            if (x->leaf)
                throw std::out_of_range("Key not found in B-Tree");

            bool isLast = (i == x->n);
            if (x->children[i]->n == t - 1)
                fill(x, i);

            if (isLast && i > x->n)
                deleteKey(x->children[i - 1], k);
            else
                deleteKey(x->children[i], k);
        }
    }

    void inOrder(Node* x, std::vector<K>& result) const {
        if (!x) return;
        for (int i = 0; i < x->n; ++i) {
            if (!x->leaf) inOrder(x->children[i], result);
            result.push_back(x->keys[i]);
        }
        if (!x->leaf) inOrder(x->children[x->n], result);
    }

    int countKeys(Node* x) const {
        if (!x) return 0;
        int total = x->n;
        for (auto* c : x->children) total += countKeys(c);
        return total;
    }
};