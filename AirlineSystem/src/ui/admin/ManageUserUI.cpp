#include "ManageUserUI.h"
#include "../../domain/user/Administrator.h"
#include "../../domain/user/BookingAgent.h"
#include "../../domain/user/Passenger.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

using namespace std;

ManageUsersUI::ManageUsersUI(UserRepository& repo)
    : userRepository(repo) {}

void ManageUsersUI::display() {
    bool managing = true;

    while (managing) {
        cout << "\n--- Manage Users ---\n";
        cout << "1. View All Users\n";
        cout << "2. Search User\n";
        cout << "3. Add New User\n";
        cout << "4. Update User Status\n";
        cout << "5. Delete User\n";
        cout << "6. Back to Admin Menu\n";
        cout << "Enter choice: ";

        int choice;
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1:
                displayAllUsers();
                break;
            case 2:
                searchUser();
                break;
            case 3:
                addNewUser();
                break;
            case 4:
                updateUserStatus();
                break;
            case 5:
                deleteUser();
                break;
            case 6:
                managing = false;
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
                break;
        }
    }
}

void ManageUsersUI::displayAllUsers() const {
    const auto& allUsers = userRepository.getAllUsers();

    if (allUsers.empty()) {
        cout << "\nNo users found in the system.\n";
        return;
    }

    cout << "\n========================================\n";
    cout << "All Users in System\n";
    cout << "========================================\n";

    for (size_t i = 0; i < allUsers.size(); ++i) {
        const auto& user = allUsers[i];
        cout << "\n" << (i + 1) << ". User ID: " << user->getUserId() << "\n";
        displayUserDetails(user);
    }

    cout << "\n========================================\n";
}

void ManageUsersUI::displayUserDetails(shared_ptr<User> user) const {
    cout << "   Username: " << user->getUsername() << "\n";
    cout << "   Full Name: " << user->getFullName() << "\n";
    cout << "   Role: " << getRoleString(user->getRole()) << "\n";
    cout << "   Status: " << (user->isActive() ? "Active" : "Inactive") << "\n";

    // Role-specific details
    if (user->getRole() == UserRole::Administrator) {
        auto admin = dynamic_pointer_cast<Administrator>(user);
        if (admin) {
            cout << "   Admin Level: " << admin->getAdminLevel() << "\n";
        }
    }
    else if (user->getRole() == UserRole::BookingAgent) {
        auto agent = dynamic_pointer_cast<BookingAgent>(user);
        if (agent) {
            cout << "   Agent ID: " << agent->getAgentId() << "\n";
            cout << "   Office Location: " << agent->getOfficeLocation() << "\n";
        }
    }
    else if (user->getRole() == UserRole::Passenger) {
        auto passenger = dynamic_pointer_cast<Passenger>(user);
        if (passenger) {
            cout << "   Passport Number: " << passenger->getPassportNumber() << "\n";
        }
    }
}

void ManageUsersUI::searchUser() {
    cout << "\n--- Search User ---\n";
    cout << "Search by:\n";
    cout << "1. Username\n";
    cout << "2. User ID\n";
    cout << "3. Full Name\n";
    cout << "Enter choice: ";

    int searchChoice;
    cin >> searchChoice;
    cin.ignore();

    shared_ptr<User> foundUser = nullptr;

    switch (searchChoice) {
        case 1: {
            cout << "Enter username: ";
            string username;
            getline(cin, username);
            foundUser = userRepository.findByUsername(username);
            break;
        }
        case 2: {
            cout << "Enter user ID: ";
            string userId;
            getline(cin, userId);
            foundUser = userRepository.findByUserId(userId);
            break;
        }
        case 3: {
            cout << "Enter full name: ";
            string fullName;
            getline(cin, fullName);

            const auto& allUsers = userRepository.getAllUsers();
            for (const auto& user : allUsers) {
                if (user->getFullName().find(fullName) != string::npos) {
                    foundUser = user;
                    break;
                }
            }
            break;
        }
        default:
            cout << "Invalid search option.\n";
            return;
    }

    if (foundUser) {
        cout << "\n========================================\n";
        cout << "User Found\n";
        cout << "========================================\n";
        displayUserDetails(foundUser);
        cout << "========================================\n";
    } else {
        cout << "\nNo user found matching your search criteria.\n";
    }
}

void ManageUsersUI::addNewUser() {
    cout << "\n--- Add New User ---\n";

    // Generate new user ID
    const auto& allUsers = userRepository.getAllUsers();
    string newUserId = "U" + to_string(allUsers.size() + 1);

    cout << "New User ID: " << newUserId << "\n";

    cout << "Enter username: ";
    string username;
    getline(cin, username);

    // Check if username already exists
    if (userRepository.findByUsername(username)) {
        cout << "Error: Username already exists.\n";
        return;
    }

    cout << "Enter password: ";
    string password;
    getline(cin, password);

    cout << "Enter full name: ";
    string fullName;
    getline(cin, fullName);

    cout << "\nSelect user role:\n";
    cout << "1. Administrator\n";
    cout << "2. Booking Agent\n";
    cout << "3. Passenger\n";
    cout << "Enter choice: ";

    int roleChoice;
    cin >> roleChoice;
    cin.ignore();

    shared_ptr<User> newUser;

    switch (roleChoice) {
        case 1: {
            cout << "Enter admin level (1-5): ";
            int adminLevel;
            cin >> adminLevel;
            cin.ignore();
            newUser = make_shared<Administrator>(newUserId, username, password, fullName, adminLevel);
            break;
        }
        case 2: {
            cout << "Enter agent ID: ";
            string agentId;
            getline(cin, agentId);

            cout << "Enter office location: ";
            string officeLocation;
            getline(cin, officeLocation);

            newUser = make_shared<BookingAgent>(newUserId, username, password, fullName, agentId, officeLocation);
            break;
        }
        case 3: {
            cout << "Enter passport number: ";
            string passportNumber;
            getline(cin, passportNumber);

            cout << "Enter email: ";
            string email;
            getline(cin, email);

            cout << "Enter loyalty ID: ";
            string loyaltyId;
            getline(cin, loyaltyId);

            newUser = make_shared<Passenger>(newUserId, username, password, fullName, passportNumber, email, loyaltyId);
            break;
        }
        default:
            cout << "Invalid role choice.\n";
            return;
    }

    if (newUser) {
        userRepository.addUser(newUser);
        cout << "\nUser added successfully!\n";
        cout << "User ID: " << newUserId << "\n";
    }
}

void ManageUsersUI::updateUserStatus() {
    cout << "\n--- Update User Status ---\n";
    cout << "Enter user ID: ";
    string userId;
    getline(cin, userId);

    auto user = userRepository.findByUserId(userId);
    if (!user) {
        cout << "User not found.\n";
        return;
    }

    cout << "Current status: " << (user->isActive() ? "Active" : "Inactive") << "\n";
    cout << "New status:\n";
    cout << "1. Active\n";
    cout << "2. Inactive\n";
    cout << "Enter choice: ";

    int statusChoice;
    cin >> statusChoice;
    cin.ignore();

    bool newStatus;
    switch (statusChoice) {
        case 1:
            newStatus = true;
            break;
        case 2:
            newStatus = false;
            break;
        default:
            cout << "Invalid choice.\n";
            return;
    }

    user->setActive(newStatus);
    userRepository.updateUser(user);
    cout << "User status updated successfully!\n";
}

void ManageUsersUI::deleteUser() {
    cout << "\n--- Delete User ---\n";
    cout << "Enter user ID to delete: ";
    string userId;
    getline(cin, userId);

    auto user = userRepository.findByUserId(userId);
    if (!user) {
        cout << "User not found.\n";
        return;
    }

    cout << "Are you sure you want to delete this user?\n";
    cout << "Username: " << user->getUsername() << "\n";
    cout << "Full Name: " << user->getFullName() << "\n";
    cout << "1. Yes, delete\n";
    cout << "2. No, cancel\n";
    cout << "Enter choice: ";

    int confirmChoice;
    cin >> confirmChoice;
    cin.ignore();

    if (confirmChoice == 1) {
        userRepository.deleteUser(userId);
        cout << "User deleted successfully!\n";
    } else {
        cout << "Deletion cancelled.\n";
    }
}

UserRole ManageUsersUI::getUserRoleChoice() const {
    cout << "Select role:\n";
    cout << "1. Administrator\n";
    cout << "2. Booking Agent\n";
    cout << "3. Passenger\n";
    cout << "Enter choice: ";

    int choice;
    cin >> choice;
    cin.ignore();

    switch (choice) {
        case 1:
            return UserRole::Administrator;
        case 2:
            return UserRole::BookingAgent;
        case 3:
            return UserRole::Passenger;
        default:
            return UserRole::Passenger;
    }
}

string ManageUsersUI::getRoleString(UserRole role) const {
    switch (role) {
        case UserRole::Administrator:
            return "Administrator";
        case UserRole::BookingAgent:
            return "Booking Agent";
        case UserRole::Passenger:
            return "Passenger";
        default:
            return "Unknown";
    }
}