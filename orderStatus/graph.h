#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <limits>
#include <set>
#include <string>

using namespace std;

class Graph {
public:
    void addEdge(const string& src, const string& dest, int weight);
    void addNode(const string& node);
    void updateEdgeWeight(const string& src, const string& dest, int weight);
    vector<string> shortestPath(const string& start, const string& end);
    void displayGraph() const;

private:
    unordered_map<string, unordered_map<string, int>> adjList;
    int getWeight(const string& src, const string& dest) const;
};

#endif // GRAPH_H