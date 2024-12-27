#pragma once

#include <iostream>
#include <string>
#include <vector>
using namespace std;

// Hash Table size for customer ID lookup
const int HASH_SIZE = 100;

// Hash Function
int hashFunction(int customerID) {
    return customerID % HASH_SIZE;
}

// Customer Node for BST
struct CustomerNode {
    int customerID;
    string name;
    string contact;
    CustomerNode* left;
    CustomerNode* right;

    CustomerNode(int id, string n, string c) : customerID(id), name(n), contact(c), left(nullptr), right(nullptr) {}
};

// Customer Management Class
class CustomerManagement {
private:
    CustomerNode* root; // Root of the BST
    vector<bool> hashTable; // Hash table for quick ID existence check

    // Insert Helper for BST
    CustomerNode* insert(CustomerNode* node, int id, string name, string contact) {
        if (node == nullptr) {
            return new CustomerNode(id, name, contact);
        }
        if (id < node->customerID) {
            node->left = insert(node->left, id, name, contact);
        } else if (id > node->customerID) {
            node->right = insert(node->right, id, name, contact);
        }
        return node;
    }

    // Search Helper for BST
    CustomerNode* search(CustomerNode* node, int id) {
        if (node == nullptr || node->customerID == id) {
            return node;
        }
        if (id < node->customerID) {
            return search(node->left, id);
        } else {
            return search(node->right, id);
        }
    }

    // In-order Traversal for Display
    void inorder(CustomerNode* node) {
        if (node != nullptr) {
            inorder(node->left);
            cout << "ID: " << node->customerID << " | Name: " << node->name << " | Contact: " << node->contact << endl;
            inorder(node->right);
        }
    }

public:
    CustomerManagement() : root(nullptr), hashTable(HASH_SIZE, false) {}

    // Add Customer
    void addCustomer(int id, string name, string contact) {
        if (hashTable[hashFunction(id)]) {
            cout << "Customer ID already exists. Try again with a unique ID.\n";
            return;
        }
        root = insert(root, id, name, contact);
        hashTable[hashFunction(id)] = true;
        cout << "Customer added successfully!\n";
    }

    // Search Customer
    void searchCustomer(int id) {
        CustomerNode* result = search(root, id);
        if (result != nullptr) {
            cout << "Customer Found:\n";
            cout << "ID: " << result->customerID << " | Name: " << result->name << " | Contact: " << result->contact << endl;
        } else {
            cout << "Customer not found.\n";
        }
    }

    // Display All Customers
    void displayAllCustomers() {
        if (root == nullptr) {
            cout << "No customers to display.\n";
            return;
        }
        cout << "Customer List:\n";
        inorder(root);
    }
};

