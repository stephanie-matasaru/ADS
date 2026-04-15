#pragma once
#include <iostream>
#include <stdexcept>
#include <vector>
#include <string>
#include <climits>
#include "BinomialNode.h"

class BinomialHeap {
public:
    BinomialHeap() : head(nullptr), nodeCount(0) {}

    ~BinomialHeap() { destroyAll(head); }

    // Disable copy
    BinomialHeap(const BinomialHeap&)            = delete;
    BinomialHeap& operator=(const BinomialHeap&) = delete;

    bool empty() const { return head == nullptr; }
    int  size()  const { return nodeCount; }

    // ── Insert(H, key) ────────────────────────────────────────
    // Creates a single-node heap and unions it with this heap.
    void insert(int key) {
        BinomialNode* node = new BinomialNode(key);
        BinomialHeap  tmp;
        tmp.head      = node;
        tmp.nodeCount = 1;
        unionWith(tmp);
        tmp.head = nullptr; // ownership transferred
    }

    // ── Minimum(H) ────────────────────────────────────────────
    // Walks the root list and returns the smallest key.
    int minimum() const {
        if (empty()) throw std::out_of_range("Heap is empty");
        BinomialNode* cur = head;
        int           min = cur->key;
        while (cur) {
            if (cur->key < min) min = cur->key;
            cur = cur->sibling;
        }
        return min;
    }

    // ── ExtractMin(H) ─────────────────────────────────────────
    // Removes and returns the minimum key.
    int extractMin() {
        if (empty()) throw std::out_of_range("Heap is empty");

        // Find root with minimum key and its predecessor in root list
        BinomialNode* minNode = nullptr;
        BinomialNode* minPrev = nullptr;
        BinomialNode* prev    = nullptr;
        BinomialNode* cur     = head;

        int minKey = cur->key;
        minNode = cur;

        while (cur) {
            if (cur->key < minKey) {
                minKey  = cur->key;
                minNode = cur;
                minPrev = prev;
            }
            prev = cur;
            cur  = cur->sibling;
        }

        // Remove minNode from root list
        if (minPrev)
            minPrev->sibling = minNode->sibling;
        else
            head = minNode->sibling;

        // Reverse the child list of minNode to form a new heap
        BinomialHeap childHeap;
        BinomialNode* child = minNode->child;
        BinomialNode* next  = nullptr;
        while (child) {
            next            = child->sibling;
            child->sibling  = childHeap.head;
            child->parent   = nullptr;
            childHeap.head  = child;
            child           = next;
        }
        // Count children (degree of minNode)
        childHeap.nodeCount = (1 << minNode->degree) - 1;
        nodeCount -= 1;

        // Union remaining heap with child heap
        unionWith(childHeap);
        childHeap.head = nullptr;

        int result = minNode->key;
        minNode->child   = nullptr;
        minNode->sibling = nullptr;
        delete minNode;
        return result;
    }

    // ── Union(H1, H2) ─────────────────────────────────────────
    // Merges another heap into this one. The other heap is emptied.
    void unionWith(BinomialHeap& other) {
        head      = mergeRootLists(head, other.head);
        nodeCount += other.nodeCount;
        other.head      = nullptr;
        other.nodeCount = 0;

        if (!head) return;

        BinomialNode* prev    = nullptr;
        BinomialNode* cur     = head;
        BinomialNode* next    = cur->sibling;

        while (next) {
            // Cases 1 & 2: degrees differ, or three in a row with same degree
            if (cur->degree != next->degree ||
               (next->sibling && next->sibling->degree == cur->degree)) {
                prev = cur;
                cur  = next;
            } else {
                // Cases 3 & 4: link the two trees of equal degree
                if (cur->key <= next->key) {
                    cur->sibling = next->sibling;
                    linkTrees(next, cur); // next becomes child of cur
                } else {
                    if (prev)
                        prev->sibling = next;
                    else
                        head = next;
                    linkTrees(cur, next); // cur becomes child of next
                    cur = next;
                }
            }
            next = cur->sibling;
        }
    }

    // ── DecreaseKey(node, newKey) ─────────────────────────────
    // Decreases the key of a given node and bubbles it up.
    // Returns pointer to the node with the new key (for chaining).
    BinomialNode* decreaseKey(BinomialNode* node, int newKey) {
        if (newKey > node->key)
            throw std::invalid_argument("New key is greater than current key");
        node->key = newKey;
        BinomialNode* cur    = node;
        BinomialNode* parent = cur->parent;
        while (parent && cur->key < parent->key) {
            std::swap(cur->key, parent->key);
            cur    = parent;
            parent = cur->parent;
        }
        return cur;
    }

    // ── Delete(node) ──────────────────────────────────────────
    // Deletes an arbitrary node by decreasing its key to -INF
    // and then extracting the minimum.
    void deleteNode(BinomialNode* node) {
        decreaseKey(node, INT_MIN);
        extractMin();
    }

    // ── Find node by key (linear scan) ───────────────────────
    BinomialNode* findNode(int key) const {
        return findInTree(head, key);
    }

    // ── Print heap (all binomial trees) ──────────────────────
    void printHeap() const {
        if (empty()) {
            std::cout << "  (empty heap)\n";
            return;
        }
        BinomialNode* cur = head;
        while (cur) {
            std::cout << "  B" << cur->degree << " tree  (root=" << cur->key << "):\n";
            printTree(cur, "    ", false);
            cur = cur->sibling;
        }
    }

    // ── Print root list only ──────────────────────────────────
    void printRoots() const {
        if (empty()) { std::cout << "  (empty)\n"; return; }
        std::cout << "  Roots: ";
        BinomialNode* cur = head;
        while (cur) {
            std::cout << cur->key << "(B" << cur->degree << ")";
            if (cur->sibling) std::cout << " -> ";
            cur = cur->sibling;
        }
        std::cout << "\n";
    }

// ─────────────────────────────────────────────────────────────
private:
    BinomialNode* head;
    int           nodeCount;

    // Link two binomial trees of equal degree.
    // y becomes the leftmost child of z.  (z->key <= y->key)
    void linkTrees(BinomialNode* y, BinomialNode* z) {
        y->parent  = z;
        y->sibling = z->child;
        z->child   = y;
        z->degree += 1;
    }

    // Merge two root lists by degree (like merge sort).
    BinomialNode* mergeRootLists(BinomialNode* h1, BinomialNode* h2) {
        if (!h1) return h2;
        if (!h2) return h1;

        BinomialNode* result = nullptr;
        BinomialNode** tail  = &result;

        while (h1 && h2) {
            if (h1->degree <= h2->degree) {
                *tail = h1; h1 = h1->sibling;
            } else {
                *tail = h2; h2 = h2->sibling;
            }
            tail = &((*tail)->sibling);
        }
        *tail = h1 ? h1 : h2;
        return result;
    }

    // Recursive tree printer (ASCII art)
    void printTree(BinomialNode* node, const std::string& prefix, bool isLeft) const {
        if (!node) return;
        std::cout << prefix;
        std::cout << (isLeft ? "|-- " : "\\-- ");
        std::cout << node->key << "\n";
        BinomialNode* child = node->child;
        std::vector<BinomialNode*> children;
        while (child) { children.push_back(child); child = child->sibling; }
        for (int i = 0; i < (int)children.size(); ++i) {
            bool last = (i == (int)children.size() - 1);
            printTree(children[i],
                      prefix + (isLeft ? "|   " : "    "),
                      !last);
        }
    }

    // Recursive key search
    BinomialNode* findInTree(BinomialNode* node, int key) const {
        if (!node) return nullptr;
        if (node->key == key) return node;
        BinomialNode* found = findInTree(node->child, key);
        if (found) return found;
        return findInTree(node->sibling, key);
    }

    // Recursive destruction
    void destroyAll(BinomialNode* node) {
        if (!node) return;
        destroyAll(node->child);
        destroyAll(node->sibling);
        delete node;
    }
};