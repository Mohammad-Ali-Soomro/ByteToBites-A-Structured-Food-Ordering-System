#include "auth/loginPage.h"
#include "menu/menu.h"
#include "orderStatus/orders.h"
#include "orderStatus/delivery.h"
#include <iostream>




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

    OrderList orderList;

    int choice;
    int orderId;

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
          //  int orderId=0;
            int statusInput;
            string customerName;
            cout << "Enter Order ID: ";
            cin >> orderId;
            cin.ignore();  // To consume newline left by cin
            cout << "Enter Customer Name: ";
            cin>>customerName;
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

}

void delivery()
{
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
}

int main()
{   
    loginPage();
    home();
    order();
    delivery();
    return 0;
}