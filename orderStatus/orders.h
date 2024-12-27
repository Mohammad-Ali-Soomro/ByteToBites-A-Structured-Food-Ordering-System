#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <ctime>
#include <unordered_set>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <unordered_map>

using namespace std;

class MyOrder {
private:
    // Singleton instance
    static MyOrder* instance;
    
    struct OrderNode {
        int orderID;
        string foodItems;
        string deliveryDetails;
        string status;
        double totalAmount;
        OrderNode* next;
        OrderNode* prev;
    };

    OrderNode* head;
    OrderNode* tail;
    int orderCount;
    vector<pair<string, double>> currentOrderItems;  // Store current order items
    double currentOrderTotal = 0.0;

    // Add map to store order statuses
    unordered_map<int, string> orderStatuses;

    // Make constructor private for singleton
    MyOrder() {
        head = nullptr;
        tail = nullptr;
        orderCount = 0;
    }

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

    void updateOrderStats(const string& username, double amount) {
        // Update user statistics
        ofstream statsFile("D:\\GIKI\\SEMESTER 03\\DSA\\Semester Project\\Dsa Project\\stats.txt", ios::app);
        if (statsFile.is_open()) {
            statsFile << getCurrentTime() << "," << username << "," << amount << "\n";
            statsFile.close();
        }
    }

public:
    // Singleton access method
    static MyOrder& getInstance() {
        if (instance == nullptr) {
            instance = new MyOrder();
        }
        return *instance;
    }

    void placeOrder() {
        if (currentOrderItems.empty()) {
            cout << "No items in order. Please select items from the menu first.\n";
            return;
        }

        string deliveryDetails;
        cout << "Enter delivery details (address, phone number): ";
        cin.ignore();
        getline(cin, deliveryDetails);

        int newOrderID = generateOrderID();
        string initialStatus = "Pending";
        
        // Calculate total and create food items string
        stringstream foodItemsStream;
        double totalAmount = 0.0;
        
        for (const auto& item : currentOrderItems) {
            foodItemsStream << item.first << " (Rs. " << item.second << "), ";
            totalAmount += item.second;
        }
        
        string foodItems = foodItemsStream.str();
        if (!foodItems.empty()) {
            foodItems = foodItems.substr(0, foodItems.length() - 2);
        }

        // Create new order node
        OrderNode* newOrder = new OrderNode;
        newOrder->orderID = newOrderID;
        newOrder->foodItems = foodItems;
        newOrder->deliveryDetails = deliveryDetails;
        newOrder->status = initialStatus;
        newOrder->totalAmount = totalAmount;
        newOrder->next = nullptr;
        newOrder->prev = tail;

        if (tail != nullptr) {
            tail->next = newOrder;
        }
        tail = newOrder;

        if (head == nullptr) {
            head = newOrder;
        }

        // Save to file
        ofstream file("D:\\Dsa Project\\orderStatus\\orders.txt", ios::app);
        if (file.is_open()) {
            file << "Order ID: " << newOrderID << "\n"
                 << "Food Items: " << foodItems << "\n"
                 << "Delivery Details: " << deliveryDetails << "\n"
                 << "Status: " << initialStatus << "\n"
                 << "Total Amount: Rs. " << totalAmount << "\n"
                 << "Timestamp: " << getCurrentTime() << "\n\n";
            file.close();
        }

        cout << "Order placed successfully! Your Order ID is: " << newOrderID << endl;
        
        // Clear current order after placing
        currentOrderItems.clear();
    }

    void viewOrderHistory(const string& username) {
        ifstream file("D:\\GIKI\\SEMESTER 03\\DSA\\Semester Project\\Dsa Project\\orderStatus\\orders.txt");
        if (!file.is_open()) {
            cout << "No order history found.\n";
            return;
        }

        cout << "\n=== Order History for " << username << " ===\n";
        string line;
        bool foundOrder = false;
        bool isUserOrder = false;

        while (getline(file, line)) {
            if (line.find("=== Order Details ===") != string::npos) {
                isUserOrder = false;
                continue;
            }
            if (line.find("Username: " + username) != string::npos) {
                isUserOrder = true;
                foundOrder = true;
                cout << "\n" << line << endl;
                continue;
            }
            if (isUserOrder && !line.empty()) {
                cout << line << endl;
            }
        }

        if (!foundOrder) {
            cout << "No orders found for " << username << endl;
        }
        file.close();
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

    void updateOrderStatus(int orderID, const string& newStatus, const string& riderName) {
        ifstream inFile("D:\\GIKI\\SEMESTER 03\\DSA\\Semester Project\\Dsa Project\\orderStatus\\orders.txt");
        ofstream outFile("D:\\GIKI\\SEMESTER 03\\DSA\\Semester Project\\Dsa Project\\orderStatus\\orders_temp.txt");
        
        if (!inFile || !outFile) {
            cout << "Error: Cannot access order files.\n";
            return;
        }

        string line;
        bool foundOrder = false;
        bool isCurrentOrder = false;
        
        while (getline(inFile, line)) {
            if (line.find("Order ID: " + to_string(orderID)) != string::npos) {
                isCurrentOrder = true;
                foundOrder = true;
            }

            if (isCurrentOrder && line.find("Status:") != string::npos) {
                outFile << "Status: " << newStatus << "\n";
                if (!riderName.empty()) {
                    outFile << "Delivery Agent: " << riderName << "\n";
                }
                isCurrentOrder = false;
            } else {
                outFile << line << "\n";
            }
        }

        inFile.close();
        outFile.close();

        if (foundOrder) {
            remove("D:\\GIKI\\SEMESTER 03\\DSA\\Semester Project\\Dsa Project\\orderStatus\\orders.txt");
            rename("D:\\GIKI\\SEMESTER 03\\DSA\\Semester Project\\Dsa Project\\orderStatus\\orders_temp.txt",
                   "D:\\GIKI\\SEMESTER 03\\DSA\\Semester Project\\Dsa Project\\orderStatus\\orders.txt");
            cout << "Order #" << orderID << " status updated to: " << newStatus << endl;
        } else {
            cout << "Order #" << orderID << " not found.\n";
            remove("D:\\GIKI\\SEMESTER 03\\DSA\\Semester Project\\Dsa Project\\orderStatus\\orders_temp.txt");
        }
    }

    void trackOrder(int orderID) {
        ifstream file("D:\\GIKI\\SEMESTER 03\\DSA\\Semester Project\\Dsa Project\\orderStatus\\orders.txt");
        if (!file.is_open()) {
            cout << "Cannot access order records.\n";
            return;
        }

        string line;
        bool foundOrder = false;
        bool isCurrentOrder = false;

        cout << "\n=== Order Tracking Details ===\n";
        
        while (getline(file, line)) {
            if (line.find("Order ID: " + to_string(orderID)) != string::npos) {
                isCurrentOrder = true;
                foundOrder = true;
            }
            
            if (isCurrentOrder) {
                if (line.find("Status:") != string::npos || 
                    line.find("Delivery Agent:") != string::npos ||
                    line.find("Date:") != string::npos) {
                    cout << line << endl;
                }
                if (line.empty()) {
                    isCurrentOrder = false;
                }
            }
        }

        if (!foundOrder) {
            cout << "Order #" << orderID << " not found.\n";
            cout << "Please check your Order ID and try again.\n";
        }
        
        file.close();
    }

    // New method to check if an item is in the current order
    bool isItemInOrder(const string& itemName) const {
        return std::find_if(currentOrderItems.begin(), currentOrderItems.end(),
            [&itemName](const auto& item) { return item.first == itemName; }) != currentOrderItems.end();
    }

    // Get current order items
    const vector<pair<string, double>>& getCurrentOrderItems() const {
        return currentOrderItems;
    }

    // Add item to current order
    void addItemToOrder(const string& itemName, double price) {
        currentOrderItems.push_back({itemName, price});
        currentOrderTotal += price;
        cout << "Added " << itemName << " to order (Rs. " << price << ")\n";
        cout << "Current total: Rs. " << currentOrderTotal << " (without tax)\n";
    }

    bool loadMostRecentOrder() {
        ifstream file("D:\\Dsa Project\\orderStatus\\orders.txt");
        if (!file.is_open()) {
            return false;
        }

        string line;
        vector<string> items;
        double price;
        currentOrderItems.clear();

        // Read the file from end to beginning to get the most recent order
        while (getline(file, line)) {
            if (line.find("Food Items:") != string::npos) {
                string itemsStr = line.substr(11); // Skip "Food Items: "
                stringstream ss(itemsStr);
                string item;
                
                while (getline(ss, item, ',')) {
                    size_t priceStart = item.find("Rs. ");
                    if (priceStart != string::npos) {
                        string itemName = item.substr(0, priceStart);
                        string priceStr = item.substr(priceStart + 4);
                        // Remove parentheses and convert to double
                        priceStr = priceStr.substr(0, priceStr.find(')'));
                        price = stod(priceStr);
                        
                        // Trim whitespace
                        itemName = itemName.substr(0, itemName.find_last_not_of(" \t") + 1);
                        currentOrderItems.push_back({itemName, price});
                    }
                }
                return !currentOrderItems.empty();
            }
        }
        return false;
    }

    void saveBillToFile(double subtotal, double tax, double finalAmount) {
        ofstream file("D:\\Dsa Project\\orderStatus\\bills.txt", ios::app);
        if (file.is_open()) {
            file << "\n=== Bill Details ===\n";
            file << "Date: " << getCurrentTime() << "\n";
            file << "Items:\n";
            
            for (const auto& item : currentOrderItems) {
                file << item.first << " - Rs. " << item.second << "\n";
            }
            
            file << "\nSubtotal: Rs. " << subtotal;
            file << "\nTax (5%): Rs. " << tax;
            file << "\nFinal Amount: Rs. " << finalAmount;
            file << "\n==================\n";
            file.close();
        }
    }

    double getCurrentOrderTotal() const {
        return currentOrderTotal;
    }

    bool placeOrder(const string& username) {
        try {
            if (currentOrderItems.empty()) {
                throw runtime_error("No items in order. Please select items first.");
            }

            string deliveryDetails;
            cout << "Enter delivery details (address, phone number): ";
            cin.ignore();
            getline(cin, deliveryDetails);

            int newOrderID = generateOrderID();
            
            ofstream file("D:\\GIKI\\SEMESTER 03\\DSA\\Semester Project\\Dsa Project\\orderStatus\\orders.txt", ios::app);
            if (!file.is_open()) {
                throw runtime_error("Failed to open orders file.");
            }

            double totalWithTax = currentOrderTotal * 1.05;
            
            file << "\n=== Order Details ===\n"
                 << "Order ID: " << newOrderID << "\n"
                 << "Username: " << username << "\n"
                 << "Date: " << getCurrentTime() << "\n"
                 << "Items:\n";
            
            for (const auto& item : currentOrderItems) {
                file << "- " << item.first << " (Rs. " << item.second << ")\n";
            }
            
            file << "\nSubtotal: Rs. " << currentOrderTotal << "\n"
                 << "Tax (5%): Rs. " << (currentOrderTotal * 0.05) << "\n"
                 << "Total Amount: Rs. " << totalWithTax << "\n"
                 << "Delivery Details: " << deliveryDetails << "\n"
                 << "Status: Pending\n"
                 << "===================\n";
            file.close();

            // Update order statistics
            updateOrderStats(username, totalWithTax);

            cout << "\nOrder placed successfully!\n";
            cout << "Your Order ID is: " << newOrderID << endl;
            cout << "You can track your order status using this ID.\n";
            
            // Generate bill immediately
            generateBill(username, newOrderID);
            
            // Clear current order
            currentOrderItems.clear();
            currentOrderTotal = 0.0;
            
            return true;
        }
        catch (const exception& e) {
            cout << "Error placing order: " << e.what() << endl;
            return false;
        }
    }

    void generateBill(const string& username, int orderID) {
        ofstream billFile("D:\\GIKI\\SEMESTER 03\\DSA\\Semester Project\\Dsa Project\\orderStatus\\bills.txt", ios::app);
        if (billFile.is_open()) {
            billFile << "\n=== Bill #" << orderID << " ===\n"
                    << "Customer: " << username << "\n"
                    << "Date: " << getCurrentTime() << "\n"
                    << "\nItems ordered:\n"
                    << "--------------------------------\n";
            
            for (const auto& item : currentOrderItems) {
                billFile << left << setw(30) << item.first 
                        << right << setw(10) << "Rs. " << fixed << setprecision(2) << item.second << "\n";
            }
            
            double tax = currentOrderTotal * 0.05;
            double finalAmount = currentOrderTotal + tax;
            
            billFile << "--------------------------------\n"
                    << left << setw(30) << "Subtotal:" 
                    << right << setw(10) << "Rs. " << currentOrderTotal << "\n"
                    << left << setw(30) << "Tax (5%):" 
                    << right << setw(10) << "Rs. " << tax << "\n"
                    << left << setw(30) << "Final Amount:" 
                    << right << setw(10) << "Rs. " << finalAmount << "\n"
                    << "================================\n";
            billFile.close();
        }
    }

    bool loadMostRecentOrderForUser(const string& username) {
        ifstream file("D:\\Dsa Project\\orderStatus\\orders.txt");
        if (!file.is_open()) {
            return false;
        }

        string line;
        bool foundOrder = false;
        vector<string> orderLines;

        while (getline(file, line)) {
            if (line.find("Username: " + username) != string::npos) {
                foundOrder = true;
                orderLines.clear();
                orderLines.push_back(line);
            } else if (foundOrder && !line.empty()) {
                orderLines.push_back(line);
            } else if (foundOrder && line.empty()) {
                // Keep only the most recent order
                currentOrderItems.clear();
                currentOrderTotal = 0.0;
            }
        }

        if (!orderLines.empty()) {
            // Process the most recent order
            for (const string& orderLine : orderLines) {
                if (orderLine.find(" - Rs. ") != string::npos) {
                    size_t pricePos = orderLine.find(" - Rs. ");
                    string itemName = orderLine.substr(0, pricePos);
                    double price = stod(orderLine.substr(pricePos + 7));
                    currentOrderItems.push_back({itemName, price});
                    currentOrderTotal += price;
                }
            }
            return true;
        }
        return false;
    }

    void displayBill(const string& username) {
        ifstream file("D:\\GIKI\\SEMESTER 03\\DSA\\Semester Project\\Dsa Project\\orderStatus\\orders.txt");
        if (!file.is_open()) {
            cout << "No orders found.\n";
            return;
        }

        cout << "\n=== Your Orders ===\n";
        string line;
        bool foundOrder = false;
        bool isUserOrder = false;
        int currentOrderId = 0;

        while (getline(file, line)) {
            if (line.find("Order ID: ") != string::npos) {
                currentOrderId = stoi(line.substr(10));
                isUserOrder = false;
            }
            else if (line.find("Username: " + username) != string::npos) {
                isUserOrder = true;
                foundOrder = true;
                cout << "\n----------------------------------------\n";
                cout << "Order #" << currentOrderId << endl;
            }
            
            if (isUserOrder && !line.empty()) {
                cout << line << endl;
                if (line.find("Status:") != string::npos) {
                    cout << "----------------------------------------\n";
                }
            }
        }

        if (!foundOrder) {
            cout << "No orders found for your account.\n";
        }
        file.close();
    }

    void viewAllOrders() {
        ifstream file("D:\\GIKI\\SEMESTER 03\\DSA\\Semester Project\\Dsa Project\\orderStatus\\orders.txt");
        if (!file.is_open()) {
            cout << "No orders found.\n";
            return;
        }

        cout << "\n=== All Orders ===\n";
        string line;
        while (getline(file, line)) {
            cout << line << endl;
        }
        file.close();
    }

    void viewAssignedOrders(const string& riderName) {
        ifstream file("D:\\GIKI\\SEMESTER 03\\DSA\\Semester Project\\Dsa Project\\orderStatus\\orders.txt");
        if (!file.is_open()) {
            cout << "No orders found.\n";
            return;
        }

        cout << "\n=== Orders Assigned to " << riderName << " ===\n";
        string line;
        bool foundOrder = false;
        bool isAssignedOrder = false;
        int currentOrderId = 0;

        while (getline(file, line)) {
            if (line.find("Order ID: ") != string::npos) {
                currentOrderId = stoi(line.substr(10));
                isAssignedOrder = false;
            }
            else if (line.find("Delivery Agent: " + riderName) != string::npos) {
                isAssignedOrder = true;
                foundOrder = true;
                cout << "\n----------------------------------------\n";
                cout << "Order #" << currentOrderId << endl;
            }
            
            if (isAssignedOrder && !line.empty()) {
                cout << line << endl;
            }
        }

        if (!foundOrder) {
            cout << "No orders currently assigned to you.\n";
        }
        file.close();
    }

    void updateDeliveryStatus(int orderID, const string& status, const string& riderName, const string& comment = "") {
        ifstream inFile("D:\\GIKI\\SEMESTER 03\\DSA\\Semester Project\\Dsa Project\\orderStatus\\orders.txt");
        ofstream outFile("D:\\GIKI\\SEMESTER 03\\DSA\\Semester Project\\Dsa Project\\orderStatus\\orders_temp.txt");
        
        bool foundOrder = false;
        bool isAssignedToRider = false;
        string line;
        
        while (getline(inFile, line)) {
            outFile << line << "\n";
            
            if (line.find("Order ID: " + to_string(orderID)) != string::npos) {
                foundOrder = true;
            }
            else if (foundOrder && line.find("Delivery Agent: " + riderName) != string::npos) {
                isAssignedToRider = true;
            }
            else if (foundOrder && line.find("Status:") != string::npos && isAssignedToRider) {
                outFile << "Status: " << status << "\n";
                if (!comment.empty()) {
                    outFile << "Delivery Note: " << comment << "\n";
                }
                foundOrder = false;
                continue;
            }
        }

        inFile.close();
        outFile.close();

        if (isAssignedToRider) {
            remove("D:\\GIKI\\SEMESTER 03\\DSA\\Semester Project\\Dsa Project\\orderStatus\\orders.txt");
            rename("D:\\GIKI\\SEMESTER 03\\DSA\\Semester Project\\Dsa Project\\orderStatus\\orders_temp.txt",
                   "D:\\GIKI\\SEMESTER 03\\DSA\\Semester Project\\Dsa Project\\orderStatus\\orders.txt");
            cout << "Order status updated successfully!\n";
        } else {
            cout << "Error: Order not found or not assigned to you.\n";
            remove("D:\\GIKI\\SEMESTER 03\\DSA\\Semester Project\\Dsa Project\\orderStatus\\orders_temp.txt");
        }
    }

    void viewRiderDeliveryHistory(const string& riderName) {
        ifstream file("D:\\GIKI\\SEMESTER 03\\DSA\\Semester Project\\Dsa Project\\orderStatus\\orders.txt");
        if (!file.is_open()) {
            cout << "No delivery history found.\n";
            return;
        }

        cout << "\n=== Delivery History for " << riderName << " ===\n";
        string line;
        bool foundDelivery = false;
        vector<string> currentOrder;
        bool isRelevantOrder = false;

        while (getline(file, line)) {
            if (line.find("=== Order Details ===") != string::npos) {
                if (isRelevantOrder && !currentOrder.empty()) {
                    for (const auto& orderLine : currentOrder) {
                        cout << orderLine << endl;
                    }
                    cout << "----------------------------------------\n";
                }
                currentOrder.clear();
                isRelevantOrder = false;
            }

            if (!line.empty()) {
                currentOrder.push_back(line);
            }

            if (line.find("Delivery Agent: " + riderName) != string::npos) {
                isRelevantOrder = true;
                foundDelivery = true;
            }
        }

        // Print last order if relevant
        if (isRelevantOrder && !currentOrder.empty()) {
            for (const auto& orderLine : currentOrder) {
                cout << orderLine << endl;
            }
            cout << "----------------------------------------\n";
        }

        if (!foundDelivery) {
            cout << "No delivery history found for " << riderName << endl;
        }

        file.close();
    }

    void notifyRider(int orderID, const string& riderName) {
        ofstream notifFile("D:\\GIKI\\SEMESTER 03\\DSA\\Semester Project\\Dsa Project\\notifications.txt", ios::app);
        if (notifFile.is_open()) {
            notifFile << getCurrentTime() << " - New order #" << orderID << " assigned to " << riderName << "\n";
            notifFile.close();
        }
    }
};

// Initialize static member
MyOrder* MyOrder::instance = nullptr;