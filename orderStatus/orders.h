#ifndef ORDERS_H
#define ORDERS_H

#include <iostream>
#include <string>
#include <ctime>

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

// Order class
class Order {
public:
    int orderId;
    string customerName;
    OrderStatus status;
    time_t timestamp;  // To store the order creation time

    Order(int orderId, string customerName = " ", OrderStatus status = PREPARING) {
        this->orderId = orderId;
        this->customerName = customerName;
        this->status = status;
        this->timestamp = time(0);  // Assign the current time
    }

    void displayOrderInfo() {
        const char* statusStrings[] = { "Preparing", "Dispatched", "On the Way", "Delivered" };
        char timestampStr[100];
        strftime(timestampStr, sizeof(timestampStr), "%Y-%m-%d %H:%M:%S", localtime(&timestamp));
        cout << "Order ID: " << orderId << ", Customer: " << customerName 
             << ", Status: " << statusStrings[status] << ", Timestamp: " << timestampStr << endl;
    }
};

// Node class for doubly linked list
class Node {
public:
    Order order;
    Node* next;
    Node* prev;

    Node(Order order) : order(order), next(nullptr), prev(nullptr) {}
};

// Linked list class for order management
class OrderList {
private:
    Node* head;
    Node* tail;

public:
    OrderList() : head(nullptr), tail(nullptr) {}

    // Insert a new order at the end
    void insertOrder(Order newOrder) {
        Node* newNode = new Node(newOrder);
        if (!head) {
            head = tail = newNode;  // First node in the list
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;  // Move the tail to the new node
        }
    }

    // Display all orders
    void displayOrders() {
        Node* current = head;
        while (current) {
            current->order.displayOrderInfo();
            current = current->next;
        }
    }

    // Update an order's status
    void updateOrderStatus(int orderId, OrderStatus newStatus) {
        Node* current = head;
        while (current) {
            if (current->order.orderId == orderId) {
                current->order.status = newStatus;
                cout << "Updated Order ID " << orderId << " Status." << endl;
                return;
            }
            current = current->next;
        }
        cout << "Order ID not found." << endl;
    }

    // Delete an order by order ID
    void deleteOrder(int orderId) {
        Node* current = head;
        while (current) {
            if (current->order.orderId == orderId) {
                if (current->prev) {
                    current->prev->next = current->next;
                } else {
                    head = current->next;  // If deleting the head
                }
                if (current->next) {
                    current->next->prev = current->prev;
                } else {
                    tail = current->prev;  // If deleting the tail
                }
                delete current;
                cout << "Deleted Order ID " << orderId << endl;
                return;
            }
            current = current->next;
        }
        cout << "Order ID not found." << endl;
    }

    // Search for an order by order ID
    void searchOrder(int orderId) {
        Node* current = head;
        while (current) {
            if (current->order.orderId == orderId) {
                current->order.displayOrderInfo();
                return;
            }
            current = current->next;
        }
        cout << "Order ID not found." << endl;
    }
};

#endif