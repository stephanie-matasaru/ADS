#pragma once
#include <iostream>
#include <string>
#include <limits>
#include <sstream>
#include <vector>

namespace IODialog {

    // ── Screen ────────────────────────────────────────────────
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

    // ── Input ─────────────────────────────────────────────────
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

    // ── Banners ───────────────────────────────────────────────
    inline void printWelcomeBanner() {
        std::cout << "================================\n";
        std::cout << "  BINOMIAL HEAP INTERACTIVE CLI \n";
        std::cout << "================================\n\n";
    }

    inline void printHeader(int size) {
        std::cout << "================================\n";
        std::cout << "      BINOMIAL HEAP MENU        \n";
        std::cout << "================================\n";
        std::cout << "  nodes = " << size << "\n\n";
    }

    inline void printMenu() {
        std::cout << "  [1] Insert key\n";
        std::cout << "  [2] Insert multiple keys\n";
        std::cout << "  [3] Find minimum\n";
        std::cout << "  [4] Extract minimum\n";
        std::cout << "  [5] Decrease key\n";
        std::cout << "  [6] Delete node by key\n";
        std::cout << "  [7] Print heap (full tree view)\n";
        std::cout << "  [8] Print root list\n";
        std::cout << "  [9] Load demo data\n";
        std::cout << "  [0] Exit\n";
        std::cout << "--------------------------------\n";
    }

    // ── Output ────────────────────────────────────────────────
    inline void printSuccess(const std::string& msg) {
        std::cout << "OK: " << msg << "\n";
    }

    inline void printError(const std::string& msg) {
        std::cout << "ERROR: " << msg << "\n";
    }

    inline void printInfo(const std::string& msg) {
        std::cout << msg << "\n";
    }

} // namespace IODialog