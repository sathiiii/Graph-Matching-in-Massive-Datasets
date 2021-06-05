/*
	Assumptions:
		(01). The given graph is bipartite.
		(02). Edges in the stream are sent as two integers (u, v) such that u is in the left partition and v is in the right.
		(03). There are no duplicate edges in the stream (Does it matter???)

	Usage example: ./match_approx ../data/wiki-Vote-bipartite.txt 14
*/

#include <bits/stdc++.h>
#include <time.h>

#define NIL 0
#define INF INT_MAX

using namespace std;

// A class to represent Bipartite graph for Hopcroft
// Karp implementation
class BipGraph {
    // m and n are number of vertices on left
    // and right sides of Bipartite Graph
    int m, n;
 
    // adj[u] stores adjacents of left side
    // vertex 'u'. The value of u ranges from 1 to m.
    // 0 is used for dummy vertex
    list<int> *adj;
 
    // These are basically pointers to arrays needed
    // for hopcroftKarp()
    int *pairU, *pairV, *dist;
 
public:
    BipGraph(int m, int n); // Constructor
    void addEdge(int u, int v); // To add edge
 
    // Returns true if there is an augmenting path
    bool bfs();
 
    // Adds augmenting path if there is one beginning
    // with u
    bool dfs(int u);
 
    // Returns size of maximum matcing
    int hopcroftKarp();
};
 
// Returns size of maximum matching
int BipGraph::hopcroftKarp() {
    // pairU[u] stores pair of u in matching where u
    // is a vertex on left side of Bipartite Graph.
    // If u doesn't have any pair, then pairU[u] is NIL
    pairU = new int[m+1];
 
    // pairV[v] stores pair of v in matching. If v
    // doesn't have any pair, then pairU[v] is NIL
    pairV = new int[n+1];
 
    // dist[u] stores distance of left side vertices
    // dist[u] is one more than dist[u'] if u is next
    // to u'in augmenting path
    dist = new int[m+1];
 
    // Initialize NIL as pair of all vertices
    for (int u=0; u<=m; u++)
        pairU[u] = NIL;
    for (int v=0; v<=n; v++)
        pairV[v] = NIL;
 
    // Initialize result
    int result = 0;
 
    // Keep updating the result while there is an
    // augmenting path.
    while (bfs())
    {
        // Find a free vertex
        for (int u=1; u<=m; u++)
 
            // If current vertex is free and there is
            // an augmenting path from current vertex
            if (pairU[u]==NIL && dfs(u))
                result++;
    }
    return result;
}
 
// Returns true if there is an augmenting path, else returns
// false
bool BipGraph::bfs() {
    queue<int> Q; //an integer queue
 
    // First layer of vertices (set distance as 0)
    for (int u=1; u<=m; u++)
    {
        // If this is a free vertex, add it to queue
        if (pairU[u]==NIL)
        {
            // u is not matched
            dist[u] = 0;
            Q.push(u);
        }
 
        // Else set distance as infinite so that this vertex
        // is considered next time
        else dist[u] = INF;
    }
 
    // Initialize distance to NIL as infinite
    dist[NIL] = INF;
 
    // Q is going to contain vertices of left side only.
    while (!Q.empty())
    {
        // Dequeue a vertex
        int u = Q.front();
        Q.pop();
 
        // If this node is not NIL and can provide a shorter path to NIL
        if (dist[u] < dist[NIL])
        {
            // Get all adjacent vertices of the dequeued vertex u
            list<int>::iterator i;
            for (i=adj[u].begin(); i!=adj[u].end(); ++i)
            {
                int v = *i;
 
                // If pair of v is not considered so far
                // (v, pairV[V]) is not yet explored edge.
                if (dist[pairV[v]] == INF)
                {
                    // Consider the pair and add it to queue
                    dist[pairV[v]] = dist[u] + 1;
                    Q.push(pairV[v]);
                }
            }
        }
    }
 
    // If we could come back to NIL using alternating path of distinct
    // vertices then there is an augmenting path
    return (dist[NIL] != INF);
}
 
// Returns true if there is an augmenting path beginning with free vertex u
bool BipGraph::dfs(int u) {
    if (u != NIL)
    {
        list<int>::iterator i;
        for (i=adj[u].begin(); i!=adj[u].end(); ++i)
        {
            // Adjacent to u
            int v = *i;
 
            // Follow the distances set by BFS
            if (dist[pairV[v]] == dist[u]+1)
            {
                // If dfs for pair of v also returns
                // true
                if (dfs(pairV[v]) == true)
                {
                    pairV[v] = u;
                    pairU[u] = v;
                    return true;
                }
            }
        }
 
        // If there is no augmenting path beginning with u.
        dist[u] = INF;
        return false;
    }
    return true;
}
 
// Constructor
BipGraph::BipGraph(int m, int n) {
    this->m = m;
    this->n = n;
    adj = new list<int>[m+1];
}
 
// To add edge from u to v and v to u
void BipGraph::addEdge(int u, int v) {
    adj[u].push_back(v); // Add u to v’s list.
}

// ------------------------------Approximation Algorithm------------------------------


// Take k edges uniformly at random from the remaining edges.
int uarSample(vector<pair<int, int>>& edges, vector<pair<int, int>>& picked) {
	int i = 0, j = 0;
	int m = edges.size(), k = picked.size();
	for (; i < m and j < k; i++)
		picked[j++] = edges[i];
	if (j < k) return j;
	srand(time(NULL));
	for ( ; i < m; i++) {
		j = rand() % (i + 1);
		if (j < k)
			picked[j] = edges[i];
	}
	return k;
}

// Approximate vertex cover
void minVertexCover(vector<pair<int, int>>& edges, unordered_map<int, set<int>> sample) {
	int m = edges.size();
	unordered_map<int, int> visited;
	set<int> idx;
	for (auto u : sample)
		if (visited.find(u.first) == visited.end())
			for (int v : u.second)
				if (visited.find(v) == visited.end()) {
					visited[u.first] = 0;
					visited[v] = 0;
					for (int i = 0; i < m; i++)
						if (edges[i].first == u.first && edges[i].second == v) idx.insert(i);
					break;
				}
	vector<pair<int, int>> tmp;
	for (int i = 0; i < m; i++) {
		if (idx.find(i) == idx.end()) continue;
		tmp.push_back(edges[i]);
	}
	edges = tmp;
}

int main(int argc, char const *argv[]) {
    if (argc > 1) {
		vector<pair<int, int>> edges;
		unordered_map<int, int> vertices;

        ifstream dataIn(argv[1]);
        string str;
        int m = 0, n = 0;
        while (getline(dataIn, str)) {
			int u, v;
            if (str[0] == '#' || str[0] =='%') continue;
            stringstream ss(str);
            ss >> u >> v;
			if (vertices.find(u) == vertices.end())
				vertices[u] = n++;
			if (vertices.find(v) == vertices.end())
				vertices[v] = n++;
			edges.push_back({u, v});
		}
		dataIn.close();
		m = edges.size();
		cout << "The graph has " << n << " vertices and " << m << " edges." << endl;

		float alpha = argc == 3 ? stof(argv[2]) : sqrt(n);

		int k = n * alpha;
		unordered_map<int, set<int>> sample;
		vector<pair<int, int>> picked(k);
		int s, i = 0;

		BipGraph g(n / 2, n / 2), f(n / 2, n / 2);

		for (auto [u, v] : edges)
			f.addEdge(u, v - n / 2);

		time_t start = time(NULL);

		while ((s = uarSample(edges, picked)) == k) {
			// cout << "Picked edges at iteration-" << i << ":" << endl;
			for (auto e : picked) {
				sample[e.first].insert(e.second);
				sample[e.second].insert(e.first);
				g.addEdge(e.first, e.second - n / 2);
				// cout << e.first << "-" << e.second << " ";
			}
			// cout << endl;
			minVertexCover(edges, sample);
			i++;
		}

		// ***Take the (i-1)st sample

		cout << "Size of the sample: " << sample.size() << endl;

		cout << "Approximate solution: " << g.hopcroftKarp() << endl;

		time_t end = time(NULL);

		cout << "Approximation algorithm took " << (end - start) << "s." << endl;
		
		start = time(NULL);
		cout << "Hopcroft-Karp result: " << f.hopcroftKarp() << endl;
		end = time(NULL);

		cout << "Hopcroft-Karp algorithm took " << (end - start) << "s." << endl;
 	}
    return 0;
}
