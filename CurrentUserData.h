#ifndef CURRENT_USER_DATA_H
#define CURRENT_USER_DATA_H

#include <iostream>
#include <vector>
using namespace std;

// Base class for current user
class CurrentUser {
protected:
    string password; // Password is kept protected for secure access

public:
    string username; // Public to allow direct username assignment
    int role;

    // Constructor
    CurrentUser(const string& username = "", const string& password = "")
        : username(username), password(password) {}

    // Getter for password
    string getPassword() const {
        return password;
    }

    // Setter for password
    void setPassword(const string& newPassword) {
        password = newPassword;
    }

    // Method to display user details
    virtual void displayData() const {
        cout << "Username: " << username << endl;
        // Password isn't displayed for security reasons
    }

    // Virtual destructor for safe polymorphic use
    virtual ~CurrentUser() {}
};

// Derived class for riders
class CurrentRider : public CurrentUser {
public:
    vector<string> routes; // Preferred routes

    // Constructor
    CurrentRider(const string& username = "", const string& password = "", const vector<string>& routes = {})
        : CurrentUser(username, password), routes(routes) {role=2;}

    // Method to display rider data
    void displayData() const override {
        CurrentUser::displayData();
        cout << "Preferred Routes: ";
        for (const auto& route : routes) {
            cout << route << " ";
        }
        cout << endl;
    }
};

// Derived class for customers
class CurrentCustomer : public CurrentUser {
public:
    int balance; // Account balance

    // Constructor
    CurrentCustomer(const string& username = "", const string& password = "", int balance = 0)
        : CurrentUser(username, password), balance(balance) {role=1;}

    // Method to display customer data
    void displayData() const override {
        CurrentUser::displayData();
        cout << "Account Balance: $" << balance << endl;
    }
};

// Derived class for admins
class CurrentAdmin : public CurrentUser {
public:
    // Constructor
    CurrentAdmin(const string& username = "", const string& password = "")
        : CurrentUser(username, password) {role=3;}

    // Method to display admin data
    void displayData() const override {
        CurrentUser::displayData();
        cout << "Role: Admin" << endl;
    }
};

#endif // CURRENT_USER_DATA_H
