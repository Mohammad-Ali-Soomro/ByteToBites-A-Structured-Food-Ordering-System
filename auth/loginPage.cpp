#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "../CurrentUserData.h"

using namespace std;


//global user pointer       
CurrentUser* currentUser = nullptr; // Global pointer to store the logged-in user


// Function prototypes
void loginUser();
void registeration();
void loginRider();
void loginAdmin();
int getValidatedInput(int min, int max);
void riderMenu();
void userMenu();
void loginPage();
bool validateCredentials(const string& filename, const string& username, const string& password);

// User login function
void loginUser() {
    cout << "\n--- User Login ---\n";
    string username, password;
    cout << "Enter Username: ";
    cin >> username;
    cout << "Enter Password: ";
    cin >> password;

    double balance;
    ifstream userFile("users.txt");
    string fileUsername, filePassword;
    double fileBalance;

    bool userFound = false;
    while (userFile >> fileUsername >> filePassword >> fileBalance) {
        if (fileUsername == username && filePassword == password) {
            userFound = true;
            balance = fileBalance;
            break;
        }
    }
    userFile.close();

    if (userFound) {
        cout << "Login successful! Welcome, " << username << "!\n";
       

        // Create a CurrentCustomer object and store in currentUser
        if (currentUser) delete currentUser; // Clean up previous user
        currentUser = new CurrentCustomer(username, password, balance);

    
    } else {
        cout << "Account not found or invalid credentials.\n";
        cout << "Returning to User Menu...\n";
        userMenu();
    }
}


// Rider login function
void loginRider() {
    cout << "\n--- Rider Login ---\n";
    string username, password;
    cout << "Enter Username: ";
    cin >> username;
    cout << "Enter Password: ";
    cin >> password;

    ifstream riderFile("riders.txt");
    string fileUsername, filePassword, route;
    vector<string> routes;
    bool riderFound = false;

    string line;
    while (getline(riderFile, line)) {
        istringstream iss(line);
        iss >> fileUsername >> filePassword;
        if (fileUsername == username && filePassword == password) {
            riderFound = true;
            while (iss >> route) {
                routes.push_back(route);
            }
            break;
        }
    }
    riderFile.close();

    if (riderFound) {
        cout << "Login successful! Welcome, Rider " << username << "!\n";
        

        // Create a CurrentRider object and store in currentUser
        if (currentUser) delete currentUser; // Clean up previous user
        currentUser = new CurrentRider(username, password, routes);

 
    } else {
        cout << "Account not found or invalid credentials.\n";
        cout << "Returning to Rider Menu...\n";
        riderMenu();
    }
}


// Admin login function
void loginAdmin() {
    cout << "\n--- Admin Login ---\n";
    string username, password;
    cout << "Enter Admin Username: ";
    cin >> username;
    cout << "Enter Admin Password: ";
    cin >> password;

    if (validateCredentials("admins.txt", username, password)) {
        cout << "Login successful! Welcome, Admin " << username << "!\n";

        // Create a CurrentAdmin object and store in currentUser
        if (currentUser) delete currentUser; // Clean up previous user
        currentUser = new CurrentAdmin(username, password);

        // Display admin data
        currentUser->displayData();
    } else {
        cout << "Account not found or invalid credentials.\n";
        cout << "Please contact tech support. Returning to Login Page\n";
        loginPage();
    }
}


// Registration function for both users and riders
void registeration() {
    cout << "\n--- Registration ---\n";
    cout << "1. Register as User\n";
    cout << "2. Register as Rider\n";
    cout << "Enter your choice: ";
    int choice = getValidatedInput(1, 2);

    string username, password;
    cout << "Enter a Username: ";
    cin >> username;
    cout << "Enter a Password: ";
    cin >> password;

    if (choice == 1) { // Register as user
        double balance;
        cout << "Enter the amount to top up in your account: ";
        cin >> balance;

        ofstream userFile("users.txt", ios::app);
        userFile << username << " " << password << " " << balance << endl;
        userFile.close();
        cout << "User account created successfully!\n";
        userMenu();

    } else if (choice == 2) { // Register as rider
        int routeCount;
        cout << "Enter the number of preferred routes: ";
        cin >> routeCount;
        vector<string> routes(routeCount);
        cout << "Enter your preferred routes:\n";
        for (int i = 0; i < routeCount; ++i) {
            cout << "Route " << i + 1 << ": ";
            cin >> routes[i];
        }

        ofstream riderFile("riders.txt", ios::app);
        riderFile << username << " " << password;
        for (const auto& route : routes) {
            riderFile << " " << route;
        }
        riderFile << endl;
        riderFile.close();
        cout << "Rider account created successfully!\n";
        riderMenu();
    }
}

// Input validation function
int getValidatedInput(int min, int max) {
    int choice;
    cin >> choice;
    while (cin.fail() || choice < min || choice > max) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Invalid choice. Please enter a number between " << min << " and " << max << ": ";
        cin >> choice;
    }
    return choice;
}
bool validateCredentials(const string& filename, const string& username, const string& password) {
    ifstream file(filename);
    if (!file) {
        cout << "Error: Unable to open file " << filename << ".\n";
        return false;
    }

    string fileUsername, filePassword;
    while (file >> fileUsername >> filePassword) {
        if (fileUsername == username && filePassword == password) {
            return true;
        }
    }
    return false;
}


// User menu function
void userMenu() {
    cout << "\n--- User Menu ---\n";
    cout << "1. Login (Existing Account)\n";
    cout << "2. Register (New Account)\n";
    cout << "Enter your choice: ";
    int choice = getValidatedInput(1, 2);

    if (choice == 1) {
        loginUser();
    } else if (choice == 2) {
        registeration();
    }
}

// Rider menu function
void riderMenu() {
    cout << "\n--- Rider Menu ---\n";
    cout << "1. Login (Existing Account)\n";
    cout << "2. Register (New Account)\n";
    cout << "Enter your choice: ";
    int choice = getValidatedInput(1, 2);

    if (choice == 1) {
        loginRider();
    } else if (choice == 2) {
        registeration();
    }
}

// Main menu function
void loginPage() {
    cout << "======================================\n";
    cout << "   Welcome to Bytes to Bits Delivery\n";
    cout << "======================================\n";
    cout << "Please choose your preferred login method:\n";
    cout << "1. User\n";
    cout << "2. Rider\n";
    cout << "3. Admin\n";
    cout << "--------------------------------------\n";

    int choice = getValidatedInput(1, 3);

    if (choice == 1) {
        userMenu();
    } else if (choice == 2) {
        riderMenu();
    } else if (choice == 3) {
        loginAdmin();
    }
}

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
        filename = "users.txt";
        tempFilename = "temp_users.txt";
    } else if (currentUser->role == 2) {
        filename = "riders.txt";
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


void home() {
    while (true) {
        cout << "\nPlease choose an action:\n";
        cout << "1. View Info\n";
        cout << "2. Change Password\n";
        cout << "3. Logout\n";
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

            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }
}


// Main function
int main() {
    loginPage();
    home();
    return 0;
}
