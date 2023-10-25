#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <algorithm>
#include <time.h>

using namespace std;
vector<int> bfs(vector<vector<int>>& adj_matrix, int s) {
    int n = adj_matrix.size();
    vector<bool> visited(n, false);
    queue<int> q;

    visited[s] = true;
    q.push(s);
    vector<int>bfs_order;
    
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        
        //cout << u << " ";
        bfs_order.push_back(u);
        
        for (int v = 0; v < n; v++) {
            if (adj_matrix[u][v]==1 && !visited[v]) {
                visited[v] = true;
                q.push(v);
            }
        }
    }
    return bfs_order;
}

const int INF = 1e9; // initialize INF to a large value to represent infinity

int main() {
    int n, m; // n = number of nodes, m = number of edges
    cout<<"\n\n\nEnter the number of vertices and edges\n";// and then \nwtite the edge's starting and ending points \n";
    cin >> n >> m;
    cout<<"Enter the starting and ending points of all the edges\n";
    
    vector<vector<int>> adj(n, vector<int>(n, INF)); // initialize adjacency matrix with all entries set to INF
    
    for (int i = 0; i < m; i++) {
        int u, v; // starting node, ending node
        cin >> u >> v;
        
        adj[u][v] = 1; 
               // adj[u].push_back(v);

    }
    vector<int> commlink;
    int flag;
    for(int i=0;i<n;i++)
    {
        flag=0;

        for(int j=0;j<n;j++)
        {
            if(adj[j][i]==1)
            {
                //commlink.push_back(make_pair(j,i));
                flag=1;
            }
        }
        if(flag==0)
        {
            commlink.push_back(i);
        }
    }
    vector<vector<int>>bfsmatrix(commlink.size());
        //int size = commlink.size();
        for(int i=0;i<commlink.size();i++)
        {
            bfsmatrix[i]=bfs(adj,commlink[i]);
        }
        
        vector<vector<pair<int, int>>> pairs(bfsmatrix.size());
        for(int i=0;i<bfsmatrix.size();i++)
        {
            for(int j=0;j<bfsmatrix[i].size()-1;j++)
            {
                pairs[i].push_back(make_pair(bfsmatrix[i][j], bfsmatrix[i][j+1]));
                
            }           
        }
    
    // Floyd-Warshall algorithm to find all-pairs shortest paths
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                adj[i][j] = min(adj[i][j], adj[i][k] + adj[k][j]);
            }
        }
    }
    
    int max_dist = -1; // initialize max distance to -1 (invalid value)
    
    // find maximum distance between two nodes
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (adj[i][j] != INF) { // if there is a path between nodes i and j
                max_dist = max(max_dist, adj[i][j]);
            }
        }
    }
    
    cout << "Maximum distance between two nodes: " << max_dist << endl;
    cout<<"number of nodes with indegree = 0: "<<commlink.size()<<endl;

    time_t ti;
    srand((unsigned) time(&ti));

    vector<vector<vector<pair<int,int>>>> updated_pairs(0, vector<vector<pair<int,int>>>(max_dist));

    for(int i=0;i<updated_pairs.size();i++)
    {
        cout << updated_pairs[i].size() << endl;
    }

    for(int t=0;t<max_dist;t++)
    {
        int num_rows = 0;
        map<int,int> dest_row_mapping;
        for(int c=0;c<commlink.size();c++)
        {
            if(t<pairs[c].size())
            {
                int dest = pairs[c][t].second;
                int updated_row;
                if(dest_row_mapping.find(dest) == dest_row_mapping.end())
                {
                    if(t==0)
                    {
                        updated_row = updated_pairs.size();
                        vector<vector<pair<int,int>>> new_row(max_dist);
                        updated_pairs.push_back(new_row);
                        dest_row_mapping[dest] = updated_row;
                    }
                    else{
                        updated_row = num_rows;
                        dest_row_mapping[dest] = updated_row;
                        num_rows++;
                    }
                }
                else
                {
                    updated_row = dest_row_mapping[dest];
                }
                updated_pairs[updated_row][t].push_back(pairs[c][t]);
            }
        }
    }

    for(int i=0;i<updated_pairs.size();i++)
    {
        for(int j=0;j<updated_pairs[i].size();j++)
        {
            sort(updated_pairs[i][j].begin(), updated_pairs[i][j].end());

            auto duplicatePredicate = [](const auto& a, const auto& b) {
                return a.first == b.first && a.second == b.second;
            };
            auto it = unique(updated_pairs[i][j].begin(), updated_pairs[i][j].end(), duplicatePredicate);

            updated_pairs[i][j].erase(it, updated_pairs[i][j].end());
        }
    }

    vector<vector<vector<pair<int,int>>>> timetable(updated_pairs.size(), vector<vector<pair<int,int>>>(max_dist));
    for(int t=0;t<max_dist;t++)
    {
        vector<int> slots(updated_pairs.size());
        for(int i=0;i<updated_pairs.size();i++)
        {
            slots[i]=i;
        }
        for(int c=0;c<updated_pairs.size();c++)
        {
            if(t<updated_pairs[c].size())
            {
                int rand_value=rand()%slots.size();
                int current_channel=slots[rand_value];
                slots.erase(slots.begin()+rand_value);
                timetable[current_channel][t]=updated_pairs[c][t];
            }
        }
    }

    for(int i=0;i<timetable.size();i++)
    {
        for(int j=0;j<timetable[i].size();j++)
        {
            for(int k=0;k<timetable[i][j].size();k++)
            {
                cout << timetable[i][j][k].first << "-->" << timetable[i][j][k].second << " | ";
            }
            cout << "                ";
        }
        cout << endl;
        cout << endl;
    }
    cout << endl;  
    
    return 0;
}
