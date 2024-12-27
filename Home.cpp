#include "auth/loginPage.h"
#include "menu/menu.h"
#include "orderStatus/orders.h"
#include "orderStatus/delivery.h"
#include "orderStatus/customer.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>  //update
#include <ctime>
#include <limits>
#include <map>
#include <algorithm>

using namespace std;

// Function declarations
void changePassword();
void chooseOrder();
void browseMenu();
void customer();
void home();
void order();
void delivery();
void mainMenu();
void addMenuItem();
void generateBill();
string getCurrentTime();
void riderDashboard();
void adminDashboard();
void manageRiders();
void viewSystemStats();
template<typename T>
T getInput(const string& prompt);
void checkNotifications();
void viewCustomerReports();
void displayAllRiders();
void manageMenu();
void addNewRider();
void viewRiderPerformance();

// Function to change password
void changePassword() {
    if (!currentUser) {
        cout << "Error: No user is currently logged in.\n";
        return;
    }

    string newPassword;
    cout << "Enter your new password: ";
    cin >> newPassword;

    string filename, tempFilename;
    if (currentUser->role == 1) {
        filename = USERS_FILE;
        tempFilename = "temp_users.txt";
    } else if (currentUser->role == 2) {
        filename = RIDERS_FILE;
        tempFilename = "temp_riders.txt";
    } else {
        cout << "Error: Unsupported role.\n";
        return;
    }

    ifstream file(filename);
    ofstream tempFile(tempFilename);

    if (!file || !tempFile) {
        cout << "Error: Unable to open files.\n";
        return;
    }

    string line;
    bool updated = false;

    // Process each line in the file
    while (getline(file, line)) {
        istringstream iss(line);
        string username, password;

        if (username == currentUser->username) {
            // Update password for the current user
            tempFile << username << " " << newPassword;

            // Copy additional data (like balance or routes) if it exists
            string extra;
            while (iss >> extra) {
                tempFile << " " << extra;
            }
            tempFile << "\n";
            updated = true;
        } else {
            // Copy the original line for other users
            tempFile << line << "\n";
        }
    }

    file.close();
    tempFile.close();

    // Replace the original file with the updated file
    if (updated) {
        remove(filename.c_str());
        rename(tempFilename.c_str(), filename.c_str());
        cout << "Password updated successfully.\n";
        currentUser->setPassword(newPassword);  // Update the currentUser object
    } else {
        cout << "Error: User not found in the file.\n";
        remove(tempFilename.c_str()); // Clean up temporary file
    }
}

// Function to choose an order
void chooseOrder() {
    // Placeholder for order selection logic
}

// Function to browse the menu
void browseMenu() {
    Menu menu;
    MyOrder& orderSystem = MyOrder::getInstance();

    if (!menu.loadMenu("D:\\GIKI\\SEMESTER 03\\DSA\\Semester Project\\Dsa Project\\menu\\menu.txt")) {
        cout << "Failed to load the menu. Please check the file.\n";
        return;
    }

    while (true) {
        cout << "\n=== Menu Options ===\n";
        cout << "Your current balance: Rs. " << currentUser->getBalance() << endl;
        cout << "1. View Full Menu\n";
        cout << "2. Filter Menu by Type\n";
        cout << "3. View Current Order\n";
        cout << "4. Place Order\n";
        cout << "5. Go Back\n";
        cout << "Enter your choice: ";
        
        int choice = getValidatedInput(1, 5);

        switch (choice) {
            case 1: {
                menu.displayMenu();
                cout << "\nWould you like to add an item? (1: Yes, 0: No): ";
                if (getValidatedInput(0, 1)) {
                    cout << "Enter item number: ";
                    size_t index = getValidatedInput(1, menu.getItems().size());
                    const MenuItem& item = menu.getItems()[index - 1];
                    orderSystem.addItemToOrder(item.getName(), item.getPrice());
                }
                break;
            }
            case 2: {
                cout << "Enter food type (e.g., Burger, Pizza, Drink): ";
                string type;
                cin.ignore();
                getline(cin, type);
                vector<MenuItem*> filtered = menu.displayMenuByType(type);
                if (!filtered.empty()) {
                    cout << "\nWould you like to add an item? (1: Yes, 0: No): ";
                    if (getValidatedInput(0, 1)) {
                        cout << "Enter item number: ";
                        size_t index = getValidatedInput(1, filtered.size());
                        MenuItem* item = filtered[index - 1];
                        orderSystem.addItemToOrder(item->getName(), item->getPrice());
                    }
                }
                break;
            }
            case 3: {
                double total = orderSystem.getCurrentOrderTotal();
                if (total == 0) {
                    cout << "Your order is empty.\n";
                } else {
                    cout << "\n=== Current Order ===\n";
                    for (const auto& item : orderSystem.getCurrentOrderItems()) {
                        cout << item.first << " - Rs. " << item.second << endl;
                    }
                    cout << "\nSubtotal: Rs. " << total;
                    cout << "\nTotal with tax (5%): Rs. " << (total * 1.05) << endl;
                }
                break;
            }
            case 4: {
                if (orderSystem.getCurrentOrderTotal() == 0) {
                    cout << "Cannot place empty order. Please add items first.\n";
                    break;
                }
                
                double totalWithTax = orderSystem.getCurrentOrderTotal() * 1.05;
                cout << "\nOrder Summary:\n";
                cout << "Subtotal: Rs. " << orderSystem.getCurrentOrderTotal() << endl;
                cout << "Tax (5%): Rs. " << (orderSystem.getCurrentOrderTotal() * 0.05) << endl;
                cout << "Total with tax: Rs. " << totalWithTax << endl;
                cout << "Your balance: Rs. " << currentUser->getBalance() << endl;

                if (totalWithTax > currentUser->getBalance()) {
                    cout << "\nInsufficient balance!\n";
                    cout << "Required amount: Rs. " << totalWithTax << endl;
                    cout << "Your balance: Rs. " << currentUser->getBalance() << endl;
                    cout << "Missing: Rs. " << (totalWithTax - currentUser->getBalance()) << endl;
                    break;
                }

                cout << "\nProceed with order? (1: Yes, 0: No): ";
                if (getValidatedInput(0, 1)) {
                    if (orderSystem.placeOrder(currentUser->username)) {
                        currentUser->deductBalance(totalWithTax);
                        cout << "Order placed successfully!\n";
                        cout << "New balance: Rs. " << currentUser->getBalance() << endl;
                        return;
                    }
                }
                break;
            }
            case 5:
                return;
        }
    }
}

// Function to manage customers
void customer() {
    if (currentUser->role != 3) {
        cout << "Error: Only admins can manage customers.\n";
        return;
    }

    CustomerManagement cm;
    int choice, id;
    string name, contact;

    do {
        cout << "\n--- Customer Management Module ---\n";
        cout << "1. Add Customer\n";
        cout << "2. Search Customer\n";
        cout << "3. Display All Customers\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            cout << "Enter Customer ID: ";
            cin >> id;
            cout << "Enter Name: ";
            cin.ignore();
            getline(cin, name);
            cout << "Enter Contact: ";
            getline(cin, contact);
            cm.addCustomer(id, name, contact);
            break;
        case 2:
            cout << "Enter Customer ID to Search: ";
            cin >> id;
            cm.searchCustomer(id);
            break;
        case 3:
            cm.displayAllCustomers();
            break;
        case 4:
            cout << "Exiting...\n";
            break;
        default:
            cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 4);
}

// Function to manage orders
void order() {
    MyOrder& orderSystem = MyOrder::getInstance();
    
    while (true) {
        cout << "\n=== Order Management ===\n";
        cout << "1. Browse Menu & Place Order\n";
        cout << "2. View Order History\n";
        cout << "3. Track Active Order\n";
        cout << "4. View Bill\n";
        cout << "5. Back to Main Menu\n";
        cout << "Enter choice: ";
        
        int choice = getValidatedInput(1, 5);
        
        switch (choice) {
            case 1:
                browseMenu();
                break;
            case 2:
                orderSystem.viewOrderHistory(currentUser->username);
                break;
            case 3: {
                cout << "Enter your Order ID to track: ";
                int orderID;
                cin >> orderID;
                cout << "\nTracking Order #" << orderID << "...\n";
                orderSystem.trackOrder(orderID);
                break;
            }
            case 4:
                orderSystem.displayBill(currentUser->username);
                break;
            case 5:
                return;
        }
    }
}

// Function to manage deliveries
void delivery() {
    if (currentUser->role != 3) {
        cout << "Error: Only admins can manage deliveries.\n";
        return;
    }

    Delivery deliverySystem(5);
    MyOrder& orderSystem = MyOrder::getInstance();

    while (true) {
        cout << "\n=== Delivery Management System ===\n";
        cout << "1. Add Delivery Agent\n";
        cout << "2. Add Delivery Route\n";
        cout << "3. Assign Agent to Order\n";
        cout << "4. Update Order Status\n";
        cout << "5. Find Shortest Route\n";
        cout << "6. Display Agent Details\n";
        cout << "7. View All Orders\n";
        cout << "8. Back to Main Menu\n";
        cout << "Enter your choice: ";

        int choice = getValidatedInput(1, 8);

        switch (choice) {
            case 1: {
                string agentName;
                cout << "Enter the name of the delivery agent: ";
                cin.ignore();
                getline(cin, agentName);
                deliverySystem.addDeliveryAgent(agentName);
                break;
            }
            case 2: {
                int from, to, distance;
                cout << "Enter starting location (0-4): ";
                cin >> from;
                cout << "Enter destination location (0-4): ";
                cin >> to;
                cout << "Enter distance between locations: ";
                cin >> distance;
                deliverySystem.addDeliveryRoute(from, to, distance);
                break;
            }
            case 3: {
                int orderID;
                string agentName;
                cout << "Enter Order ID: ";
                cin >> orderID;
                cout << "Enter Delivery Agent Name: ";
                cin.ignore();
                getline(cin, agentName);
                
                if (deliverySystem.assignAgent(orderID, agentName)) {
                    orderSystem.updateOrderStatus(orderID, "Assigned to Rider", agentName);
                }
                break;
            }
            case 4: {
                int orderID;
                cout << "Enter Order ID: ";
                cin >> orderID;
                
                cout << "\nChoose new status:\n";
                cout << "1. Preparing\n";
                cout << "2. Out for Delivery\n";
                cout << "3. Delivered\n";
                cout << "4. Cancelled\n";
                
                int statusChoice = getValidatedInput(1, 4);
                string newStatus;
                
                switch (statusChoice) {
                    case 1: newStatus = "Preparing"; break;
                    case 2: newStatus = "Out for Delivery"; break;
                    case 3: newStatus = "Delivered"; break;
                    case 4: newStatus = "Cancelled"; break;
                }
                
                string riderName;
                if (statusChoice == 2) {
                    cout << "Enter rider name: ";
                    cin.ignore();
                    getline(cin, riderName);
                }
                
                orderSystem.updateOrderStatus(orderID, newStatus, riderName);
                break;
            }
            case 5: {
                int start, end;
                cout << "Enter starting location (0-4): ";
                cin >> start;
                cout << "Enter destination location (0-4): ";
                cin >> end;
                deliverySystem.findShortestRoute(start, end);
                break;
            }
            case 6: {
                deliverySystem.displayAgentDetails();
                break;
            }
            case 7: {
                orderSystem.viewAllOrders();
                break;
            }
            case 8:
                return;
        }
    }
}

// Function to add a new menu item (Admin only)
void addMenuItem() {
    if (currentUser && currentUser->role == 3) { // Check if the current user is an admin
        string name, type;
        int price, estimatedTime;
        char location;

        cout << "Enter the name of the new menu item: ";
        cin.ignore();
        getline(cin, name);
        cout << "Enter the price of the new menu item: ";
        cin >> price;
        cout << "Enter the estimated time (in minutes) to prepare the new menu item: ";
        cin >> estimatedTime;
        cout << "Enter the location (e.g., 'A', 'B', 'C') of the new menu item: ";
        cin >> location;
        cout << "Enter the type (category) of the new menu item: ";
        cin.ignore();
        getline(cin, type);

        ofstream menuFile("D:\\GIKI\\SEMESTER 03\\DSA\\Semester Project\\Dsa Project\\menu\\menu.txt", ios::app);
        if (menuFile.is_open()) {
            menuFile << name << "," << price << "," << estimatedTime << "," << location << "," << type << endl;
            menuFile.close();
            cout << "New menu item added successfully!\n";
        } else {
            cout << "Failed to open the menu file.\n";
        }
    } else {
        cout << "Error: Only admins can add new menu items.\n";
    }
}

// Function to generate a total bill for the user
void generateBill() {
    if (!currentUser || currentUser->role != 1) {
        cout << "Error: Only customers can generate a bill.\n";
        return;
    }

    MyOrder& orderSystem = MyOrder::getInstance();
    
    // Try to get the most recent order for this user
    if (!orderSystem.loadMostRecentOrderForUser(currentUser->username)) {
        cout << "No orders found for your account.\n";
        return;
    }

    orderSystem.displayBill(currentUser->username);
}

// Function to display the home menu
void home() {
    while (true) {
        cout << "\n=== Main Menu ===\n";
        cout << "1. View Info\n";
        cout << "2. Change Password\n";
        cout << "3. Logout\n";
        
        if (currentUser->role == 1) {  // Customer
            cout << "4. Manage Orders\n";
            cout << "5. Generate Bill\n";
        }
        else if (currentUser->role == 2) {  // Rider
            cout << "4. Rider Dashboard\n";
        }
        else if (currentUser->role == 3) {  // Admin
            cout << "4. Manage Orders\n";
            cout << "5. Manage Deliveries\n";
            cout << "6. Manage Customers\n";
            cout << "7. Add Menu Item\n";
        }
        
        cout << "8. Return to Login\n";
        cout << "9. Exit Application\n";
        cout << "Enter your choice: ";

        int choice = getValidatedInput(1, 9);

        switch (choice) {
            case 1:
                if (currentUser) {
                    currentUser->displayData();
                }
                break;
            case 2:
                changePassword();
                break;
            case 3:
                cout << "Logging out...\n";
                delete currentUser;
                currentUser = nullptr;
                return;
            case 4:
                if (currentUser->role == 1) {
                    order();
                }
                else if (currentUser->role == 2) {
                    riderDashboard();
                }
                else if (currentUser->role == 3) {
                    order();
                }
                break;
            case 8:
                cout << "Returning to login screen...\n";
                delete currentUser;
                currentUser = nullptr;
                return;
            case 9:
                cout << "Thank you for using our service. Goodbye!\n";
                delete currentUser;
                currentUser = nullptr;
                exit(0);
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }
}

// Main function to display the main menu
void mainMenu() {
    bool running = true;
    while (running) {
        try {
            cout << "\n=== Welcome to Food Delivery System ===\n";
            cout << "1. Login/Register\n";
            cout << "2. Exit Application\n";
            cout << "Enter your choice: ";
            
            int choice = getValidatedInput(1, 2);
            
            switch (choice) {
                case 1:
                    loginPage();
                    if (currentUser) {
                        checkNotifications();  // Check for notifications after login
                        switch (currentUser->role) {
                            case 1: // Customer
                                home();
                                break;
                            case 2: // Rider
                                riderDashboard();
                                break;
                            case 3: // Admin
                                adminDashboard();
                                break;
                        }
                    }
                    break;
                case 2:
                    cout << "Thank you for using our service. Goodbye!\n";
                    running = false;
                    break;
            }
        }
        catch (const exception& e) {
            cout << "An error occurred: " << e.what() << endl;
            cout << "Please try again.\n";
        }
    }
}

string getCurrentTime() {
    time_t now = time(0);
    char* dt = ctime(&now);
    string timestamp = dt;
    timestamp.pop_back(); // Remove newline character
    return timestamp;
}

void riderDashboard() {
    if (currentUser->role != 2) {
        cout << "Error: Access denied. Only riders can access this menu.\n";
        return;
    }

    MyOrder& orderSystem = MyOrder::getInstance();
    
    while (true) {
        cout << "\n=== Rider Dashboard ===\n";
        cout << "1. View Assigned Orders\n";
        cout << "2. Update Delivery Status\n";
        cout << "3. View Delivery History\n";
        cout << "4. View Route Information\n";
        cout << "5. Update Availability\n";
        cout << "6. Return to Main Menu\n";
        cout << "7. Return to Login\n";
        cout << "8. Exit Application\n";
        cout << "Enter choice: ";

        int choice = getValidatedInput(1, 8);

        switch (choice) {
            case 1: {
                orderSystem.viewAssignedOrders(currentUser->username);
                break;
            }
            case 2: {
                cout << "Enter Order ID: ";
                int orderID;
                cin >> orderID;
                
                cout << "\nUpdate delivery status:\n";
                cout << "1. Picked up from restaurant\n";
                cout << "2. On the way\n";
                cout << "3. Delivered\n";
                cout << "Enter choice: ";
                
                int statusChoice = getValidatedInput(1, 3);
                string newStatus;
                string comment;
                
                switch (statusChoice) {
                    case 1: 
                        newStatus = "Picked up - On the way";
                        break;
                    case 2: 
                        newStatus = "Near delivery location";
                        break;
                    case 3: {
                        newStatus = "Delivered";
                        cout << "Add delivery confirmation note: ";
                        cin.ignore();
                        getline(cin, comment);
                        break;
                    }
                }
                
                orderSystem.updateDeliveryStatus(orderID, newStatus, currentUser->username, comment);
                break;
            }
            case 3: {
                orderSystem.viewRiderDeliveryHistory(currentUser->username);
                break;
            }
            case 4: {
                cout << "\nDelivery Routes for " << currentUser->username << ":\n";
                for (const auto& route : ((CurrentRider*)currentUser)->getRoutes()) {
                    cout << "- " << route << endl;
                }
                break;
            }
            case 5: {
                cout << "Set availability:\n";
                cout << "1. Available\n";
                cout << "2. Not Available\n";
                int availChoice = getValidatedInput(1, 2);
                ((CurrentRider*)currentUser)->setAvailability(availChoice == 1);
                cout << "Availability updated successfully!\n";
                break;
            }
            case 6:
                return;
            case 7:
                cout << "Returning to login screen...\n";
                delete currentUser;
                currentUser = nullptr;
                return;
            case 8:
                cout << "Thank you for using our service. Goodbye!\n";
                delete currentUser;
                currentUser = nullptr;
                exit(0);
        }
    }
}

void adminDashboard() {
    if (currentUser->role != 3) {
        cout << "Error: Access denied. Only admins can access this dashboard.\n";
        return;
    }

    while (true) {
        cout << "\n=== Admin Dashboard ===\n";
        cout << "1. View All Orders\n";
        cout << "2. Manage Riders\n";
        cout << "3. View System Statistics\n";
        cout << "4. Manage Menu\n";
        cout << "5. View Customer Reports\n";
        cout << "6. Return to Main Menu\n";
        cout << "7. Exit\n";
        cout << "Enter choice: ";

        int choice = getValidatedInput(1, 7);

        switch (choice) {
            case 1: {
                MyOrder::getInstance().viewAllOrders();
                break;
            }
            case 2:
                manageRiders();
                break;
            case 3:
                viewSystemStats();
                break;
            case 4:
                manageMenu();
                break;
            case 5:
                viewCustomerReports();
                break;
            case 6:
                return;
            case 7:
                cout << "Thank you for using the system. Goodbye!\n";
                exit(0);
        }
    }
}

void manageRiders() {
    cout << "\n=== Rider Management ===\n";
    cout << "1. View All Riders\n";
    cout << "2. Add New Rider\n";
    cout << "3. View Rider Performance\n";
    cout << "4. Back\n";
    
    int choice = getValidatedInput(1, 4);
    
    switch (choice) {
        case 1:
            displayAllRiders();
            break;
        case 2:
            addNewRider();
            break;
        case 3:
            viewRiderPerformance();
            break;
        case 4:
            return;
    }
}

void viewSystemStats() {
    ifstream statsFile("D:\\GIKI\\SEMESTER 03\\DSA\\Semester Project\\Dsa Project\\stats.txt");
    if (!statsFile.is_open()) {
        cout << "No statistics available.\n";
        return;
    }

    double totalRevenue = 0;
    int totalOrders = 0;
    map<string, int> userOrders;

    string line;
    while (getline(statsFile, line)) {
        istringstream iss(line);
        string timestamp, username, amount;
        if (getline(iss, timestamp, ',') && 
            getline(iss, username, ',') && 
            getline(iss, amount, ',')) {
            
            totalRevenue += stod(amount);
            totalOrders++;
            userOrders[username]++;
        }
    }

    cout << "\n=== System Statistics ===\n";
    cout << "Total Orders: " << totalOrders << endl;
    cout << "Total Revenue: Rs. " << fixed << setprecision(2) << totalRevenue << endl;
    cout << "Average Order Value: Rs. " << (totalOrders > 0 ? totalRevenue/totalOrders : 0) << endl;
    
    cout << "\nTop Customers:\n";
    vector<pair<string, int>> sortedUsers(userOrders.begin(), userOrders.end());
    sort(sortedUsers.begin(), sortedUsers.end(), 
         [](const auto& a, const auto& b) { return a.second > b.second; });
    
    for (int i = 0; i < min(5, (int)sortedUsers.size()); i++) {
        cout << i+1 << ". " << sortedUsers[i].first 
             << " (" << sortedUsers[i].second << " orders)\n";
    }
}

template<typename T>
T getInput(const string& prompt) {
    T value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please try again.\n";
    }
}

void checkNotifications() {
    ifstream notifFile("D:\\GIKI\\SEMESTER 03\\DSA\\Semester Project\\Dsa Project\\notifications.txt");
    if (notifFile.is_open()) {
        string line;
        vector<string> notifications;
        
        while (getline(notifFile, line)) {
            if (line.find(currentUser->username) != string::npos) {
                notifications.push_back(line);
            }
        }
        
        if (!notifications.empty()) {
            cout << "\n=== New Notifications ===\n";
            for (const auto& notif : notifications) {
                cout << notif << endl;
            }
            cout << "=====================\n";
        }
    }
}

void viewCustomerReports() {
    ifstream file("D:\\GIKI\\SEMESTER 03\\DSA\\Semester Project\\Dsa Project\\auth\\users.txt");
    if (!file.is_open()) {
        cout << "Cannot access user data.\n";
        return;
    }

    cout << "\n=== Customer Reports ===\n";
    string username, password;
    double balance;
    
    while (file >> username >> password >> balance) {
        cout << "Username: " << username << endl;
        cout << "Current Balance: Rs. " << balance << endl;
        MyOrder::getInstance().viewOrderHistory(username);
        cout << "------------------------\n";
    }
    file.close();
}

void displayAllRiders() {
    ifstream file(RIDERS_FILE);
    if (!file.is_open()) {
        cout << "Cannot access rider data.\n";
        return;
    }

    cout << "\n=== All Riders ===\n";
    string username, password, status;
    while (file >> username >> password >> status) {
        cout << "Rider: " << username << endl;
        cout << "Status: " << status << endl;
        cout << "----------------\n";
    }
    file.close();
}

void manageMenu() {
    cout << "\n=== Menu Management ===\n";
    cout << "1. Add New Item\n";
    cout << "2. View All Items\n";
    cout << "3. Back\n";
    
    int choice = getValidatedInput(1, 3);
    switch (choice) {
        case 1:
            addMenuItem();
            break;
        case 2:
            Menu().displayMenu();
            break;
        case 3:
            return;
    }
}

void addNewRider() {
    string username, password;
    cout << "Enter rider username: ";
    cin >> username;
    cout << "Enter rider password: ";
    cin >> password;

    ofstream file(RIDERS_FILE, ios::app);
    if (file.is_open()) {
        file << username << " " << password << " available\n";
        cout << "Rider added successfully!\n";
        file.close();
    }
}

void viewRiderPerformance() {
    MyOrder& orderSystem = MyOrder::getInstance();
    cout << "\n=== Rider Performance ===\n";
    
    ifstream file(RIDERS_FILE);
    string username, password, status;
    
    while (file >> username >> password >> status) {
        cout << "\nRider: " << username << endl;
        cout << "Status: " << status << endl;
        orderSystem.viewRiderDeliveryHistory(username);
    }
    file.close();
}

int main() {
    mainMenu();
    return 0;
}