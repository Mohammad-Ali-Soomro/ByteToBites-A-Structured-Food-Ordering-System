#pragma once

//#ifndef ORDERS_H
//#define ORDERS_H

#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <ctime>

using namespace std;

class MyOrder {
private:
    struct OrderNode {
        int orderID;
        string foodItems;
        string deliveryDetails;
        string status;
        OrderNode* next;
        OrderNode* prev;
    };

    OrderNode* head;
    OrderNode* tail;
    int orderCount;

    // Helper function to generate unique Order ID
    int generateOrderID() {
        return ++orderCount;
    }

    // Helper function to get current timestamp
    string getCurrentTime() {
        time_t now = time(0);
        char* dt = ctime(&now);
        string timestamp = dt;
        timestamp.pop_back(); // Remove newline character
        return timestamp;
    }

public:
    MyOrder() {
        head = nullptr;
        tail = nullptr;
        orderCount = 0;
    }
    
    ~MyOrder() {
        while (head != nullptr) {
            OrderNode* temp = head;
            head = head->next;
            delete temp;
        }
    }

    void placeOrder() {
        string foodItems, deliveryDetails;

        cout << "Enter food items (comma-separated): ";
        cin.ignore();
        getline(cin, foodItems);

        cout << "Enter delivery details (address, phone number): ";
        getline(cin, deliveryDetails);

        int newOrderID = generateOrderID();
        string initialStatus = "Pending";

        // Create a new order node
        OrderNode* newOrder = new OrderNode;
        newOrder->orderID = newOrderID;
        newOrder->foodItems = foodItems;
        newOrder->deliveryDetails = deliveryDetails;
        newOrder->status = initialStatus;
        newOrder->next = nullptr;
        newOrder->prev = tail;

        if (tail != nullptr) {
            tail->next = newOrder;
        }
        tail = newOrder;

        if (head == nullptr) {
            head = newOrder;
        }

        // Store order details in a file
        ofstream file("orders.txt", ios::app);
        if (file.is_open()) {
            file << "Order ID: " << newOrderID << "\n"
                 << "Food Items: " << foodItems << "\n"
                 << "Delivery Details: " << deliveryDetails << "\n"
                 << "Status: " << initialStatus << "\n"
                 << "Timestamp: " << getCurrentTime() << "\n\n";
            file.close();
        }

        cout << "Order placed successfully! Your Order ID is: " << newOrderID << endl;
    }

    void viewOrderHistory() {
        if (head == nullptr) {
            cout << "No past orders found." << endl;
            return;
        }

        cout << "Order History:\n";
        OrderNode* current = head;
        while (current != nullptr) {
            cout << "Order ID: " << current->orderID << "\n"
                 << "Food Items: " << current->foodItems << "\n"
                 << "Delivery Details: " << current->deliveryDetails << "\n"
                 << "Status: " << current->status << "\n\n";
            current = current->next;
        }
    }

    void cancelOrder(int orderID) {
        OrderNode* current = head;

        while (current != nullptr && current->orderID != orderID) {
            current = current->next;
        }

        if (current == nullptr) {
            cout << "Order ID not found or already processed." << endl;
            return;
        }

        if (current->status != "Pending") {
            cout << "Order cannot be canceled as it is already " << current->status << "." << endl;
            return;
        }

        // Remove order from the list
        if (current->prev != nullptr) {
            current->prev->next = current->next;
        } else {
            head = current->next;
        }

        if (current->next != nullptr) {
            current->next->prev = current->prev;
        } else {
            tail = current->prev;
        }

        delete current;
        cout << "Order canceled successfully." << endl;
    }

    void updateOrderStatus(int orderID, const string& newStatus) {
        OrderNode* current = head;

        while (current != nullptr && current->orderID != orderID) {
            current = current->next;
        }

        if (current == nullptr) {
            cout << "Order ID not found." << endl;
            return;
        }

        current->status = newStatus;
        cout << "Order status updated to: " << newStatus << endl;
    }

    void trackOrder(int orderID) {
        OrderNode* current = head;

        while (current != nullptr && current->orderID != orderID) {
            current = current->next;
        }

        if (current == nullptr) {
            cout << "Order ID not found." << endl;
            return;
        }

        cout << "Order Status for Order ID " << orderID << ": " << current->status << endl;
    }
};

//#endif
