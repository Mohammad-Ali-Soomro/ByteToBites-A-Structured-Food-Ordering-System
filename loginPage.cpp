#include <iostream>
#include <string>
using namespace std;

// Helper function for input validation using basic C++ syntax
int getValidatedInput(int min, int max) {
    int choice;
    cout << "Enter your choice: ";
    cin >> choice;

    while ( choice < min || choice > max) {
       
        cout << "Invalid choice. Please enter a number between " << min << " and " << max << ": ";
        cin >> choice;
    }
    return choice;
}

// Dummy functions
void loginUser() {
    cout << "\n--- User Login ---\n";
    cout << "Feature coming soon!\n";
}

void registeration() {
    cout << "\n--- Registration ---\n";
    cout << "Feature coming soon!\n";
}

void loginRider() {
    cout << "\n--- Rider Login ---\n";
    cout << "Feature coming soon!\n";
}

void loginAdmin() {
    cout << "\n--- Admin Login ---\n";
    cout << "Feature coming soon!\n";
}

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
        cout << "\n--- User Menu ---\n";
        cout << "1. Login (Existing Account)\n";
        cout << "2. Register (New Account)\n";
        choice = getValidatedInput(1, 2);

        switch (choice) {
            case 1:
                loginUser();
                break;
            case 2:
                registeration();
                break;
        }
    } else if (choice == 2) {
        cout << "\n--- Rider Menu ---\n";
        cout << "1. Login (Existing Account)\n";
        cout << "2. Register (New Account)\n";
        choice = getValidatedInput(1, 2);

        switch (choice) {
            case 1:
                loginRider();
                break;
            case 2:
                registeration();
                break;
        }
    } else if (choice == 3) {
        cout << "\n--- Admin Login ---\n";
        cout << "Please enter your company-provided credentials:\n";
        loginAdmin();
    }
}

int main() {
    loginPage();
    return 0;
}
