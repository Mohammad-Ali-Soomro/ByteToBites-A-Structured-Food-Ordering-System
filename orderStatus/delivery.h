#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <climits>
using namespace std;

class Delivery {
private:
    struct DeliveryAgent {
        string name;
        bool isAvailable;
        int deliveriesCompleted;
    };

    struct Route {
        int from;
        int to;
        int distance;
    };

    vector<DeliveryAgent> agents;
    vector<Route> routes;
    int locationCount;

    // Helper to find agent by name
    int findAgentByName(const string& name) {
        for (size_t i = 0; i < agents.size(); ++i) {
            if (agents[i].name == name) {
                return i;
            }
        }
        return -1;
    }

public:
    // Constructor
    Delivery(int locations) : locationCount(locations) {}

    // Add a delivery agent
    void addDeliveryAgent(const string& name) {
        agents.push_back({name, true, 0});
        cout << "Agent " << name << " added to the system.\n";
    }

    // Add a delivery route
    void addDeliveryRoute(int from, int to, int distance) {
        if (from >= 0 && from < locationCount && to >= 0 && to < locationCount) {
            routes.push_back({from, to, distance});
            cout << "Route added from location " << from << " to location " << to << " with distance " << distance << " units.\n";
        } else {
            cout << "Invalid locations for the route.\n";
        }
    }

    // Assign an available agent to an order
    bool assignAgent(int orderID, const string& agentName) {
        // Check if agent exists
        bool agentFound = false;
        for (const auto& agent : agents) {
            if (agent.name == agentName) {
                agentFound = true;
                break;
            }
        }

        if (!agentFound) {
            cout << "Error: Agent " << agentName << " not found.\n";
            return false;
        }

        cout << "Agent " << agentName << " assigned to order #" << orderID << endl;
        return true;
    }

    // Mark an order as delivered
    void completeOrder(const string& agentName) {
        int index = findAgentByName(agentName);
        if (index != -1 && !agents[index].isAvailable) {
            agents[index].isAvailable = true;
            agents[index].deliveriesCompleted++;
            cout << "Agent " << agentName << " has completed an order.\n";
        } else {
            cout << "Agent " << agentName << " is either not found or already available.\n";
        }
    }

    // Find the shortest route between two locations
    void findShortestRoute(int start, int end) {
        vector<int> distances(locationCount, INT_MAX);
        distances[start] = 0;

        for (int i = 0; i < locationCount - 1; ++i) {
            for (const auto& route : routes) {
                if (distances[route.from] != INT_MAX && distances[route.from] + route.distance < distances[route.to]) {
                    distances[route.to] = distances[route.from] + route.distance;
                }
            }
        }

        if (distances[end] == INT_MAX) {
            cout << "No route exists from location " << start << " to location " << end << ".\n";
        } else {
            cout << "Shortest route from location " << start << " to location " << end << " is: " << distances[end] << " units.\n";
        }
    }

    // Display agent details
    void displayAgentDetails() {
        for (const auto& agent : agents) {
            cout << "Agent Name: " << agent.name << "\n"
                 << "Status: " << (agent.isAvailable ? "Available" : "Busy") << "\n"
                 << "Deliveries Completed: " << agent.deliveriesCompleted << "\n\n";
        }
    }
};


