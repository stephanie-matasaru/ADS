#include "DynamicLeaderboard.h"
#include "DynamicMedian.h"

#ifdef _WIN32
    #define CLEAR "cls"
#else
    #define CLEAR "clear"
#endif

using namespace std;

int main() {
    int choice;
    while (true) {
        system(CLEAR);
        cout << "\n=== Main Menu ===\n"
             << "1. Leaderboard\n"
             << "2. Median Finder\n"
             << "3. Exit\n"
             << "Enter choice (1-3): ";
        cin >> choice;
        cin.ignore(10000, '\n');

        if      (choice == 1) runLeaderboard();
        else if (choice == 2) runMedian();
        else if (choice == 3) { cout << "Bye!\n"; return 0; }
        else cout << "Unknown option.\n";
    }
}