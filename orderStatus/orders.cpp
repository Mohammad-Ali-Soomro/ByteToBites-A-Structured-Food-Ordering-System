#include <iostream>
#include <cstring>
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
    char customerName[100];
    OrderStatus status;
    time_t timestamp;  // To store the order creation time

    Order(int orderId, const char* customerName, OrderStatus status) {
        this->orderId = orderId;
        strncpy(this->customerName, customerName, sizeof(this->customerName) - 1);
        this->customerName[sizeof(this->customerName) - 1] = '\0';
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



// Main function to interact with the system
int main() {
    OrderList orderList;

    int choice;
    do {
        cout << "\nOrder Management System\n";
        cout << "1. Add Order\n";
        cout << "2. Display Orders\n";
        cout << "3. Update Order Status\n";
        cout << "4. Delete Order\n";
        cout << "5. Search Order\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            int orderId, statusInput;
            char customerName[100];
            cout << "Enter Order ID: ";
            cin >> orderId;
            cin.ignore();  // To consume newline left by cin
            cout << "Enter Customer Name: ";
            cin.getline(customerName, 100);
            cout << "Enter Order Status (1: Preparing, 2: Dispatched, 3: On the Way, 4: Delivered): ";
            cin >> statusInput;

            OrderStatus status = getOrderStatusFromInput(statusInput);
            Order newOrder(orderId, customerName, status);
            orderList.insertOrder(newOrder);
            break;
        }
        case 2:
            cout << "\nDisplaying All Orders:\n";
            orderList.displayOrders();
            break;
        case 3: {
            int orderId, statusInput;
            cout << "Enter Order ID to Update: ";
            cin >> orderId;
            cout << "Enter New Status (1: Preparing, 2: Dispatched, 3: On the Way, 4: Delivered): ";
            cin >> statusInput;

            OrderStatus newStatus = getOrderStatusFromInput(statusInput);
            orderList.updateOrderStatus(orderId, newStatus);
            break;
        }
        case 4: {
            int orderId;
            cout << "Enter Order ID to Delete: ";
            cin >> orderId;
            orderList.deleteOrder(orderId);
            break;
        }
        case 5: {
            int orderId;
            cout << "Enter Order ID to Search: ";
            cin >> orderId;
            orderList.searchOrder(orderId);
            break;
        }
        case 6:
            cout << "Exiting the system.\n";
            break;
        default:
            cout << "Invalid choice! Try again.\n";
        }
    } while (choice != 6);

    return 0;
}