#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <limits>
using namespace std;

struct UF {
    vector<int> parent, rank_, size_;

    UF(int n) : parent(n), rank_(n, 0), size_(n, 1) {
        for (int i = 0; i < n; i++) parent[i] = i;
    }

    int find(int x) {
        if (parent[x] != x)
            parent[x] = find(parent[x]);
        return parent[x];
    }

    bool unite(int x, int y) {
        int rx = find(x), ry = find(y);
        if (rx == ry) return false;
        if (rank_[rx] < rank_[ry]) swap(rx, ry);
        parent[ry] = rx;
        size_[rx] += size_[ry];
        if (rank_[rx] == rank_[ry]) rank_[rx]++;
        return true;
    }
};

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pause() {
    cout << "\nPress Enter to return to menu...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void printHeader(const string& title) {
    cout << "\n==========================================\n";
    cout << "  " << title << "\n";
    cout << "==========================================\n\n";
}

vector<pair<int,int>> readEdges() {
    int m;
    cout << "Enter number of edges: ";
    cin >> m;
    vector<pair<int,int>> edges(m);
    cout << "Enter edges (u v) one per line:\n";
    for (int i = 0; i < m; i++) {
        cout << "  Edge " << i + 1 << ": ";
        cin >> edges[i].first >> edges[i].second;
    }
    return edges;
}

void runConnectedComponents() {
    clearScreen();
    printHeader("Problem 1: Connected Components");

    int n;
    cout << "Enter number of nodes: ";
    cin >> n;

    vector<pair<int,int>> edges = readEdges();

    UF uf(n);
    cout << "\n--- Processing edges ---\n";
    for (auto& [u, v] : edges) {
        int ru = uf.find(u), rv = uf.find(v);
        if (uf.unite(u, v))
            cout << "  Edge (" << u << "," << v << "): merged components " << ru << " and " << rv << "\n";
        else
            cout << "  Edge (" << u << "," << v << "): already in same component (root " << ru << ")\n";
    }

    unordered_map<int, vector<int>> comps;
    for (int i = 0; i < n; i++)
        comps[uf.find(i)].push_back(i);

    cout << "\n--- Result ---\n";
    cout << "Connected components: " << comps.size() << "\n";
    int idx = 1;
    for (auto& [root, members] : comps) {
        cout << "  Component " << idx++ << ": { ";
        for (int x : members) cout << x << " ";
        cout << "}\n";
    }

    pause();
}

void runCycleDetection() {
    clearScreen();
    printHeader("Problem 2: Cycle Detection");

    int n;
    cout << "Enter number of nodes: ";
    cin >> n;

    vector<pair<int,int>> edges = readEdges();

    UF uf(n);
    bool cycleFound = false;
    pair<int,int> cycleEdge;

    cout << "\n--- Processing edges ---\n";
    for (auto& [u, v] : edges) {
        if (uf.find(u) == uf.find(v)) {
            cout << "  Edge (" << u << "," << v << "): CYCLE DETECTED - both in same component!\n";
            cycleFound = true;
            cycleEdge = {u, v};
            break;
        }
        uf.unite(u, v);
        cout << "  Edge (" << u << "," << v << "): merged - no cycle yet\n";
    }

    cout << "\n--- Result ---\n";
    if (cycleFound)
        cout << "  Graph HAS a cycle (detected at edge " << cycleEdge.first << " - " << cycleEdge.second << ")\n";
    else
        cout << "  Graph has NO cycle\n";

    pause();
}

void runProvinces() {
    clearScreen();
    printHeader("Problem 3: Number of Provinces");

    int n;
    cout << "Enter number of cities (matrix size n x n): ";
    cin >> n;

    vector<vector<int>> matrix(n, vector<int>(n));
    cout << "Enter the " << n << "x" << n << " isConnected matrix row by row:\n";
    for (int i = 0; i < n; i++) {
        cout << "  Row " << i << ": ";
        for (int j = 0; j < n; j++)
            cin >> matrix[i][j];
    }

    UF uf(n);
    cout << "\n--- Processing matrix ---\n";
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            if (matrix[i][j] == 1)
                if (uf.unite(i, j))
                    cout << "  City " << i << " and city " << j << " connected - merged\n";

    unordered_map<int, vector<int>> provinces;
    for (int i = 0; i < n; i++)
        provinces[uf.find(i)].push_back(i);

    cout << "\n--- Result ---\n";
    cout << "Number of provinces: " << provinces.size() << "\n";
    int idx = 1;
    for (auto& [root, members] : provinces) {
        cout << "  Province " << idx++ << ": { ";
        for (int x : members) cout << x << " ";
        cout << "}\n";
    }

    pause();
}

void runFriendGroups() {
    clearScreen();
    printHeader("Problem 4: Friend Groups");

    int n;
    cout << "Enter number of users: ";
    cin >> n;

    vector<pair<int,int>> friendships = readEdges();

    UF uf(n);
    cout << "\n--- Processing friendships ---\n";
    for (auto& [u, v] : friendships) {
        if (uf.unite(u, v))
            cout << "  (" << u << "," << v << "): merged into same group\n";
        else
            cout << "  (" << u << "," << v << "): already in same group\n";
    }

    unordered_map<int, vector<int>> groupMap;
    for (int i = 0; i < n; i++)
        groupMap[uf.find(i)].push_back(i);

    cout << "\n--- Result ---\n";
    cout << "Total friend groups: " << groupMap.size() << "\n";
    int idx = 1;
    for (auto& [root, members] : groupMap) {
        cout << "  Group " << idx++ << " (size " << members.size() << "): { ";
        for (int x : members) cout << x << " ";
        cout << "}\n";
    }

    pause();
}

void printMenu() {
    clearScreen();
    cout << "\n==========================================\n";
    cout << "       Union-Find Problem Suite\n";
    cout << "==========================================\n";
    cout << "  1.  Connected Components\n";
    cout << "  2.  Cycle Detection\n";
    cout << "  3.  Number of Provinces\n";
    cout << "  4.  Friend Groups\n";
    cout << "  0.  Exit\n";
    cout << "==========================================\n";
    cout << "\nChoice: ";
}

int main() {
    int choice;
    do {
        printMenu();
        cin >> choice;
        switch (choice) {
            case 1: runConnectedComponents(); break;
            case 2: runCycleDetection();      break;
            case 3: runProvinces();           break;
            case 4: runFriendGroups();        break;
            case 0: clearScreen(); cout << "Goodbye!\n\n"; break;
            default: cout << "Invalid option. Try again.\n"; break;
        }
    } while (choice != 0);

    return 0;
}