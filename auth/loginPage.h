#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "../CurrentUserData.h"

using namespace std;

// Global user pointer
CurrentUser* currentUser = nullptr; // Initialize global pointer to store the logged-in user

const string USERS_FILE = "D:\\GIKI\\SEMESTER 03\\DSA\\Semester Project\\Dsa Project\\auth\\users.txt";
const string RIDERS_FILE = "D:\\GIKI\\SEMESTER 03\\DSA\\Semester Project\\Dsa Project\\auth\\riders.txt";
const string ADMINS_FILE = "D:\\GIKI\\SEMESTER 03\\DSA\\Semester Project\\Dsa Project\\auth\\admins.txt";

// Function prototypes
void loginUser();
void loginRider();
void loginAdmin();
void registerAccount();
int getValidatedInput(int min, int max);
void riderMenu();
void userMenu();
void loginPage();
bool validateCredentials(const string& filename, const string& username, const string& password);
void displayErrorMessage(const string& message);
bool validateFile(const string& filename);

// User login function
void loginUser() {
    cout << "\n--- User Login ---\n";
    string username, password;
    cout << "Enter Username: ";
    cin >> username;
    cout << "Enter Password: ";
    cin >> password;

    ifstream userFile(USERS_FILE);
    if (!validateFile(USERS_FILE)) return;

    string fileUsername, filePassword;
    double fileBalance;
    bool userFound = false;

    while (userFile >> fileUsername >> filePassword >> fileBalance) {
        if (fileUsername == username && filePassword == password) {
            userFound = true;
            if (currentUser) delete currentUser;
            currentUser = new CurrentUser(username, password, fileBalance);
            currentUser->role = 1;
            break;
        }
    }
    userFile.close();

    if (userFound) {
        cout << "Login successful! Welcome, " << username << "!\n";
        cout << "Your current balance: Rs. " << currentUser->getBalance() << endl;
    } else {
        displayErrorMessage("Invalid credentials or account not found.");
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

    if (!validateFile(RIDERS_FILE)) return;

    ifstream riderFile(RIDERS_FILE);
    string fileUsername, filePassword, route;
    vector<string> routes;
    bool riderFound = false;

    string line;
    while (getline(riderFile, line)) {
        istringstream iss(line);
        iss >> fileUsername >> filePassword;
        if (fileUsername == username && filePassword == password) {
            riderFound = true;
            while (iss >> route) routes.push_back(route);
            break;
        }
    }
    riderFile.close();

    if (riderFound) {
        cout << "Login successful! Welcome, Rider " << username << "!\n";
        if (currentUser) delete currentUser; // Clean up previous user
        currentUser = new CurrentRider(username, password, routes);
      //  riderMenu();
    } else {
        displayErrorMessage("Invalid credentials or account not found.");
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

    if (validateCredentials(ADMINS_FILE, username, password)) {
        cout << "Login successful! Welcome, Admin " << username << "!\n";
        if (currentUser) delete currentUser; // Clean up previous user
        currentUser = new CurrentAdmin(username, password);
    } else {
        displayErrorMessage("Invalid credentials or account not found.");
        loginPage();
    }
    
}

// Registration function for users and riders
void registerAccount() {
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

        ofstream userFile(USERS_FILE, ios::app);
        if (!validateFile(USERS_FILE)) return;
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

        ofstream riderFile(RIDERS_FILE, ios::app);
        if (!validateFile(RIDERS_FILE)) return;
        riderFile << username << " " << password;
        for (const auto& route : routes) riderFile << " " << route;
        riderFile << endl;
        riderFile.close();
        cout << "Rider account created successfully!\n";
       // riderMenu();
    }
}

// Utility functions
bool validateFile(const string& filename) {
    ifstream file(filename);
    if (!file) {
        displayErrorMessage("Error: Unable to open file " + filename + ".");
        return false;
    }
    file.close();
    return true;
}

bool validateCredentials(const string& filename, const string& username, const string& password) {
    ifstream file(filename);
    string fileUsername, filePassword;
    while (file >> fileUsername >> filePassword) {
        if (fileUsername == username && filePassword == password) return true;
    }
    return false;
}

void displayErrorMessage(const string& message) {
    cout << "Error: " << message << "\nReturning to the main menu...\n";
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

// User menu function
void userMenu() {
    cout << "\n--- User Menu ---\n";
    cout << "1. Login (Existing Account)\n";
    cout << "2. Register (New Account)\n";
    cout << "Enter your choice: ";
    int choice = getValidatedInput(1, 2);

    if (choice == 1) {
        loginUser();
    } else {
        registerAccount();
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
    } else {
        registerAccount();
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
    } else {
        loginAdmin();
    }
}