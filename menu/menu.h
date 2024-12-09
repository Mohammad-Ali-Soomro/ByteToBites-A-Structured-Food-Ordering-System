#ifndef MENU_H
#define MENU_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

class MenuItem {
private:
    string name;
    int price;
    int estimatedTime; // in minutes
    char location;     // e.g., 'A', 'B', 'C'
    string type;       // Category/type of the item

public:
    // Constructor
    MenuItem(const string& name, double price, int estimatedTime, char location, const string& type)
        : name(name), price(price), estimatedTime(estimatedTime), location(location), type(type) {}

    // Display menu item details
    void display() const {
        cout << "Name: " << name << ", Price: Rs " << price
             << ", Estimated Time: " << estimatedTime << " mins"
             << ", Location: " << location
             << ", Type: " << type << endl;
    }

    // Getter methods for filtering and other operations
    string getName() const { return name; }
    double getPrice() const { return price; }
    int getEstimatedTime() const { return estimatedTime; }
    char getLocation() const { return location; }
    string getType() const { return type; }
};

class Menu {
private:
    vector<MenuItem> items; // Vector to store menu items

public:
    // Load menu from file
    bool loadMenu(const string& filename) {
        ifstream file(filename);
        if (!file) {
            cerr << "Error: Could not open " << filename << endl;
            return false;
        }

        string line;
        while (getline(file, line)) {
            istringstream iss(line);
            string name, type;
            int price;
            int estimatedTime;
            char location;

            // Assuming the file format: name,price,estimatedTime,location,type
           if (getline(iss, name, ',') && iss >> price && iss.ignore() // Skip the comma
    && iss >> estimatedTime && iss.ignore() // Skip the comma
    && iss >> location && iss.ignore() // Skip the comma
    && getline(iss, type)) {
    // Clean up name and type
    name.erase(0, name.find_first_not_of(" \t"));
    name.erase(name.find_last_not_of(" \t") + 1);
    type.erase(0, type.find_first_not_of(" \t"));
    type.erase(type.find_last_not_of(" \t") + 1);

    // Add to items vector
    items.emplace_back(name, price, estimatedTime, location, type);
} else {
    cerr << "Error: Malformed line in file: " << line << endl;
}

        }

        file.close();
        return true;
    }

    // Display all menu items
    void displayMenu() const {
        cout << "\n--- Menu ---\n";
        int i=0;
        for (const auto& item : items) {
            i++;
            cout<<i<<") ";
            item.display();
            cout<<endl;
        }
    }

    // Display menu items filtered by type
    vector<MenuItem*> displayMenuByType(const string& filterType) const {
        vector<MenuItem*> filteredItems; // Temporary list for filtered items

        cout << "\n--- Menu Items of Type: " << filterType << " ---\n";
        for (const auto& item : items) {
            if (item.getType() == filterType) {
                filteredItems.push_back(const_cast<MenuItem*>(&item)); // Store pointer to the item
            }
        }

        if (filteredItems.empty()) {
            cout << "No items found for the type: " << filterType << endl;
        } else {
            for (size_t i = 0; i < filteredItems.size(); ++i) {
                cout << i + 1 << ". ";
                filteredItems[i]->display();
            }
        }

        return filteredItems; // Return the filtered list
    }


    // Get all menu items
     vector<MenuItem>& getItems()  { return items; }
};


#endif // MENU_H
