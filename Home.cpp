#include "auth/loginPage.h"
#include "menu/menu.h"
#include <iostream>


//doing this to merge branches meow


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



int main()
{   
    loginPage();
    home();
    return 0;
}