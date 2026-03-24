#pragma once
#include <iostream>
#include <string>
#include <limits>
#include <sstream>
#include <iomanip>
#include <vector>

namespace IODialog {

    inline void clearScreen() {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    }

    inline void pressEnter() {
        std::cout << "\nPress ENTER to continue...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
    }

    inline int readInt(const std::string& prompt) {
        int val;
        while (true) {
            std::cout << prompt;
            if (std::cin >> val) {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return val;
            }
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter an integer.\n";
        }
    }

    inline int readDegree() {
        int t;
        while (true) {
            std::cout << "Enter minimum degree t (t >= 2, recommended 2-5): ";
            if (std::cin >> t && t >= 2) {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return t;
            }
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "t must be an integer >= 2. Try again.\n";
        }
    }

    inline std::vector<int> readIntList(const std::string& prompt) {
        std::cout << prompt;
        std::string line;
        std::getline(std::cin, line);
        std::istringstream iss(line);
        std::vector<int> values;
        int v;
        while (iss >> v) values.push_back(v);
        return values;
    }

    inline char readChar(const std::string& prompt) {
        std::cout << prompt;
        std::string line;
        std::getline(std::cin, line);
        return line.empty() ? '\0' : line[0];
    }

    inline int readMenuChoice(const std::string& prompt) {
        std::cout << prompt;
        std::string line;
        std::getline(std::cin, line);
        if (line.empty()) return -1;
        try { return std::stoi(line); } catch (...) { return -1; }
    }

    inline void printWelcomeBanner() {
        std::cout << "================================\n";
        std::cout << "    B-TREE INTERACTIVE CLI      \n";
        std::cout << "================================\n\n";
    }

    inline void printHeader(int t) {
        std::cout << "================================\n";
        std::cout << "          B-TREE MENU           \n";
        std::cout << "================================\n";
        std::cout << "  t=" << t
                  << "  max keys=" << (2*t-1)
                  << "  min keys=" << (t-1) << "\n\n";
    }

    inline void printMenu() {
        std::cout << "  [1] Insert key\n";
        std::cout << "  [2] Insert multiple keys\n";
        std::cout << "  [3] Delete key\n";
        std::cout << "  [4] Search key\n";
        std::cout << "  [5] Print tree\n";
        std::cout << "  [6] In-order traversal\n";
        std::cout << "  [7] Stats\n";
        std::cout << "  [8] Clear tree\n";
        std::cout << "  [9] Load demo data\n";
        std::cout << "  [0] Exit\n";
        std::cout << "--------------------------------\n";
    }

    inline void printSuccess(const std::string& msg) {
        std::cout << "OK: " << msg << "\n";
    }

    inline void printError(const std::string& msg) {
        std::cout << "ERROR: " << msg << "\n";
    }

    inline void printInfo(const std::string& msg) {
        std::cout << msg << "\n";
    }

    inline void printStats(int size, int height, int minVal, int maxVal, int t) {
        std::cout << "--------------------------------\n";
        std::cout << "  Total keys : " << size   << "\n";
        std::cout << "  Height     : " << height << "\n";
        std::cout << "  Minimum    : " << minVal << "\n";
        std::cout << "  Maximum    : " << maxVal << "\n";
        std::cout << "  Degree (t) : " << t      << "\n";
        std::cout << "--------------------------------\n";
    }

    inline void printTraversal(const std::vector<int>& keys) {
        if (keys.empty()) {
            std::cout << "Tree is empty.\n";
            return;
        }
        std::cout << "Sorted keys:\n  ";
        for (int i = 0; i < (int)keys.size(); ++i) {
            std::cout << keys[i];
            if (i + 1 < (int)keys.size()) std::cout << " -> ";
            if ((i + 1) % 10 == 0 && i + 1 < (int)keys.size())
                std::cout << "\n  ";
        }
        std::cout << "\n";
    }

} 