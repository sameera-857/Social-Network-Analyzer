#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <ctime>

using namespace std;

void findFriendGroupDFS(int user, vector<vector<int>> &friendNetwork, vector<bool> &hasBeenVisited, vector<int> &currentGroup) {
    hasBeenVisited[user] = true;
    currentGroup.push_back(user);

    for (int friendId : friendNetwork[user]) {
        if (!hasBeenVisited[friendId]) {
            findFriendGroupDFS(friendId, friendNetwork, hasBeenVisited, currentGroup);
        }
    }
}

int main() {
    cout << "--- Social Network Analysis Engine Started ---" << endl;
    srand(time(0));
    int totalUsers = 30;
    vector<string> possibleInterests = {"Dancing", "Music", "Coding", "Politics", "Sports"};
    vector<string> userInterests(totalUsers);

    for (int i = 0; i < totalUsers; i++) {
        userInterests[i] = possibleInterests[rand() % possibleInterests.size()];   
    }


    vector<vector<int>> friendNetwork(totalUsers);
    
    for (int currentUser = 0; currentUser < totalUsers; currentUser++) {
        int targetFriendCount = rand() % 4 + 1;

        while (friendNetwork[currentUser].size() < targetFriendCount) {
            int randomFriend = rand() % totalUsers;

            bool isSelf = (randomFriend == currentUser);
            bool alreadyFriends = (find(friendNetwork[currentUser].begin(), friendNetwork[currentUser].end(), randomFriend) != friendNetwork[currentUser].end());

            if (!isSelf && !alreadyFriends) {
                friendNetwork[currentUser].push_back(randomFriend);
                friendNetwork[randomFriend].push_back(currentUser); 
            }
        }
    }

    int maxFriendsFound = -1;
    vector<int> mostPopularUsers;

    for(int i = 0; i < totalUsers; i++) {
        int friendCount = friendNetwork[i].size();
        if(friendCount > maxFriendsFound) {
            maxFriendsFound = friendCount;
        }
    }

    for (int i = 0; i < totalUsers; i++) {
        if (friendNetwork[i].size() == maxFriendsFound) {
            mostPopularUsers.push_back(i);
        }
    }

    cout << "\n[1] MOST POPULAR USERS (Degree Centrality)\n";
    for (int user : mostPopularUsers) {
        cout << "User " << user << " (" << userInterests[user] << ") has " << maxFriendsFound << " friends." << endl;
    }

    vector<bool> hasBeenVisited(totalUsers, false);
    cout << "\n[2] FRIEND GROUPS / COMMUNITIES (DFS)\n";

    for (int i = 0; i < totalUsers; i++) {
        if (!hasBeenVisited[i]) {
            vector<int> newGroup;
            findFriendGroupDFS(i, friendNetwork, hasBeenVisited, newGroup);

            cout << "Community: ";
            for (int member : newGroup) {
                cout << member << " ";
            }
            cout << endl;
        }
    }

    cout << "\n[3] ECHO CHAMBER DETECTION\n";

    for (int user = 0; user < totalUsers; user++) {
        int friendsWithSameInterest = 0;
        int totalFriends = friendNetwork[user].size();

        if (totalFriends == 0) continue;

        for (int friendId : friendNetwork[user]) {
            if (userInterests[friendId] == userInterests[user]) {
                friendsWithSameInterest++;
            }
        }

        double similarityRatio = (double)friendsWithSameInterest / totalFriends;

        // If 60% or more of their friends share the exact same interest
        if (similarityRatio >= 0.6) {
            cout << "Warning: User " << user << " (" << userInterests[user] << ") is in an Echo Chamber.\n";
        }
    }

    vector<double> pageRankScore(totalUsers, 1.0 / totalUsers); 
    vector<double> newScores(totalUsers);
    double dampeningFactor = 0.85; 

    for (int iteration = 0; iteration < 10; iteration++) {
        
        for (int i = 0; i < totalUsers; i++) {
            newScores[i] = (1 - dampeningFactor) / totalUsers;
        }

        for (int user = 0; user < totalUsers; user++) {
            for (int friendId : friendNetwork[user]) {
                if (friendNetwork[friendId].size() > 0) {
                    newScores[user] += dampeningFactor * (pageRankScore[friendId] / friendNetwork[friendId].size());
                }
            }
        }
        pageRankScore = newScores;
    }

    vector<pair<double, int>> rankedUsers;
    for (int i = 0; i < totalUsers; i++) {
        rankedUsers.push_back({pageRankScore[i], i});
    }
    sort(rankedUsers.rbegin(), rankedUsers.rend());

    cout << "\n[4] TRUE INFLUENCERS (PageRank Algorithm)\n";
    for (int i = 0; i < min(4, totalUsers); i++) { 
        int user = rankedUsers[i].second;
        double score = rankedUsers[i].first;
        cout << "User " << user << " (" << userInterests[user] << ") | Score: " << score << endl;
    }

    return 0;
}