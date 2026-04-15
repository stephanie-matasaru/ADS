#pragma once
#include <stdexcept>
#include <vector>
#include <memory>

#include "BinomialHeap.h"
#include "IODialog.h"

class Menu {
public:
    Menu() : heap(new BinomialHeap()) {}
    ~Menu() { delete heap; }

    void run() {
        while (true) {
            IODialog::clearScreen();
            IODialog::printHeader(heap->size());
            IODialog::printMenu();

            int choice = IODialog::readMenuChoice("  Choice: ");

            IODialog::clearScreen();
            IODialog::printHeader(heap->size());

            switch (choice) {
                case 1:  doInsertOne();    break;
                case 2:  doInsertMany();   break;
                case 3:  doMinimum();      break;
                case 4:  doExtractMin();   break;
                case 5:  doDecreaseKey();  break;
                case 6:  doDeleteNode();   break;
                case 7:  doPrintHeap();    break;
                case 8:  doPrintRoots();   break;
                case 9:  doLoadDemo();     break;
                case 0:  doExit(); return;
                default:
                    IODialog::printError("Invalid option. Please choose 0-9.");
                    IODialog::pressEnter();
                    break;
            }
        }
    }

private:
    BinomialHeap* heap;

    void doInsertOne() {
        int k = IODialog::readInt("  Enter key to insert: ");
        heap->insert(k);
        IODialog::printSuccess("Inserted " + std::to_string(k));
        std::cout << "\n  Heap after insertion:\n";
        heap->printRoots();
        IODialog::pressEnter();
    }

    void doInsertMany() {
        auto values = IODialog::readIntList(
            "  Enter keys separated by spaces: ");
        if (values.empty()) {
            IODialog::printError("No valid integers found.");
        } else {
            for (int v : values) heap->insert(v);
            IODialog::printSuccess("Inserted " + std::to_string(values.size()) + " key(s)");
            std::cout << "\n  Heap after insertion:\n";
            heap->printRoots();
        }
        IODialog::pressEnter();
    }

    void doMinimum() {
        if (heap->empty()) {
            IODialog::printError("Heap is empty.");
        } else {
            IODialog::printSuccess("Minimum key = " + std::to_string(heap->minimum()));
        }
        IODialog::pressEnter();
    }

    void doExtractMin() {
        if (heap->empty()) {
            IODialog::printError("Heap is empty.");
        } else {
            int val = heap->extractMin();
            IODialog::printSuccess("Extracted minimum = " + std::to_string(val));
            std::cout << "\n  Heap after extraction:\n";
            heap->printRoots();
        }
        IODialog::pressEnter();
    }

    void doDecreaseKey() {
        if (heap->empty()) {
            IODialog::printError("Heap is empty.");
            IODialog::pressEnter();
            return;
        }
        int oldKey = IODialog::readInt("  Enter current key of node: ");
        int newKey = IODialog::readInt("  Enter new (smaller) key: ");

        BinomialNode* node = heap->findNode(oldKey);
        if (!node) {
            IODialog::printError("Key " + std::to_string(oldKey) + " not found.");
        } else {
            try {
                heap->decreaseKey(node, newKey);
                IODialog::printSuccess("Key updated from " + std::to_string(oldKey)
                                       + " to " + std::to_string(newKey));
                std::cout << "\n  Heap after decrease:\n";
                heap->printRoots();
            } catch (const std::invalid_argument& e) {
                IODialog::printError(e.what());
            }
        }
        IODialog::pressEnter();
    }

    void doDeleteNode() {
        if (heap->empty()) {
            IODialog::printError("Heap is empty.");
            IODialog::pressEnter();
            return;
        }
        int k = IODialog::readInt("  Enter key of node to delete: ");
        BinomialNode* node = heap->findNode(k);
        if (!node) {
            IODialog::printError("Key " + std::to_string(k) + " not found.");
        } else {
            heap->deleteNode(node);
            IODialog::printSuccess("Deleted node with key " + std::to_string(k));
            std::cout << "\n  Heap after deletion:\n";
            heap->printRoots();
        }
        IODialog::pressEnter();
    }

    void doPrintHeap() {
        std::cout << "  Full heap structure:\n\n";
        heap->printHeap();
        IODialog::pressEnter();
    }

    void doPrintRoots() {
        std::cout << "  Root list (by degree):\n";
        heap->printRoots();
        IODialog::pressEnter();
    }

    void doLoadDemo() {
        std::vector<int> demo = {10, 1, 12, 25, 6, 8, 14, 29, 11, 17, 38, 7};
        for (int v : demo) heap->insert(v);
        IODialog::printSuccess("Loaded " + std::to_string(demo.size()) + " demo keys.");
        std::cout << "\n  Heap structure:\n\n";
        heap->printHeap();
        IODialog::pressEnter();
    }

    void doExit() {
        IODialog::clearScreen();
        std::cout << "\nGoodbye!\n\n";
    }
};