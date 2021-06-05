/*
    Create two partitions by duplicating the original graph’s vertex set and adding an edge from a vertex 𝑣 in a partition to
    vertex 𝑢 in the other partition if there is an edge from 𝑣 to 𝑢 in the original graph.

    Usage example: ./preprocess ../data/wiki-Vote.txt ../data/wiki-Vote-bipartite.txt
*/

#include <bits/stdc++.h>

using namespace std;

int main(int argc, char const *argv[]) {
    ifstream dataIn(argv[1]);
    ofstream dataOut;
    unordered_map<int, unordered_map<int, int>> edges;
    unordered_map<int, int> vertices;

    string str;
    int n = 0;
    while (getline(dataIn, str)) {
        int u, v;
        if (str[0] == '#' || str[0] =='%') continue;
        stringstream ss(str);
        ss >> u >> v;
        if (vertices.find(u) == vertices.end())
            vertices[u] = n++;
        if (vertices.find(v) == vertices.end())
            vertices[v] = n++;
        edges[u][v] = 1;
    }
    dataIn.close();

    dataOut.open(argv[2]);
    for (auto [u, adj] : edges) {
        for (auto [v, _] : adj) {
                dataOut << vertices[u] << " " << (vertices[v] + n) << endl;
                dataOut << vertices[v] << " " << (vertices[u] + n) << endl;
            }
    }
    dataOut.close();

    return 0;
}
