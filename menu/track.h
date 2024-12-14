#ifndef TRACK_H
#define TRACK_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstring>
#include "../CurrentUserData.h"
#include "menu.h"

using namespace std;

class DeliveryTracking {
private:
    string deliveryLogFile;

public:
    // Constructor
    DeliveryTracking(const string& logFile = "auth/delivery_log.txt")
        : deliveryLogFile(logFile) {}

    // Log delivery details
    void logDelivery(const string& customer, const string& rider, const string& item, int price, int estimatedTime) {
        ofstream logFile("delivery_Log.txt", ios::app);
        if (!logFile) {
            cerr << "Error: Unable to open delivery log file." << endl;
            return;
        }

        time_t now = time(nullptr);
        char* dt = ctime(&now);
        dt[strlen(dt) - 1] = '\0'; // Remove newline

        logFile << "Customer: " << customer
                << ", Rider: " << rider
                << ", Item: " << item
                << ", Price: Rs " << price
                << ", Estimated Time: " << estimatedTime << " mins"
                << ", Timestamp: " << dt << endl;

        logFile.close();
    }

    // Update rider's assigned deliveries
    void updateRiderRoutes(const string& rider, const string& deliveryRoute) {
        fstream riderFile("auth/riders.txt", ios::in | ios::out);
        if (!riderFile) {
            cerr << "Error: Unable to open riders file." << endl;
            return;
        }

        string line;
        vector<string> lines;
        bool riderFound = false;

        while (getline(riderFile, line)) {
            istringstream iss(line);
            string username, password;
            iss >> username >> password;

            if (username == rider) {
                riderFound = true;
                line += " " + deliveryRoute;
            }

            lines.push_back(line);
        }

        riderFile.close();

        if (riderFound) {
            ofstream outFile("auth/riders.txt", ios::trunc);
            if (!outFile) {
                cerr << "Error: Unable to write to riders file." << endl;
                return;
        }

            for (const auto& l : lines) {
                outFile << l << endl;
            }
            outFile.close();
        } else {
            cerr << "Rider not found: " << rider << endl;
        }
    }

    // Update customer balance
    void updateCustomerBalance(const string& customer, int orderAmount) {
        fstream userFile("auth/users.txt", ios::in | ios::out);
        if (!userFile) {
            cerr << "Error: Unable to open users file." << endl;
            return;
        }

        string line;
        vector<string> lines;
        bool userFound = false;

        while (getline(userFile, line)) {
            istringstream iss(line);
            string username, password;
            double balance;

            iss >> username >> password >> balance;

            if (username == customer) {
                userFound = true;
                balance -= orderAmount;
                line = username + " " + password + " " + to_string(balance);
            }

            lines.push_back(line);
        }

        userFile.close();

        if (userFound) {
            ofstream outFile("auth/users.txt", ios::trunc);
            for (const auto& l : lines) {
                outFile << l << endl;
            }
            outFile.close();
        } else {
            cerr << "Customer not found: " << customer << endl;
        }
    }
};

#endif // DELIVERY_TRACKING_H
