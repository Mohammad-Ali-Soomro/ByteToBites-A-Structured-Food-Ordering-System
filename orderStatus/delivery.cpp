#include <iostream>
#include <queue>
#include <cstring>

using namespace std;

// Order Status enum
enum OrderStatus {
    PREPARING,
    DISPATCHED,
    ON_THE_WAY,
    DELIVERED
};

// Function to convert integer input to OrderStatus
OrderStatus getOrderStatusFromInput(int input) {
    switch (input) {
        case 1: return PREPARING;
        case 2: return DISPATCHED;
        case 3: return ON_THE_WAY;
        case 4: return DELIVERED;
        default: return PREPARING; // Default to PREPARING if invalid input
    }
}

// DeliveryRoute class for route calculation
class DeliveryRoute {
private:
    int** graph;  // 2D adjacency matrix to represent the graph of locations
    int numLocations;

public:
    // Constructor: Initializes the graph and the number of locations
    DeliveryRoute(int numLocations) {
        this->numLocations = numLocations;
        graph = new int*[numLocations];
        for (int i = 0; i < numLocations; i++) {
            graph[i] = new int[numLocations];
            for (int j = 0; j < numLocations; j++) {
                if (i == j) {
                    graph[i][j] = 0;  // No distance to itself
                } else {
                    graph[i][j] = -1;  // No direct route
                }
            }
        }
    }

    // Destructor: Cleans up dynamically allocated memory
    ~DeliveryRoute() {
        for (int i = 0; i < numLocations; i++) {
            delete[] graph[i];
        }
        delete[] graph;
    }

    // Method to add a direct route between two locations
    void addRoute(int startLocation, int endLocation, int distance) {
        if (startLocation >= 0 && startLocation < numLocations &&
            endLocation >= 0 && endLocation < numLocations) {
            graph[startLocation][endLocation] = distance;
            graph[endLocation][startLocation] = distance;  // Assuming undirected graph
        }
    }

    // Simple route finding (you can use algorithms like Dijkstra here)
    void findRoute(int startLocation, int endLocation) {
        // For simplicity, just print the direct distance (if available)
        if (graph[startLocation][endLocation] != -1) {
            cout << "Optimal Route from " << startLocation << " to " << endLocation 
                 << ": Distance = " << graph[startLocation][endLocation] << endl;
        } else {
            cout << "No direct route found between " << startLocation << " and " << endLocation << endl;
        }
    }
};

// Order class to track orders and status
class Order {
private:
    int orderId;
    char customerName[100];
    OrderStatus status;
    int startLocation, endLocation;

public:
    Order(int orderId, const char* customerName, int startLocation, int endLocation)
        : orderId(orderId), startLocation(startLocation), endLocation(endLocation), status(PREPARING) {
        strncpy(this->customerName, customerName, sizeof(this->customerName) - 1);
        this->customerName[sizeof(this->customerName) - 1] = '\0';
    }

    void updateStatus(OrderStatus newStatus) {
        status = newStatus;
    }

    void displayStatus() {
        const char* statusStrings[] = { "Preparing", "Dispatched", "On the Way", "Delivered" };
        cout << "Order ID: " << orderId << ", Customer: " << customerName 
             << ", Status: " << statusStrings[status] << endl;
    }

    int getStartLocation() {
        return startLocation;
    }

    int getEndLocation() {
        return endLocation;
    }

    OrderStatus getStatus() {
        return status;
    }
};

// OrderManager class to manage multiple orders and track their status
class OrderManager {
private:
    DeliveryRoute route;  // Private route object
    queue<Order> ordersQueue;

public:
    OrderManager(int numLocations) : route(numLocations) {}

    void createOrder(int orderId, const char* customerName, int startLocation, int endLocation) {
        Order newOrder(orderId, customerName, startLocation, endLocation);
        ordersQueue.push(newOrder);
    }

    void updateOrderStatus(int orderId, OrderStatus newStatus) {
        queue<Order> tempQueue;
        while (!ordersQueue.empty()) {
            Order order = ordersQueue.front();
            ordersQueue.pop();

            if (order.getStartLocation() == orderId) {
                order.updateStatus(newStatus);
            }

            tempQueue.push(order);
        }
        ordersQueue = tempQueue;
    }

    void displayOrders() {
        queue<Order> tempQueue = ordersQueue;
        while (!tempQueue.empty()) {
            tempQueue.front().displayStatus();
            tempQueue.pop();
        }
    }

    // Method to interact with the private DeliveryRoute object
    void findRouteForOrder(int startLocation, int endLocation) {
        route.findRoute(startLocation, endLocation);
    }

    void addRoute(int startLocation, int endLocation, int distance) {
        route.addRoute(startLocation, endLocation, distance);
    }
};

// Main function to run the system
int main() {
    int numLocations;
    cout << "Enter the number of locations: ";
    cin >> numLocations;

    OrderManager orderManager(numLocations);

    // Add routes between locations
    int numRoutes;
    cout << "Enter the number of routes to add: ";
    cin >> numRoutes;

    for (int i = 0; i < numRoutes; ++i) {
        int start, end, distance;
        cout << "Enter start location, end location and distance for route " << i + 1 << ": ";
        cin >> start >> end >> distance;
        orderManager.addRoute(start, end, distance);
    }

    // Create orders
    int numOrders;
    cout << "Enter the number of orders: ";
    cin >> numOrders;

    for (int i = 0; i < numOrders; ++i) {
        int orderId, startLocation, endLocation;
        char customerName[100];
        
        cout << "Enter Order ID, Customer Name, Start Location, End Location for order " << i + 1 << ": ";
        cin >> orderId;
        cin.ignore();  // To consume newline left by cin
        cin.getline(customerName, 100);
        cin >> startLocation >> endLocation;
        
        orderManager.createOrder(orderId, customerName, startLocation, endLocation);
    }

    // Display current orders and their statuses
    cout << "\nCurrent Orders:\n";
    orderManager.displayOrders();

    // Update status of orders
    int orderIdToUpdate;
    int statusInput;
    cout << "\nEnter Order ID to update status (1: Preparing, 2: Dispatched, 3: On the Way, 4: Delivered): ";
    cin >> orderIdToUpdate >> statusInput;

    // Convert integer input to OrderStatus enum
    OrderStatus newStatus = getOrderStatusFromInput(statusInput);

    orderManager.updateOrderStatus(orderIdToUpdate, newStatus);

    cout << "\nUpdated Orders:\n";
    orderManager.displayOrders();

    // Display route for an order
    int startLocation, endLocation;
    cout << "\nEnter start location and end location to find route: ";
    cin >> startLocation >> endLocation;

    orderManager.findRouteForOrder(startLocation, endLocation);

    return 0;
}
