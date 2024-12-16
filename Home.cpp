#include "auth/loginPage.h"
#include "menu/menu.h"
#include "menu/track.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>





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



void chooseOrder() {
    // Vectors to hold customer and rider names
    vector<string> customers;
    vector<string> riders;

    // Read customers from users.txt
    ifstream userFile("auth/users.txt");
    if (!userFile) {
        cout << "Error: Unable to open users.txt file.\n";
        return;
    }
    string line, customerName;
    while (getline(userFile, line)) {
        istringstream iss(line);
        iss >> customerName; // First token is the username
        customers.push_back(customerName);
    }
    userFile.close();

    // Read riders from riders.txt
    ifstream riderFile("auth/riders.txt");
    if (!riderFile) {
        cout << "Error: Unable to open riders.txt file.\n";
        return;
    }
    string riderName;
    while (getline(riderFile, line)) {
        istringstream iss(line);
        iss >> riderName; // First token is the username
        riders.push_back(riderName);
    }
    riderFile.close();

    // Display available customers
    cout << "\nAvailable Customers:\n";
    for (size_t i = 0; i < customers.size(); ++i) {
        cout << i + 1 << ". " << customers[i] << "\n";
    }

    // Let user select a customer
    int customerChoice;
    cout << "Select a customer by number: ";
    cin >> customerChoice;
    while (customerChoice < 1 || customerChoice > static_cast<int>(customers.size())) {
        cout << "Invalid choice. Please select a valid customer number: ";
        cin >> customerChoice;
    }
    customerName = customers[customerChoice - 1];

    // Display available riders
    cout << "\nAvailable Riders:\n";
    for (size_t i = 0; i < riders.size(); ++i) {
        cout << i + 1 << ". " << riders[i] << "\n";
    }

    // Let user select a rider
    int riderChoice;
    cout << "Select a rider by number: ";
    cin >> riderChoice;
    while (riderChoice < 1 || riderChoice > static_cast<int>(riders.size())) {
        cout << "Invalid choice. Please select a valid rider number: ";
        cin >> riderChoice;
    }
    riderName = riders[riderChoice - 1];

    // Read menu items from menu.txt
    ifstream menuFile("menu/menu.txt");
    if (!menuFile) {
        cout << "Error: Unable to open menu.txt file.\n";
        return;
    }

    vector<pair<string, string>> menuItems; // To store item names and their details
    string itemName, details, category;
    int itemPrice, estimatedTime;
    string deliveryRoute;

    while (getline(menuFile, line)) {
        istringstream iss(line);
        string tempItemName, tempRoute;
        int tempPrice, tempTime;

        getline(iss, tempItemName, ',');
        iss >> tempPrice;
        iss.ignore();
        iss >> tempTime;
        iss.ignore();
        getline(iss, tempRoute, ',');
        getline(iss, category);

        menuItems.push_back({tempItemName, line}); // Store item name and the entire line
    }
    menuFile.close();

    // Display available menu items
    cout << "\nAvailable Menu Items:\n";
    for (size_t i = 0; i < menuItems.size(); ++i) {
        cout << i + 1 << ". " << menuItems[i].first << "\n";
    }

    // Let user select an item
    int itemChoice;
    cout << "Confirm your order by number: ";
    cin >> itemChoice;
    while (itemChoice < 1 || itemChoice > static_cast<int>(menuItems.size())) {
        cout << "Invalid choice. Please select a valid item number: ";
        cin >> itemChoice;
    }

    // Fetch selected item details
    istringstream itemDetailsStream(menuItems[itemChoice - 1].second);
    getline(itemDetailsStream, itemName, ',');
    itemDetailsStream >> itemPrice;
    itemDetailsStream.ignore();
    itemDetailsStream >> estimatedTime;
    itemDetailsStream.ignore(); 
    getline(itemDetailsStream, deliveryRoute, ',');

    // Log the order
    DeliveryTracking deliveryTracker;
    deliveryTracker.logDelivery(customerName, riderName, itemName, itemPrice, estimatedTime);
    deliveryTracker.updateRiderRoutes(riderName, deliveryRoute);    
    deliveryTracker.updateCustomerBalance(customerName, itemPrice);

    cout << "Order placed successfully!" << endl;
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



int main()
{   
    loginPage();
    home();
    return 0;
}