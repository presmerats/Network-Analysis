#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <map> 
//#include <set> 
#include <iterator> 
#include <dirent.h>
#include <cstring>
#include <memory>
#include <fstream>
#include <sstream>
#include <algorithm>
//#include <utility> 
using namespace std;


typedef pair<double, int> WArc; // weighted arc
typedef map<int, vector<WArc> > WGraph; // weighted graph
/*
struct Edge {
  string origin;
  string dest;
};
*/
struct Node {
  vector<string> neighbours;
  vector<double> distances;
};

typedef map<string, Node > Graph; // unweighted graph
typedef vector <pair<string, string> > Edges; // stores all the edges
 
const int infinit = 100000000;

vector <string> get_directory_files(const string& dir) {
	struct dirent *dirent_ptr;
	shared_ptr<DIR> directory_ptr(opendir(dir.c_str()), [](DIR* dir){ dir && closedir(dir); });
	vector <string> files;
	
	if (!directory_ptr) {
		cout << "Error opening : " << strerror(errno) << dir << endl;
		return files;
	}

	while ((dirent_ptr = readdir(directory_ptr.get())) != nullptr) {
		string file_name = dirent_ptr->d_name;
		if(file_name[0] != '.' and file_name.substr(file_name.size() - 3, 3) == "txt")
			files.push_back(file_name);
	}
	return files;
}

void create_graph(const string file_name) throw() {
	try {
		string line;
		ifstream myfile(file_name);
		if (myfile.is_open()) {
			// initializing the needed variables
			bool firstLine = true;
			int sameNode = 0;
			int n = 0;
			int m = 0; // n for number of nodes, m for number of edges
			Graph G; // a dictionary which the key is the name of node (e.g a word) and the value is a adjacency list [b, c, etc.]  
			Edges E; // all the edges
			vector <string> Nodes; // stores the words in the order of incoming
			map<string, int> nodeIndex; // stores the correspondences between words and indices
			map<int, string> indexNode; // stores the correspondences between indices and words
			
			// read all lines of the file
			while (getline(myfile, line)) {
				if (!firstLine) {
					istringstream iss(line);
					string a, b;
					// if the line contains only two elements
					if (!(iss >> a >> b)) throw "Not exact two elements in one line"; 
					// avoiding loop
					if (a != b) {
						//cout << a << " " << b << endl;
						map <string, Node>::iterator it;
						it = G.find(a);
						// if the dictionary already contains word a
						if (it != G.end()) {
							if (find(G[a].neighbours.begin(), G[a].neighbours.end(), b) == G[a].neighbours.end()) {
								G[a].neighbours.push_back(b); // add b to adjacency list
								E.push_back(make_pair(a, b)); // add to the edges vector
								++m;
							}
						}
						else {
							// since word a does not present in the dictionary, we add it with the edge
							G[a].neighbours.push_back(b);
							E.push_back(make_pair(a, b));
							Nodes.push_back(a);
							nodeIndex[a] = n;
							++n;
							++m;								
						}
						
						it = G.find(b);
						if (it == G.end()) {
							// since word b does not present in the dictionary, we add it with the edge
							//G[b].neighbours.push_back(a);
							//E.push_back(make_pair(b, a));
							Node auxN;
							G[b] = auxN; 
							Nodes.push_back(b);
							nodeIndex[b] = n;
							++n;
							//++m; 
						}
						// if the dictionary already contains word b
						/*
						if (it != G.end()) {
							if (find(G[b].neighbours.begin(), G[b].neighbours.end(), a) == G[b].neighbours.end()) {
								G[b].neighbours.push_back(a);
								E.push_back(make_pair(b, a));
								++m; 
							}
						}
						
						*/
					}
					//else ++sameNode;
				}
				else firstLine = false;
			}
			myfile.close();
			
			cout << "Print G" << endl;
			for (auto itr = G.begin(); itr != G.end(); ++itr) 
			{ 
				cout << itr->first << ": ";  
				for (string neighbour : itr->second.neighbours)
					cout << neighbour << '\t'; 
				cout << endl;
			} 
			cout << "Print E" << endl;
			for (auto p : E) 
				cout << p.first << '\t' << p.second << endl; 
			cout << "Print Nodes" << endl;
			for (auto node : Nodes) 
				cout << node << endl; 
			cout << "Print nodeIndex" << endl;
			for (auto itr = nodeIndex.begin(); itr != nodeIndex.end(); ++itr) 
				cout << itr->first << "\t" << itr->second << endl;
			cout << "n: " << n << ", m: " << m << " , " << sameNode <<endl; 
			cout << G.size() << ", " << E.size() << endl;
		}
		else cout << "Could not open the file, please check the path name"; 
	}
	catch (const char* msg) {
		cout << msg << endl;
		exit(1);
	}
}


void calculate_closeness() {
	
	
}

void calculate_distance(const WGraph& G, int s, vector<double>& d, vector<int>& p) {

	
}

void dijkstra(const WGraph& G, int s, vector<double>& d, vector<int>& p, int n) {

	d = vector<double>(n, infinit); d[s] = 0;
	p = vector<int>(n, -1);
	vector<bool> S(n, false);
	priority_queue<WArc, vector<WArc>, greater<WArc> > Q;
	Q.push(WArc(0, s));
	while (not Q.empty()) {
		int u = Q.top().second; Q.pop();
		if (not S[u]) {
			S[u] = true;
			map <int, vector<WArc> >::const_iterator it;
			it = G.find(u);
		    if (it != G.end()) {
				for (WArc a : it->second) {
					int v = a.second;
					double c = a.first;
					if (d[v] > d[u] + c) {
						d[v] = d[u] + c;
						p[v] = u;
						Q.push(WArc(d[v], v));
					} 
				} 
			}
		} 
	} 
}


int main() {
	const auto& directory_path = string("./data/");
	const auto& files = get_directory_files(directory_path);
	for (const auto& file : files) {
		cout << directory_path+file << endl;
		create_graph(directory_path+file);
	}
	
    int n, m, u, v, c, x, y;
    while (cin >> n >> m) {
        WGraph G;
        for (int i = 0; i < m; ++i) {
            cin >> u >> v >> c;
            G[u].push_back(WArc(c, v));
        }
		vector<double> d;
		vector<int> p;
        cin >> x >> y;
        dijkstra(G, x, d, p, n);
		cout << x << "\n";
		for (unsigned int j = 0; j < d.size(); ++j) {
			
			cout << "node " << j << " and cost " << d[j] << "\n";  
		}
		cout << "\n";
    }

}