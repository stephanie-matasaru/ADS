#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

struct Node {
    int key;
    Node *p, *left, *right;
    Node(int val) : key(val), p(nullptr), left(nullptr), right(nullptr) {}
};

class BSTree {
private:
    void savePreOrder(Node* n, std::ofstream &out) {
        if (n == nullptr) return;
        out << n->key << " ";
        savePreOrder(n->left, out);
        savePreOrder(n->right, out);
    }

    void save() {
        std::ofstream out("bst.txt");
        savePreOrder(root, out);
        out.close();
    }

public:
    Node *root;
    BSTree() : root(nullptr) {}

    void insert(int val, bool triggerSave = true) {
        Node* z = new Node(val);
        Node* y = nullptr;
        Node* x = root;
        while (x != nullptr) {
            y = x;
            if (z->key < x->key) x = x->left;
            else x = x->right;
        }
        z->p = y;
        if (y == nullptr) root = z;
        else if (z->key < y->key) y->left = z;
        else y->right = z;

        if (triggerSave) save(); 
    }

    void del(Node* z) {
        if (!z) return;
        Node *y, *x;
        if (z->left == nullptr || z->right == nullptr) y = z;
        else y = successor(z);

        if (y->left != nullptr) x = y->left;
        else x = y->right;

        if (x != nullptr) x->p = y->p;
        if (y->p == nullptr) root = x;
        else if (y == y->p->left) y->p->left = x;
        else y->p->right = x;

        if (y != z) z->key = y->key;
        delete y;
        
        save();
    }

    Node* search(int k) {
        Node* x = root;
        while (x != nullptr && k != x->key) {
            if (k < x->key) x = x->left;
            else x = x->right;
        }
        return x;
    }

    Node* minimum(Node* x) {
        while (x && x->left) x = x->left;
        return x;
    }

    Node* maximum(Node* x) {
        while (x && x->right) x = x->right;
        return x;
    }

    Node* successor(Node* x) {
        if (x->right) return minimum(x->right);
        Node* y = x->p;
        while (y && x == y->right) { x = y; y = y->p; }
        return y;
    }

    void loadFromFile() {
        std::ifstream in("bst.txt");
        int val;
        while (in >> val) {
            insert(val, false); 
        }
        in.close();
    }

    void printInOrder(Node* n) {
        if (!n) return;
        printInOrder(n->left);
        std::cout << n->key << " ";
        printInOrder(n->right);
    }
};

int main() {
    BSTree tree;
    tree.loadFromFile(); 

    int choice, val;
    while (true) {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif

        std::cout << "--- MENU ---\n";
        std::cout << "1. Insert\n2. Delete\n3. Search\n4. Min/Max\n5. View Tree (In-order)\n6. Exit\n";
        std::cout << "----------------------------------\n";
        std::cout << "Choice: ";
        
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            continue;
        }

        switch (choice) {
            case 1:
                std::cout << "Value to insert: "; std::cin >> val;
                tree.insert(val);
                std::cout << "\nSuccess: " << val << " added to tree.";
                break;
            case 2:
                std::cout << "Value to delete: "; std::cin >> val;
                {
                    Node* target = tree.search(val);
                    if (target) {
                        tree.del(target);
                        std::cout << "\nSuccess: " << val << " removed.";
                    } else std::cout << "\nError: Value not found.";
                }
                break;
            case 3:
                std::cout << "Search for: "; std::cin >> val;
                std::cout << (tree.search(val) ? "\nResult: Found!" : "\nResult: Not in tree.");
                break;
            case 4:
                if (!tree.root) std::cout << "\nTree is empty.";
                else {
                    std::cout << "\nMin: " << tree.minimum(tree.root)->key;
                    std::cout << " | Max: " << tree.maximum(tree.root)->key;
                }
                break;
            case 5:
                std::cout << "\nCurrent Tree (Sorted): ";
                tree.printInOrder(tree.root);
                break;
            case 6:
                return 0;
            default:
                std::cout << "\nInvalid choice.";
        }

        std::cout << "\n\nPress Enter to return to menu";
        std::cin.ignore(1000, '\n'); 
        std::cin.get(); 
    }
}