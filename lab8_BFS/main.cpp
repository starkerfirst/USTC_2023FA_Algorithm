#include<iostream>
#include<fstream>
#include<queue>
#include<vector>
#include<cassert>
#include<utility>
#include<string>
#include<cmath>
#include<algorithm>
#include<chrono>
#include<map>
using namespace std;

struct Node{
    int index;
    int color; // 0: white, 1: gray, 2: black
    int d;  // distance
    int pred; // predecessor
};

Node* find_node(vector<Node> &nodes, int index){
    for (Node &node : nodes){
        if (node.index == index){
            return &node;
        }
    }
    return nullptr;
}

// return first non-visited node
int find_white(map<int, Node> &nodes){
    for (auto node: nodes){
        if (node.second.color == 0){
            return node.first;
        }
    }
    return -1;
}

// BFS
void BFS_matrix(int **graph, int n, vector<Node> &nodes){
    queue<int> q;
    q.push(0);
    while (!q.empty()){
        int u = q.front();
        q.pop();
        cout << char(u + 'A') << "->";
        for (int v = 0; v < n; v++){
            if (graph[u][v] == 1 && find_node(nodes, v)->color == 0){
                q.push(v);
                find_node(nodes, v)->color = 1;
                find_node(nodes, v)->d = find_node(nodes, u)->d + 1;
                find_node(nodes, v)->pred = u;
            }
        }
        find_node(nodes, u)->color = 2;
    }
    cout << endl;
}

void BFS_list(map<int,vector<int>> &graph, int n, map<int, Node> &nodes){
    int counter = 0; // node counter
    queue<int> q;
    chrono::steady_clock::time_point t1 = chrono::steady_clock::now();
    while (find_white(nodes) != -1){
        // the graph is not connected
        counter++;
        q.push(find_white(nodes));
        while (!q.empty()){
            int u = q.front();
            q.pop();
            nodes[u].color = 1;
            for (int v : graph[u]){
                if (nodes[v].color == 0){
                    q.push(v);
                    nodes[v].color = 1;
                    nodes[v].d = nodes[u].d + 1;
                    nodes[v].pred = u;
                    counter++;
                }
            }         
            nodes[u].color = 2;
        }
    }
    chrono::steady_clock::time_point t2 = chrono::steady_clock::now();
    cout << "Time difference = " << chrono::duration_cast<chrono::milliseconds>(t2 - t1).count() << "(ms)" << endl;
    cout << "Nodes visited: " << counter << endl;
}

// Breadth First Search, using adjacency matrix and adjacency list
int main(){
    // test1: adjacency matrix, non-directed graph
    cout << "test1" << endl;
    ifstream fin("data.txt");
    if (!fin.is_open()){
        cout << "Error opening file" << endl;
        return 0;
    }
    
    // initialize the graph
    string line;
    getline(fin, line);
    int n = (line.size() + 1) / 2;
    int **graph = new int*[n];
    vector<Node> nodes(n); // node info
    for (int i = 0; i < n; i++){
        graph[i] = new int[n];
        nodes.push_back({i, 0, 0, -1});
    }
    while (!fin.eof()){
        getline(fin, line);
        int u = line[0] - 'A'; // smart indexing
        int v = line[2] - 'A';
        graph[u][v] = 1;
        graph[v][u] = 1;
    }

    fin.close();

    BFS_matrix(graph, n, nodes);

    for (int i = 0; i < n; i++){
        delete[] graph[i];
    }
    delete[] graph;
    nodes.clear();


    // test2: adjacency list, directed graph, Nodes 81306, Edges 1768149
    cout << "test2" << endl;
    fin.open("twitter_small.txt");
    if (!fin.is_open()){
        cout << "Error opening file" << endl;
        return 0;
    }
    int u, v;

    // initialize the graph (map is much faster than vector)
    map<int, vector<int>> graph_list; // adjacency list, first row element is u
    map<int, Node> nodes_list; // node info
    while (!fin.eof()){
        fin >> u >> v;
        graph_list[u].push_back(v);
        nodes_list.insert({u, {u, 0, 0, -1}});
        nodes_list.insert({v, {v, 0, 0, -1}});
    }
    fin.close();

    BFS_list(graph_list, graph_list.size(), nodes_list);
    
    return 0;
}