#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include<cstdlib>
#include<ctime>
using namespace std;

void dfs(int u, vector<vector<int>> &adj, vector<bool> &visited, vector<int> &component) 
{
    visited[u] = true;
    component.push_back(u);

    for (int v : adj[u]) 
    {
        if (!visited[v]) 
        {
            dfs(v, adj, visited, component);
        }
    }
}
int main()
{
    cout<<"seed script running"<<endl;
    int N=30;
    vector<string>interests={"Dancing","Music","Coding","Politics","Sports"};
    vector<string>userInterest(N);
    srand(time(0));
    for(int i=0;i<N;i++)
    {
        userInterest[i] = interests[rand() % interests.size()];   
    }

    vector<vector<int>> adj(N);
    for (int u = 0; u < N; u++) 
    {
        int numFriends = rand() % 4 + 1;
        while (adj[u].size() < numFriends) 
        {
             int v = rand() % N;                  
             if (v != u && find(adj[u].begin(), adj[u].end(), v) == adj[u].end())  
            {
                adj[u].push_back(v);         
                adj[v].push_back(u);   
            }
        }
    }

//assigning interests randomly
    for (int i = 0; i < N; i++) 
    {
        cout << "User " << i << " (" << userInterest[i] << ") -> ";
        for (int v : adj[i]) 
        {
            cout << v << " ";
        }
        cout << endl;
    }

//degree centrality: finding influencers 

    vector<int> degree(N);
    int maxdegree=-1;
    for(int i=0;i<N;i++)
    {
        degree[i]=adj[i].size();
    }
    for(int i=0;i<N;i++)
    {
        if(degree[i]>maxdegree)
        {
            maxdegree=degree[i];
        }
    }
    
    vector<int> influencers;
    for (int i = 0; i < N; i++) 
    {
        if (degree[i] == maxdegree) 
        {
        influencers.push_back(i);
        }
    }
    cout << "\n=== Influencers (Degree Centrality) ===\n";

    for (int u : influencers) 
    {
        cout << "User " << u 
         << " (" << userInterest[u] << ") "
         << "Degree = " << maxdegree << endl;
    }
    
    //forming communities
    vector<bool> visited(N,false);
    cout << "\n=== Communities (DFS) ===\n";
    
    for (int i = 0; i < N; i++) 
    {
        if (!visited[i]) 
        {
            vector<int> component;
            dfs(i, adj, visited, component);
            cout << "Community: ";
            for (int u : component) 
            {
                cout << u << " ";
            }
            cout << endl;
        }
    }
    //echo chambers
    cout << "\n=== Echo Chambers ===\n";
    
    for (int u = 0; u < N; u++) 
    {
        int same = 0;
        int total = adj[u].size();
        
        if (total == 0) continue;
        
        for (int v : adj[u])
        {
            if (userInterest[v] == userInterest[u]) 
            {
                same++;
            }
        }
        double ratio = (double)same / total;
        if (ratio >= 0.6) 
        {
            cout << "User " << u 
             << " (" << userInterest[u] << ") "
             << "is in an Echo Chamber\n";
        }
    }
    
    //page rank:infulential users
    vector<double> pr(N, 1.0 / N);
    vector<double> new_pr(N);
    
    double d = 0.85;
    int iterations = 10;
    
    for (int it = 0; it < iterations; it++) 
    {
        for (int u = 0; u < N; u++) 
        {
            new_pr[u] = (1 - d) / N;
        }
        for (int u = 0; u < N; u++) 
        {
            for (int v : adj[u]) 
            {
                if (adj[v].size() > 0) 
                {
                     new_pr[u] += d * (pr[v] / adj[v].size());
                }
            }
        }
        pr = new_pr;
    }
    vector<pair<double, int>> ranking;
    for (int i = 0; i < N; i++) 
    {
        ranking.push_back({pr[i], i});
    }
    
    sort(ranking.rbegin(), ranking.rend());   //sorting in descending order 
    
    cout << "\n=== Top Influencers (PageRank) ===\n";
    
    int topK = min(4, N);  // print up to 4 users
    for (int i = 0; i < topK; i++) 
    {
        int user = ranking[i].second;
        double score = ranking[i].first;
        cout << "User " << user 
         << " (" << userInterest[user] << ") "
         << "PR = " << score << endl;
    }
    return 0;
}
