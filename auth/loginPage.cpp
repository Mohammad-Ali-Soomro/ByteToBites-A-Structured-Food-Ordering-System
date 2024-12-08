#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

// Function prototypes
void loginUser();
void registeration();
void loginRider();
void loginAdmin();
int getValidatedInput(int min, int max);
void riderMenu();
void userMenu();

// Helper function for file operations
bool validateCredentials(const string& filename, const string& username, const string& password);

// User login function
void loginUser() {
    cout << "\n--- User Login ---\n";
    string username, password;
    cout << "Enter Username: ";
    cin >> username;
    cout << "Enter Password: ";
    cin >> password;

    if (validateCredentials("users.txt", username, password)) {
        cout << "Login successful! Welcome, " << username << "!\n";
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

    if (validateCredentials("riders.txt", username, password)) {
        cout << "Login successful! Welcome, Rider " << username << "!\n";
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
    }
}

// Helper function to validate credentials
bool validateCredentials(const string& filename, const string& username, const string& password) {
    ifstream file(filename);
    string fileUsername, filePassword;
    while (file >> fileUsername >> filePassword) {
        if (fileUsername == username && filePassword == password) {
            return true;
        }
    }
    return false;
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

void userMenu()
{
    int choice;
    cout << "\n--- User Menu ---\n";
        cout << "1. Login (Existing Account)\n";
        cout << "2. Register (New Account)\n";
        cout << "Enter your choice: ";
        choice = getValidatedInput(1, 2);

        switch (choice) {
            case 1:
                loginUser();
                break;
            case 2:
                registeration();
                break;
        }
}

void riderMenu()
{
    int choice;
     cout << "\n--- Rider Menu ---\n";
        cout << "1. Login (Existing Account)\n";
        cout << "2. Register (New Account)\n";
        cout << "Enter your choice: ";
        choice = getValidatedInput(1, 2);

        switch (choice) {
            case 1:
                loginRider();
                break;
            case 2:
                registeration();
                break;
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

int main() {
    loginPage();
    return 0;
}
