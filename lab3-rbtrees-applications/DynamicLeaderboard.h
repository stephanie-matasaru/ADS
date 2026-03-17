#ifndef DYNAMICLEADERBOARD_H_
#define DYNAMICLEADERBOARD_H_

#include "RBNode.h"
#include <map>
#include <string>
#include <fstream>

#ifdef _WIN32
    #define CLEAR "cls"
#else
    #define CLEAR "clear"
#endif

using namespace std;

class Leaderboard {
    RBTree tree;
    map<string, int> playerKey;
    int tiebreaker = 0;

    int encodeKey(int score) {
        return (score << 12) | (tiebreaker++ & 0xFFF);
    }

public:
    void add(const string& player, int score) {
        int k = encodeKey(score);
        playerKey[player] = k;
        tree.RBInsert(tree.createNode(k, player));
    }

    void update(const string& player, int delta) {
        int oldKey = playerKey[player];
        RBNode* node = tree.search(tree.root, oldKey);
        if (!tree.isNil(node)) {
            int newScore = (oldKey >> 12) + delta;
            tree.del(node);
            int newKey = encodeKey(newScore);
            playerKey[player] = newKey;
            tree.RBInsert(tree.createNode(newKey, player));
        }
    }

    void remove(const string& player) {
        int k = playerKey[player];
        RBNode* node = tree.search(tree.root, k);
        if (!tree.isNil(node)) tree.del(node);
        playerKey.erase(player);
    }

    void top(int k) {
        RBNode* curr = tree.maximum(tree.root);
        for (int i = 0; i < k && !tree.isNil(curr); i++) {
            cout << curr->name << " " << (curr->key >> 12) << "\n";
            curr = tree.predecessor(curr);
        }
    }

    void saveToFile(const string& filename) {
        ofstream f(filename);
        if (!f) { cout << "Could not open file.\n"; return; }
        for (auto& pair : playerKey) {
            f << pair.first << " " << (pair.second >> 12) << "\n";
        }
        cout << "Saved to " << filename << "\n";
    }

    void loadFromFile(const string& filename) {
        ifstream f(filename);
        if (!f) { cout << "Could not open file.\n"; return; }
        string player; int score;
        while (f >> player >> score)
            add(player, score);
        cout << "Loaded from " << filename << "\n";
    }
};

void runLeaderboard() {
    Leaderboard lb;
    lb.loadFromFile("leaderboard.txt");   
    int choice;
    while (true) {
        system(CLEAR);
        cout << "\n=== Leaderboard ===\n"
             << "1. Add player\n"
             << "2. Update score\n"
             << "3. Remove player\n"
             << "4. Top k players\n"
             << "5. Exit\n"                
             << "Enter choice (1-5): ";
        cin >> choice;
        cin.ignore(10000, '\n');

        if (choice == 1) {
            string player; int score;
            cout << "Player name: "; cin >> player;
            cout << "Score: ";       cin >> score;
            lb.add(player, score);
            cout << "Added " << player << " with score " << score << "\n";

        } else if (choice == 2) {
            string player; int delta;
            cout << "Player name: "; cin >> player;
            cout << "Delta (+/-): "; cin >> delta;
            lb.update(player, delta);
            cout << "Updated.\n";

        } else if (choice == 3) {
            string player;
            cout << "Player name: "; cin >> player;
            lb.remove(player);
            cout << "Removed.\n";

        } else if (choice == 4) {
            int k;
            cout << "Show top how many? "; cin >> k;
            cin.ignore(10000, '\n');
            lb.top(k);
            cout << "\nPress Enter to continue...";
            cin.ignore(10000, '\n');
            continue; 

        } else if (choice == 5) {
            lb.saveToFile("leaderboard.txt");   
            break;
        } else {
            cout << "Unknown option.\n";
        }
        cout << "Press Enter to continue...";
        cin.ignore(10000, '\n');
    }
}

#endif