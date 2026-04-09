#include<iostream>
#include<queue>
#include<vector>
using namespace std;
int main(){
    int n,e,u,w,v;
    cin>>n>>e;
    vector<vector<int>>adj(n);
    vector<int>vis(n,0);
    queue<int>q;
    while(e--){
        cin>>u>>v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    int s;
    cin>>s;
    q.push(s);
    while(!q.empty()){
        int x=q.front();
        q.pop();
        cout<<x<<" ";
        for(int w : adj[x]){
            if(!vis[w]){
                vis[w]=1;
                q.push(w);
            }
        }
    }
    return 0;
}