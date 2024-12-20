#include "auth/loginPage.h"
#include "menu/menu.h"
#include "orderStatus/orders.h"
#include "orderStatus/delivery.h"
#include "orderStatus/customer.h"
#include <iostream>



using namespace std;

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
        iss >> username >> password;

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

//fill this later
void chooseOrder()
{
   
}

void browseMenu() {
    Menu menu;

    // Load menu items from file
    if (!menu.loadMenu("menu/menu.txt")) {
        cout << "Failed to load the menu. Please check the file.\n";
        return;
    }

    // Display full menu
    menu.displayMenu();

    while (true) {
        cout << "\nChoose an action:\n";
        cout << "1. Manually choose an item to add to your order\n";
        cout << "2. Filter by type\n";
        cout << "3. Go back\n";
        cout << "Enter your choice: ";
        int choice = getValidatedInput(1, 3);

        if (choice == 3) {
            // Go back to the previous menu
            break;
        }

        if (choice == 2) {
            // Filter menu items by type
            cout << "\nEnter a type to filter (or type 'exit' to go back): ";
            string filterType;
            cin >> ws;
            getline(cin, filterType);

            if (filterType == "exit") {
                continue; // Restart the loop
            }

            // Get filtered items
            vector<MenuItem*> filteredItems = menu.displayMenuByType(filterType);

            if (!filteredItems.empty()) {
                // Allow user to choose an item from the filtered list
                cout << "\nChoose an item by index to add to your order (or 0 to skip): ";
                size_t index = getValidatedInput(0, filteredItems.size());

                if (index > 0 && index <= filteredItems.size()) {
                    MenuItem* selectedItem = filteredItems[index - 1];
                    cout << "You selected: ";
                    selectedItem->display();

                    // Call chooseOrder for further logic
                    chooseOrder();
                    // Placeholder: You can now add the selectedItem to an order list
                    // Example: orders.push_back(selectedItem);
                }
            }
        } else if (choice == 1) {
            // Manual selection
            cout << "\nEnter the index of the item to add to your order (or 0 to go back): ";
            size_t index = getValidatedInput(0, menu.getItems().size());

            if (index > 0 && index <= menu.getItems().size()) {
                MenuItem* selectedItem = &menu.getItems()[index - 1];
                cout << "You selected: ";
                selectedItem->display();

                // Call chooseOrder for further logic
                chooseOrder();
                // Placeholder: Add selectedItem to an order list
                // Example: orders.push_back(selectedItem);
            }
        }
    }
}


void customer()
{
    
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


void home() {
    while (true) {
        cout << "\nPlease choose an action:\n";
        cout << "1. View Info\n";
        cout << "2. Change Password\n";
        cout << "3. Logout\n";
        cout<<"4. Browse Menu\n";
        cout<<"5. Quit\n";
        cout << "Enter your choice: ";

        int choice;
        cin >> choice;

        switch (choice) {
            case 1:
                if (currentUser) {
                    currentUser->displayData();
                } else {
                    cout << "No user is logged in.\n";
                }
                break;

            case 2:
                changePassword();
                break;

            case 3:
                cout << "Logging out...\n";
                delete currentUser; // Clean up currentUser
                currentUser = nullptr;
                loginPage(); // Return to login page
                return;
            
            case 4:
                browseMenu();
                break;

            case 5:
                return;

            default:
                cout << "Invalid choice. Please try again.\n";
                home();
        }
    }
}

void order()
{
    MyOrder orderSystem;
    int choice;

    do {
        cout << "\n--- Order Delivery Module ---\n";
        cout << "1. Place an Order\n";
        cout << "2. View Order History\n";
        cout << "3. Cancel an Order\n";
        cout << "4. Track an Order\n";
        cout << "5. Update Order Status (Admin)\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                orderSystem.placeOrder();
                break;
            case 2:
                orderSystem.viewOrderHistory();
                break;
            case 3: {
                int orderID;
                cout << "Enter Order ID to cancel: ";
                cin >> orderID;
                orderSystem.cancelOrder(orderID);
                break;
            }
            case 4: {
                int orderID;
                cout << "Enter Order ID to track: ";
                cin >> orderID;
                orderSystem.trackOrder(orderID);
                break;
            }
            case 5: {
                int orderID;
                string newStatus;
                cout << "Enter Order ID to update: ";
                cin >> orderID;
                cin.ignore(); // Clear buffer
                cout << "Enter new status (e.g., Preparing, Dispatched, Delivered): ";
                getline(cin, newStatus);
                orderSystem.updateOrderStatus(orderID, newStatus);
                break;
            }
            case 6:
                cout << "Exiting the system. Goodbye!\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 6);

    
}


void delivery()
{
    // Initialize the delivery system with 5 locations
    Delivery deliverySystem(5);

    cout << "Welcome to the Delivery Management System!" << endl;

    int choice;
    while (true) {
        cout << "\nMenu:\n";
        cout << "1. Add Delivery Agent\n";
        cout << "2. Add Delivery Route\n";
        cout << "3. Assign Agent to Order\n";
        cout << "4. Complete an Order\n";
        cout << "5. Find Shortest Route\n";
        cout << "6. Display Agent Details\n";
        cout << "7. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                string agentName;
                cout << "Enter the name of the delivery agent: ";
                cin >> agentName;
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
                cout << "Enter the order ID to assign: ";
                cin >> orderID;
                deliverySystem.assignAgent(orderID);
                break;
            }
            case 4: {
                string agentName;
                cout << "Enter the name of the agent completing the order: ";
                cin >> agentName;
                deliverySystem.completeOrder(agentName);
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
                cout << "Exiting the system. Goodbye!\n";
                return;
            }
            default: {
                cout << "Invalid choice. Please try again.\n";
                break;
            }
        }
    }

}
  

int main()
{   
  //  loginPage();
  //  home();
   // order();
   // delivery();
   customer();
    return 0;
}