#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;

// Base class for current user
class CurrentUser {
private:
    string password;
    double balance;

public:
    string username;
    int role;

    // Constructor with member initializer list to fix reorder warnings
    CurrentUser(const string& uname = "", const string& pwd = "", double bal = 0.0) 
        : password(pwd), balance(bal), username(uname), role(0) {}

    // Getter and setter for balance
    double getBalance() const { return balance; }
    void deductBalance(double amount) { 
        balance -= amount;
        updateBalanceInFile();  // Add this to persist balance changes
    }

    void setBalance(double newBalance) { 
        balance = newBalance; 
    }

    // Add method to update balance in file
    void updateBalanceInFile() {
        ifstream inFile("D:\\GIKI\\SEMESTER 03\\DSA\\Semester Project\\Dsa Project\\auth\\users.txt");
        ofstream outFile("D:\\GIKI\\SEMESTER 03\\DSA\\Semester Project\\Dsa Project\\auth\\users_temp.txt");
        
        string line;
        while (getline(inFile, line)) {
            istringstream iss(line);
            string uname, pwd;
            double bal;
            
            if (iss >> uname >> pwd >> bal) {
                if (uname == username) {
                    outFile << uname << " " << password << " " << balance << "\n";
                } else {
                    outFile << line << "\n";
                }
            }
        }
        
        inFile.close();
        outFile.close();
        
        remove("D:\\GIKI\\SEMESTER 03\\DSA\\Semester Project\\Dsa Project\\auth\\users.txt");
        rename("D:\\GIKI\\SEMESTER 03\\DSA\\Semester Project\\Dsa Project\\auth\\users_temp.txt", 
               "D:\\GIKI\\SEMESTER 03\\DSA\\Semester Project\\Dsa Project\\auth\\users.txt");
    }

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
        cout << "Current Balance: Rs. " << balance << endl;
        // Password isn't displayed for security reasons
    }

    // Virtual destructor for safe polymorphic use
    virtual ~CurrentUser() {}

    bool hasEnoughBalance(double amount) const {
        return balance >= amount;
    }
};

// Derived class for riders
class CurrentRider : public CurrentUser {
private:
    bool isAvailable;
    vector<string> routes;
    const string RIDERS_FILE = "D:\\GIKI\\SEMESTER 03\\DSA\\Semester Project\\Dsa Project\\auth\\riders.txt";

public:
    // Fix initialization order in constructor
    CurrentRider(const string& username = "", const string& password = "", const vector<string>& routes_ = {})
        : CurrentUser(username, password), 
          isAvailable(true),
          routes(routes_)
    {
        role = 2;
    }

    void setAvailability(bool available) {
        isAvailable = available;
        updateRiderStatus();
    }

    bool getAvailability() const {
        return isAvailable;
    }

    const vector<string>& getRoutes() const {
        return routes;
    }

    void displayData() const override {
        CurrentUser::displayData();
        cout << "Status: " << (isAvailable ? "Available" : "Not Available") << endl;
        cout << "Assigned Routes: ";
        for (const auto& route : routes) {
            cout << route << " ";
        }
        cout << endl;
    }

private:
    void updateRiderStatus() {
        ifstream inFile(RIDERS_FILE);
        ofstream outFile(RIDERS_FILE + ".temp");
        
        string line;
        while (getline(inFile, line)) {
            istringstream iss(line);
            string uname, pass;
            iss >> uname >> pass;
            
            if (uname == username) {
                outFile << username << " " << getPassword() << " " 
                       << (isAvailable ? "available" : "unavailable");
                for (const auto& route : routes) {
                    outFile << " " << route;
                }
                outFile << "\n";
            } else {
                outFile << line << "\n";
            }
        }
        
        inFile.close();
        outFile.close();
        
        remove(RIDERS_FILE.c_str());
        rename((RIDERS_FILE + ".temp").c_str(), RIDERS_FILE.c_str());
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

    // Add these methods to handle balance
    double getBalance() const { return balance; }
    
    void deductBalance(double amount) {
        balance -= amount;
        // You might want to save the updated balance to file here
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

