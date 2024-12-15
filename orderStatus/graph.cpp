#include "graph.h"
#include <algorithm>

void Graph::addEdge(const string& src, const string& dest, int weight) {
    adjList[src][dest] = weight;
    adjList[dest][src] = weight; // Assuming undirected graph
}

void Graph::addNode(const string& node) {
    adjList[node] = unordered_map<string, int>();
}

void Graph::updateEdgeWeight(const string& src, const string& dest, int weight) {
    if (adjList.find(src) != adjList.end() && adjList[src].find(dest) != adjList[src].end()) {
        adjList[src][dest] = weight;
        adjList[dest][src] = weight; // Assuming undirected graph
    }
}

vector<string> Graph::shortestPath(const string& start, const string& end) {
    unordered_map<string, int> distances;
    unordered_map<string, string> previous;
    set<pair<int, string>> nodes;
    


    for (const auto& pair : adjList) {
        distances[pair.first] = numeric_limits<int>::max();
        nodes.insert({numeric_limits<int>::max(), pair.first});
    }

    distances[start] = 0;
    nodes.insert({0, start});

    while (!nodes.empty()) {
        string current = nodes.begin()->second;
        nodes.erase(nodes.begin());

        if (current == end) {
            vector<string> path;
            while (previous.find(current) != previous.end()) {
                path.push_back(current);
                current = previous[current];
            }
            path.push_back(start);
            reverse(path.begin(), path.end());
            return path;
        }

        for (const auto& neighbor : adjList[current]) {
            int alt = distances[current] + neighbor.second;
            if (alt < distances[neighbor.first]) {
                nodes.erase({distances[neighbor.first], neighbor.first});
                distances[neighbor.first] = alt;
                previous[neighbor.first] = current;
                nodes.insert({alt, neighbor.first});
            }
        }
    }

    return {}; // No path found
}

void Graph::displayGraph() const {
    for (const auto& node : adjList) {
        cout << node.first << ": ";
        for (const auto& neighbor : node.second) {
            cout << "(" << neighbor.first << ", " << neighbor.second << ") ";
        }
        cout << endl;
    }
}