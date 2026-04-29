#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include<cstdlib>
#include<ctime>
#include<fstream>
#include<random>
using namespace std;


vector<string> names = {
    "Sameera","Rohan","Aanya","Mayank","Diya",
    "Karan","Sneha","Rahul","Kavya","Siddharth",
    "Meera","Amit","Arjun","Priya","Neha",
    "Vikram","Isha","Ankit","Pooja","Raj",
    "Simran","Varun","Nisha","Dev"
};

void saveToDB(vector<vector<int>> &adj, vector<string> &userInterest)
{
    ofstream file("C:/xampp/htdocs/Social Network Analyzer/data.sql");

    file << "DELETE FROM nodes;\n";
    file << "DELETE FROM edges;\n\n";

    for(int i = 0; i < userInterest.size(); i++)
    {
        string finalName = (i < names.size())
            ? names[i]
            : names[i % names.size()] + to_string(i);

        file << "INSERT INTO nodes (id, interest, name) VALUES ("
             << i << ", '" << userInterest[i] << "', '"
             << finalName << "');\n";
    }

    file << "\n";

    for(int u = 0; u < adj.size(); u++)
    {
        for(int v : adj[u])
        {
            if(u < v)
            {
                file << "INSERT INTO edges (src, dest) VALUES ("
                     << u << ", " << v << ");\n";
            }
        }
    }

    file.close();
}

void dfs(int u, vector<vector<int>> &adj, vector<bool> &visited, vector<int> &component)
{
    visited[u] = true;
    component.push_back(u);

    for(int v : adj[u])
    {
        if(!visited[v])
            dfs(v, adj, visited, component);
    }
}

int main()
{
    cout << "seed script running\n";

    int N = 12;

    random_device rd;
    mt19937 g(rd());
    shuffle(names.begin(), names.end(), g);

    vector<string> interests = {"Dancing","Music","Coding","Politics","Sports"};
    vector<string> userInterest(N);

    srand(time(0));
    for(int i=0;i<N;i++)
        userInterest[i] = interests[rand() % interests.size()];

    vector<vector<int>> adj(N);

    for(int u=0;u<N;u++)
    {
        int numFriends = rand()%2 + 1;
        while(adj[u].size() < numFriends)
        {
            int v = rand()%N;
            if(v!=u && find(adj[u].begin(), adj[u].end(), v)==adj[u].end())
            {
                adj[u].push_back(v);
                adj[v].push_back(u);
            }
        }
    }

    // PRINT GRAPH
    for(int i=0;i<N;i++)
    {
        cout << names[i] << " (" << userInterest[i] << ") -> ";
        for(int v: adj[i]) cout << names[v] << " ";
        cout << endl;
    }

    // DEGREE CENTRALITY
    vector<int> degree(N);
    int maxdegree = -1;

    for(int i=0;i<N;i++)
    {
        degree[i] = adj[i].size();
        maxdegree = max(maxdegree, degree[i]);
    }

    cout << "\n=== Influencers (Degree) ===\n";
    for(int i=0;i<N;i++)
    {
        if(degree[i]==maxdegree)
            cout << names[i] << " (" << userInterest[i] << ") Degree=" << maxdegree << endl;
    }

    // DFS COMMUNITIES
    vector<bool> visited(N,false);

    cout << "\n=== Communities ===\n";
    for(int i=0;i<N;i++)
    {
        if(!visited[i])
        {
            vector<int> comp;
            dfs(i, adj, visited, comp);

            cout << "Community: ";
            for(int u: comp) cout << names[u] << " ";
            cout << endl;
        }
    }

    // ECHO CHAMBERS
    cout << "\n=== Echo Chambers ===\n";
    for(int u=0;u<N;u++)
    {
        int same=0, total=adj[u].size();
        if(total==0) continue;

        for(int v: adj[u])
            if(userInterest[v]==userInterest[u]) same++;

        if((double)same/total >= 0.6)
            cout << names[u] << " is in an Echo Chamber\n";
    }

    // PAGERANK
    vector<double> pr(N,1.0/N), new_pr(N);
    double d=0.85;

    for(int it=0;it<10;it++)
    {
        fill(new_pr.begin(), new_pr.end(), (1-d)/N);

        for(int u=0;u<N;u++)
        {
            for(int v: adj[u])
            {
                if(adj[v].size()>0)
                    new_pr[u] += d * (pr[v]/adj[v].size());
            }
        }
        pr = new_pr;
    }

    vector<pair<double,int>> ranking;
    for(int i=0;i<N;i++)
        ranking.push_back({pr[i], i});

    sort(ranking.rbegin(), ranking.rend());

    cout << "\n=== Top Influencers (PageRank) ===\n";
    for(int i=0;i<min(4,N);i++)
    {
        int u = ranking[i].second;
        cout << names[u] << " (" << userInterest[u] << ") PR=" << ranking[i].first << endl;
    }

    saveToDB(adj, userInterest);
}