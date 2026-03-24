#pragma once
#include <vector>
#include <stdexcept>

#include "BTree.h"
#include "IODialog.h"

class Menu {
public:
    explicit Menu(int t) : tree(t) {}

    void run() {
        while (true) {
            IODialog::clearScreen();
            IODialog::printHeader(tree.getT());
            IODialog::printMenu();

            int choice = IODialog::readMenuChoice("  Choice: ");

            IODialog::clearScreen();
            IODialog::printHeader(tree.getT());

            switch (choice) {
                case 1:  doInsertOne();      break;
                case 2:  doInsertMany();     break;
                case 3:  doDelete();         break;
                case 4:  doSearch();         break;
                case 5:  doPrintTree();      break;
                case 6:  doTraverse();       break;
                case 7:  doStats();          break;
                case 8:  doClear();          break;
                case 9:  doLoadDemo();       break;
                case 0:  doExit(); return;
                default:
                    IODialog::printError("Invalid option – please choose 0-9.");
                    IODialog::pressEnter();
                    break;
            }
        }
    }

private:
    BTree<int> tree;

    void doInsertOne() {
        int k = IODialog::readInt("  Enter key to insert: ");
        tree.insert(k);
        IODialog::printSuccess("Inserted " + std::to_string(k));
        std::cout << "\n  Tree after insertion:\n";
        tree.printTree();
        IODialog::pressEnter();
    }

    void doInsertMany() {
        auto values = IODialog::readIntList(
            "  Enter keys separated by spaces (e.g. 10 20 30): ");

        if (values.empty()) {
            IODialog::printError("No valid integers found.");
        } else {
            for (int v : values) tree.insert(v);
            IODialog::printSuccess("Inserted " + std::to_string(values.size()) + " key(s)");
            std::cout << "\n  Tree after insertion:\n";
            tree.printTree();
        }
        IODialog::pressEnter();
    }

    void doDelete() {
        if (tree.empty()) {
            IODialog::printError("Tree is empty.");
            IODialog::pressEnter();
            return;
        }
        int k = IODialog::readInt("  Enter key to delete: ");
        try {
            tree.remove(k);
            IODialog::printSuccess("Deleted " + std::to_string(k));
            std::cout << "\n  Tree after deletion:\n";
            tree.printTree();
        } catch (const std::out_of_range& e) {
            IODialog::printError(e.what());
        }
        IODialog::pressEnter();
    }

    void doSearch() {
        int k = IODialog::readInt("  Enter key to search: ");
        if (tree.contains(k))
            IODialog::printSuccess("Key " + std::to_string(k) + " FOUND in the tree.");
        else
            IODialog::printError("Key " + std::to_string(k) + " NOT found in the tree.");
        IODialog::pressEnter();
    }

    void doPrintTree() {
        std::cout << "  Tree structure (level-order BFS):\n\n";
        if (tree.empty())
            IODialog::printInfo("(empty tree)");
        else
            tree.printTree();
        IODialog::pressEnter();
    }

    void doTraverse() {
        IODialog::printTraversal(tree.traverse());
        IODialog::pressEnter();
    }

    void doStats() {
        if (tree.empty()) {
            IODialog::printInfo("(tree is empty)");
        } else {
            IODialog::printStats(
                tree.size(),
                tree.height(),
                tree.minimum(),
                tree.maximum(),
                tree.getT()
            );
        }
        IODialog::pressEnter();
    }

    void doClear() {
        char ans = IODialog::readChar("  Are you sure you want to clear the tree? (y/n): ");
        if (ans == 'y' || ans == 'Y') {
            tree.clear();
            IODialog::printSuccess("Tree cleared.");
        } else {
            IODialog::printInfo("Cancelled.");
        }
        IODialog::pressEnter();
    }

    void doLoadDemo() {
        std::vector<int> demo = {10,20,5,6,12,30,7,17,3,1,40,50,25,35,15,45,8,22,28,11};
        for (int v : demo) tree.insert(v);
        IODialog::printSuccess("Loaded " + std::to_string(demo.size()) + " demo keys.");
        std::cout << "\n  Tree after loading demo:\n";
        tree.printTree();
        IODialog::pressEnter();
    }

    void doExit() {
        IODialog::clearScreen();
        std::cout
                  << "\nGoodbye!\n";
    }
};