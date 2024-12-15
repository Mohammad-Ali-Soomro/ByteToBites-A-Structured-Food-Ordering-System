#include <iostream>
#include <vector>
#include <unordered_map>
#include <set>
#include <limits>
#include <string>
#include <utility>
#include <algorithm>

class Graph {
private:
    std::unordered_map<std::string, std::vector<std::pair<std::string, int>>> adjList;

public:
    void addLocation(const std::string &location) {
        if (adjList.find(location) == adjList.end()) {
            adjList[location] = {};
            std::cout << "Location added: " << location << std::endl;
        } else {
            std::cout << "Location already exists: " << location << std::endl;
        }
    }

    void removeLocation(const std::string &location) {
        adjList.erase(location);
        for (auto &node : adjList) {
            node.second.erase(
                std::remove_if(node.second.begin(), node.second.end(),
                               [&location](const auto &pair) { return pair.first == location; }),
                node.second.end());
        }
        std::cout << "Location removed: " << location << std::endl;
    }

    void addPath(const std::string &from, const std::string &to, int distance) {
        adjList[from].emplace_back(to, distance);
        adjList[to].emplace_back(from, distance); // Assuming undirected graph
        std::cout << "Path added: " << from << " <--> " << to << " (Distance: " << distance << ")\n";
    }

    void removePath(const std::string &from, const std::string &to) {
        auto &fromList = adjList[from];
        fromList.erase(std::remove_if(fromList.begin(), fromList.end(),
                                      [&to](const auto &pair) { return pair.first == to; }),
                       fromList.end());

        auto &toList = adjList[to];
        toList.erase(std::remove_if(toList.begin(), toList.end(),
                                    [&from](const auto &pair) { return pair.first == from; }),
                     toList.end());

        std::cout << "Path removed: " << from << " <--> " << to << std::endl;
    }

    void displayGraph() const {
        std::cout << "Graph Representation:\n";
        for (const auto &location : adjList) {
            std::cout << location.first << ": ";
            for (const auto &neighbor : location.second) {
                std::cout << "(" << neighbor.first << ", " << neighbor.second << ") ";
            }
            std::cout << std::endl;
        }
    }

    std::vector<std::string> findShortestPath(const std::string &start, const std::string &end) {
        std::unordered_map<std::string, int> distances;
        std::unordered_map<std::string, std::string> previous;
        std::set<std::pair<int, std::string>> pq;

        for (const auto &node : adjList) {
            distances[node.first] = std::numeric_limits<int>::max();
        }

        distances[start] = 0;
        pq.insert({0, start});

        while (!pq.empty()) {
            auto [currentDistance, current] = *pq.begin();
            pq.erase(pq.begin());

            if (current == end) break;

            for (const auto &[neighbor, weight] : adjList[current]) {
                int newDistance = currentDistance + weight;
                if (newDistance < distances[neighbor]) {
                    pq.erase({distances[neighbor], neighbor});
                    distances[neighbor] = newDistance;
                    previous[neighbor] = current;
                    pq.insert({newDistance, neighbor});
                }
            }
        }

        std::vector<std::string> path;
        for (std::string at = end; at != ""; at = previous[at]) {
            path.insert(path.begin(), at);
        }

        if (path.size() == 1 && path[0] != start) {
            std::cout << "No path found from " << start << " to " << end << ".\n";
            return {};
        }

        return path;
    }
};

void showMenu() {
    std::cout << "\nDelivery Route System\n"
              << "1. Add Location\n"
              << "2. Remove Location\n"
              << "3. Add Path\n"
              << "4. Remove Path\n"
              << "5. Display Graph\n"
              << "6. Find Shortest Route\n"
              << "7. Exit\n";
}

int main() {
    Graph deliveryGraph;
    int choice;

    while (true) {
        showMenu();
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        std::string location1, location2;
        int distance;

        switch (choice) {
            case 1:
                std::cout << "Enter location name: ";
                std::cin >> location1;
                deliveryGraph.addLocation(location1);
                break;
            case 2:
                std::cout << "Enter location name to remove: ";
                std::cin >> location1;
                deliveryGraph.removeLocation(location1);
                break;
            case 3:
                std::cout << "Enter start location, end location, and distance: ";
                std::cin >> location1 >> location2 >> distance;
                deliveryGraph.addPath(location1, location2, distance);
                break;
            case 4:
                std::cout << "Enter start location and end location: ";
                std::cin >> location1 >> location2;
                deliveryGraph.removePath(location1, location2);
                break;
            case 5:
                deliveryGraph.displayGraph();
                break;
            case 6:
                std::cout << "Enter start and end locations: ";
                std::cin >> location1 >> location2;
                auto path = deliveryGraph.findShortestPath(location1, location2);
                if (!path.empty()) {
                    std::cout << "Shortest Path: ";
                    for (const auto &loc : path) {
                        std::cout << loc << " ";
                    }
                    std::cout << std::endl;
                }
                break;
            case 7:
                std::cout << "Exiting...\n";
                return 0;
            default:
                std::cout << "Invalid choice. Try again.\n";
        }
    }
    return 0;
}
