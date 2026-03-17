#ifndef DYNAMICMEDIAN_H_
#define DYNAMICMEDIAN_H_

#include "RBNode.h"
#include <fstream>

#ifdef _WIN32
    #define CLEAR "cls"
#else
    #define CLEAR "clear"
#endif

using namespace std;

class MedianFinder {
    RBTree lo, hi;
    int loSize = 0, hiSize = 0;

    void balance() {
    while (loSize < hiSize) {
        RBNode* minHi = hi.minimum(hi.root);
        int k = minHi->key;
        hi.del(minHi);
        hiSize--;
        lo.RBInsert(lo.createNode(k));
        loSize++;
    }
    while (loSize > hiSize + 1) {
        RBNode* maxLo = lo.maximum(lo.root);
        int k = maxLo->key;
        lo.del(maxLo);
        loSize--;
        hi.RBInsert(hi.createNode(k));
        hiSize++;
    }
}

public:
    public:
    void add(int x) {
        cout << "DEBUG: inserting " << x << "\n";
        if (loSize == 0 || x <= lo.maximum(lo.root)->key) {
            lo.RBInsert(lo.createNode(x)); loSize++;
        } else {
            hi.RBInsert(hi.createNode(x)); hiSize++;
        }
        cout << "DEBUG: inserted, now balancing\n";
        balance();
        cout << "DEBUG: done\n";
    }

    void remove(int x) {
    RBNode* node = lo.search(lo.root, x);
    if (!lo.isNil(node)) {
        lo.del(node);
        loSize--;
    } else {
        node = hi.search(hi.root, x);
        if (!hi.isNil(node)) {
            hi.del(node);
            hiSize--;
        }
    }
    balance();
}

    void median() {
        if (loSize == 0) { cout << "No elements\n"; return; }
        cout << lo.maximum(lo.root)->key << "\n";
    }

    void saveToFile(const string& filename) {
        ofstream f(filename);
        if (!f) { cout << "Could not open file.\n"; return; }
        saveInorder(lo.root, lo, f);
        saveInorder(hi.root, hi, f);
        cout << "Saved to " << filename << "\n";
    }

    void loadFromFile(const string& filename) {
        ifstream f(filename);
        if (!f) { cout << "Could not open file.\n"; return; }
        int x;
        while (f >> x) add(x);
        cout << "Loaded from " << filename << "\n";
    }

private:
    void saveInorder(RBNode* node, RBTree& t, ofstream& f) {
        if (t.isNil(node)) return;
        saveInorder(node->left, t, f);
        f << node->key << "\n";
        saveInorder(node->right, t, f);
    }
};

void runMedian() {
    MedianFinder mf;
    mf.loadFromFile("median.txt");   
    int choice;
    while (true) {
        system(CLEAR);
        cout << "\n=== Median ===\n"
             << "1. Add number\n"
             << "2. Remove number\n"
             << "3. Get median\n"
             << "4. Exit\n"
             << "Enter choice (1-4): ";
        cin >> choice;
        cin.ignore(10000, '\n');

        if (choice == 1) {
            int x;
            cout << "Number: "; cin >> x;
            mf.add(x);
            cout << "Added " << x << "\n";

        } else if (choice == 2) {
            int x;
            cout << "Number: "; cin >> x;
            mf.remove(x);
            cout << "Removed " << x << "\n";

        } else if (choice == 3) {
            cout << "Median: ";
            mf.median();

        } else if (choice == 4) {
            mf.saveToFile("median.txt");   
            break;
        } else {
            cout << "Unknown option.\n";
        }
        cout << "Press Enter to continue...";
        cin.ignore(10000, '\n');
    }
}

#endif