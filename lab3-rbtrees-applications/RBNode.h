#ifndef RBNODE_H_
#define RBNODE_H_

#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>

using namespace std;

struct RBNode {
    int key;
    string name;
    RBNode *p, *left, *right;
    enum color { RED, BLACK };
    color col;

    static RBNode* Nil;

    RBNode(int k, string n = "", RBNode *l = nullptr, RBNode *r = nullptr, RBNode *parent = nullptr, color c = RBNode::BLACK) :
        key(k), name(n), p(parent), left(l), right(r), col(c) { }

    string toString() {
        ostringstream os;
        os << key << ((col == RBNode::RED) ? ":r" : ":b");
        return os.str();
    }

    ~RBNode() {
        if (this != Nil) {
            delete left;
            delete right;
        }
    }
};

RBNode* RBNode::Nil = new RBNode(0);

struct RBTree {
    RBNode* Nil;    
    RBNode* root;

    RBTree() {
        Nil = new RBNode(0);
        Nil->left = Nil;
        Nil->right = Nil;
        Nil->p = Nil;
        Nil->col = RBNode::BLACK;
        root = Nil;
    }

    ~RBTree() {
        delete root;
        delete Nil;
    }

    RBNode* createNode(int key, string name = "") {
        RBNode* n = new RBNode(key, name);
        n->left = n->right = n->p = Nil;  
        n->col = RBNode::RED;
        return n;
    }

    bool isNil(RBNode* n) { return (n == Nil); }  

    void LeftRotate(RBNode* x) {
        RBNode* y = x->right;
        x->right = y->left;
        if (y->left != Nil)
            y->left->p = x;
        y->p = x->p;
        if (x->p == Nil)
            root = y;
        else if (x == x->p->left)
            x->p->left = y;
        else
            x->p->right = y;
        y->left = x;
        x->p = y;
    }

    void RightRotate(RBNode* y) {
        RBNode* x = y->left;
        y->left = x->right;
        if (x->right != Nil)
            x->right->p = y;
        x->p = y->p;
        if (y->p == Nil)
            root = x;
        else if (y == y->p->left)
            y->p->left = x;
        else
            y->p->right = x;
        x->right = y;
        y->p = x;
    }

    RBNode* search(RBNode* w, int key) {
        if (isNil(w) || w->key == key)
            return w;
        return search((key < w->key) ? w->left : w->right, key);
    }

    RBNode* maximum(RBNode* w) {
        RBNode* x = w;
        while (!isNil(x->right))
            x = x->right;
        return x;
    }

    RBNode* minimum(RBNode* w) {
        RBNode* x = w;
        while (!isNil(x->left))
            x = x->left;
        return x;
    }

    RBNode* successor(RBNode* w) {
        if (isNil(w)) return w;
        RBNode* x = w;
        if (!isNil(x->right))
            return minimum(x->right);
        RBNode* y = x->p;
        while (!isNil(y) && x == y->right) {
            x = y;
            y = x->p;
        }
        return y;
    }

    RBNode* predecessor(RBNode* w) {
        if (isNil(w)) return w;
        RBNode* x = w;
        if (!isNil(x->left))
            return maximum(x->left);
        RBNode* y = x->p;
        while (!isNil(y) && x == y->left) {
            x = y;
            y = x->p;
        }
        return y;
    }

    void RBInsert(RBNode* z) {
        RBNode* y = Nil;
        RBNode* x = root;
        while (!isNil(x)) {
            y = x;
            x = (z->key < x->key) ? x->left : x->right;
        }
        z->p = y;
        if (isNil(y))
            root = z;
        else if (z->key < y->key)
            y->left = z;
        else
            y->right = z;
        z->left = z->right = Nil;
        z->col = RBNode::RED;
        RBInsertFixup(z);
    }

    RBNode* del(RBNode* z) {
        RBNode* y = (isNil(z->left) || isNil(z->right)) ? z : successor(z);
        RBNode* x = !isNil(y->left) ? y->left : y->right;
        x->p = y->p;
        if (isNil(y->p)) {
            root = x;
        } else {
            if (y == y->p->left)
                y->p->left = x;
            else
                y->p->right = x;
        }
        if (y != z)
            z->key = y->key;
        if (y->col == RBNode::BLACK)
            RBDeleteFixup(x);
        return y;
    }

    void RBDeleteFixup(RBNode* x) {
        RBNode* w;
        while ((x != root) && (x->col == RBNode::BLACK)) {
            if (x == x->p->left) {
                w = x->p->right;
                if (w->col == RBNode::RED) {
                    w->col = RBNode::BLACK;
                    x->p->col = RBNode::RED;
                    LeftRotate(x->p);
                    w = x->p->right;
                }
                if ((w->left->col == RBNode::BLACK) && (w->right->col == RBNode::BLACK)) {
                    w->col = RBNode::RED;
                    x = x->p;
                } else {
                    if (w->right->col == RBNode::BLACK) {
                        w->left->col = RBNode::BLACK;
                        w->col = RBNode::RED;
                        RightRotate(w);
                        w = x->p->right;
                    }
                    w->col = x->p->col;
                    x->p->col = RBNode::BLACK;
                    w->right->col = RBNode::BLACK;
                    LeftRotate(x->p);
                    x = root;
                }
            } else {
                w = x->p->left;
                if (w->col == RBNode::RED) {
                    w->col = RBNode::BLACK;
                    x->p->col = RBNode::RED;
                    RightRotate(x->p);
                    w = x->p->left;
                }
                if ((w->left->col == RBNode::BLACK) && (w->right->col == RBNode::BLACK)) {
                    w->col = RBNode::RED;
                    x = x->p;
                } else {
                    if (w->left->col == RBNode::BLACK) {
                        w->right->col = RBNode::BLACK;
                        w->col = RBNode::RED;
                        LeftRotate(w);
                        w = x->p->left;
                    }
                    w->col = x->p->col;
                    x->p->col = RBNode::BLACK;
                    w->left->col = RBNode::BLACK;
                    RightRotate(x->p);
                    x = root;
                }
            }
        }
        x->col = RBNode::BLACK;
    }

    void RBInsertFixup(RBNode* z) {
        while (z->p->col == RBNode::RED)
            if (z->p == z->p->p->left) {
                RBNode* y = z->p->p->right;
                if (y->col == RBNode::RED) {
                    z->p->col = RBNode::BLACK;
                    y->col = RBNode::BLACK;
                    z->p->p->col = RBNode::RED;
                    z = z->p->p;
                } else {
                    if (z == z->p->right) {
                        z = z->p;
                        LeftRotate(z);
                    }
                    z->p->col = RBNode::BLACK;
                    z->p->p->col = RBNode::RED;
                    RightRotate(z->p->p);
                }
            } else {
                RBNode* y = z->p->p->left;
                if (y->col == RBNode::RED) {
                    z->p->col = RBNode::BLACK;
                    y->col = RBNode::BLACK;
                    z->p->p->col = RBNode::RED;
                    z = z->p->p;
                } else {
                    if (z == z->p->left) {
                        z = z->p;
                        RightRotate(z);
                    }
                    z->p->col = RBNode::BLACK;
                    z->p->p->col = RBNode::RED;
                    LeftRotate(z->p->p);
                }
            }
        root->col = RBNode::BLACK;
    }

    void inorder(RBNode* T) {
        if (!isNil(T)) {
            inorder(T->left);
            cout << T->toString() << ' ';
            inorder(T->right);
        }
    }

    void inorder() {
        if (isNil(root)) cout << "empty";
        else inorder(root);
    }

    void display(RBNode* w, int indent) {
        if (!isNil(w)) {
            display(w->right, indent + 2);
            for (int i = 0; i < indent; i++) cout << ' ';
            cout << w->toString() << '\n';
            display(w->left, indent + 2);
        }
    }

    void indentedDisplay() {
        if (isNil(root)) cout << "The RB tree is empty" << endl;
        else { cout << "The RB tree is" << endl; display(root, 0); }
    }

    int bh(RBNode* x) { return (isNil(x) ? 0 : countBlacks(x->left)); }

    int countBlacks(RBNode* x) {
        if (isNil(x)) return 1;
        int n = countBlacks(x->left);
        return ((x->col == RBNode::RED) ? n : n + 1);
    }

    int bh() {
        if (isNil(root)) return 0;
        int height = 0;
        RBNode* curr = root;
        while (!isNil(curr)) {
            if (curr->col == RBNode::BLACK) height++;
            curr = curr->left;
        }
        return height;
    }

    int depth(RBNode* x) {
        if (isNil(x)) return 0;
        int a = depth(x->left);
        int b = depth(x->right);
        return (a < b ? b : a) + 1;
    }

    int depth() { return depth(root); }

    void findMaxKey(RBNode* x, RBNode::color c, int& currentMax) {
        if (isNil(x)) return;
        if (x->col == c) currentMax = std::max(currentMax, x->key);
        findMaxKey(x->left, c, currentMax);
        findMaxKey(x->right, c, currentMax);
    }

    int maxBlackKey() { int m = -1000; findMaxKey(root, RBNode::BLACK, m); return m; }
    int maxRedKey()   { int m = -1000; findMaxKey(root, RBNode::RED,   m); return m; }
};

#endif