#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <set>

using namespace std;

struct UserNode {
    int id;
    string name;
    string interest;
};


// ALGORITHM: Depth First Search (Groups friends together)
void findCommunityDFS(int userId, const vector<set<int>>& friendNetwork, vector<bool>& visited, vector<int>& cluster) {
    visited[userId] = true;
    cluster.push_back(userId);
    for (int friendId : friendNetwork[userId]) {
        if (!visited[friendId]) {
            findCommunityDFS(friendId, friendNetwork, visited, cluster);
        }
    }
}

int main() {
    srand(time(0));

    // --- CAPTURE USER DATA ---
    cout << "[SYSTEM] Starting User Authentication...\n";
    cout << "Enter your Username: ";
    string myName;
    cin >> myName;

    cout << "Select Interest (Tech, Sports, Art, Music): ";
    string myInterest;
    cin >> myInterest;

    cout << "\n[SYSTEM] Connecting to Network...\n";
    cout << "Who would you like to friend? (0=Sameera, 1=Mayank, 2=Aanya, 3=Rohan)\n";
    cout << "Enter two IDs separated by space: ";
    int f1, f2;
    cin >> f1 >> f2;

    string friendNames[] = {"Sameera", "Mayank", "Aanya", "Rohan"};
    string friendInterests[] = {"Tech", "Sports", "Art", "Music"};

    int totalUsers = 15;
    vector<UserNode> nodes;
    vector<set<int>> friendNetwork(totalUsers);

    nodes.push_back({0, myName + " (You)", myInterest});

    nodes.push_back({1, friendNames[f1], friendInterests[f1]});
    nodes.push_back({2, friendNames[f2], friendInterests[f2]});

    string dummyNames[] = {"Sneha8", "Rahul9", "Kavya10", "Amit11", "Diya12", "Siddharth13", "Meera14", "Karan7"};
    string allInterests[] = {"Tech", "Sports", "Art", "Music"};
    
    for (int i = 3; i < totalUsers; i++) {
        nodes.push_back({i, dummyNames[i % 8], allInterests[i % 4]});
    }

    friendNetwork[0].insert(1); friendNetwork[1].insert(0);
    friendNetwork[0].insert(2); friendNetwork[2].insert(0);

    int edgesAdded = 0;
    while (edgesAdded < 18) {
        int a = rand() % totalUsers;
        int b = rand() % totalUsers;
        if (a != b && friendNetwork[a].find(b) == friendNetwork[a].end()) {
            friendNetwork[a].insert(b);
            friendNetwork[b].insert(a);
            edgesAdded++;
        }
    }

    cout << ">> 1. ADJACENCY LIST (Graph Memory Representation)\n";
    for (int i = 0; i < 5; i++) { // Print first 5 so it's not overwhelming
        cout << "   " << nodes[i].name << " is connected to -> [ ";
        for (int neighbor : friendNetwork[i]) {
            cout << nodes[neighbor].name << ", ";
        }
        cout << "\b\b ]\n";
    }
    cout << "   ... (remaining nodes omitted for clarity)\n\n";

    cout << ">> 2. DEGREE CENTRALITY (Finding Top Influencers)\n";
    vector<pair<int, int>> popularityRank; 
    for (int i = 0; i < totalUsers; i++) {
        popularityRank.push_back({friendNetwork[i].size(), i});
    }
    sort(popularityRank.rbegin(), popularityRank.rend()); 

    for (int i = 0; i < 3; i++) {
        int id = popularityRank[i].second;
        cout << "   Top #" << i + 1 << ": " << nodes[id].name 
             << " with " << popularityRank[i].first << " connections.\n";
    }
    cout << "\n";

    cout << ">> 3. COMMUNITY DETECTION (Using Depth First Search)\n";
    vector<bool> visited(totalUsers, false);
    int clusterNumber = 1;
    for (int i = 0; i < totalUsers; i++) {
        if (!visited[i]) {
            vector<int> cluster;
            findCommunityDFS(i, friendNetwork, visited, cluster);
            
            cout << "   Community " << clusterNumber++ << ": ";
            for (int memberId : cluster) {
                cout << nodes[memberId].name << ", ";
            }
            cout << "\b\b \n"; 
        }
    }
    cout << "\n";


    cout << ">> 4. ECHO CHAMBER WARNINGS (Homophily Analysis)\n";
    bool warningFound = false;
    for (int i = 0; i < totalUsers; i++) {
        if (friendNetwork[i].empty()) continue;

        int friendsWithSameInterest = 0;
        for (int friendId : friendNetwork[i]) {
            if (nodes[friendId].interest == nodes[i].interest) {
                friendsWithSameInterest++;
            }
        }

        double ratio = (double)friendsWithSameInterest / friendNetwork[i].size();
        
        // If 60% or more of your friends have the exact same interest, flag it
        if (ratio >= 0.6) {
            cout << "   [WARNING] " << nodes[i].name << " is in an Echo Chamber! (" 
                 << (int)(ratio * 100) << "% of friends are strictly into " << nodes[i].interest << ").\n";
            warningFound = true;
        }
    }
    if (!warningFound) cout << "   Network is diverse. No severe echo chambers detected.\n";


    return 0;
}