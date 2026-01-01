// ...existing includes...
#include "ApplicationUI.h"
#include "LoginUI.h"
#include "admin/AdminMenu.h"
#include "agent/AgentMenu.h"
#include "passenger/PassengerMenu.h"
#include <iostream>
#include <memory>

using namespace std;

ApplicationUI::ApplicationUI(UserRepository& repo,
                             AuthService& authSvc,
                             FlightService& flightSvc,
                             AircraftService& aircraftSvc,
                             CrewService& crewSvc)
    : userRepository(repo), authService(authSvc),
      flightService(flightSvc), aircraftService(aircraftSvc), crewService(crewSvc) {}

void ApplicationUI::displayRoleSelectionMenu() const {
    cout << "\n========================================\n";
    cout << "Welcome to Airline Reservation System\n";
    cout << "========================================\n";
    cout << "Please select your role:\n";
    cout << "1. Administrator\n";
    cout << "2. Booking Agent\n";
    cout << "3. Passenger\n";
    cout << "4. Exit\n";
}

int ApplicationUI::getRoleChoice() {
    cout << "Enter choice: ";
    int choice;
    cin >> choice;
    cin.ignore();
    return choice;
}

unique_ptr<Menu> ApplicationUI::createMenuForRole(shared_ptr<User> user) {
    UserRole role = user->getRole();

    if (role == UserRole::Administrator) {
        return make_unique<AdminMenu>(user, flightService, aircraftService, crewService, userRepository);  // Added userRepository
    } else if (role == UserRole::BookingAgent) {
        return make_unique<AgentMenu>(user);
    } else if (role == UserRole::Passenger) {
        return make_unique<PassengerMenu>(user, flightService, aircraftService);
    }

    return nullptr;
}


void ApplicationUI::run() {
    bool systemRunning = true;

    while (systemRunning) {
        displayRoleSelectionMenu();
        int roleChoice = getRoleChoice();

        if (roleChoice == 4) {
            cout << "Goodbye!\n";
            systemRunning = false;
            break;
        }

        if (roleChoice < 1 || roleChoice > 3) {
            cout << "Error: Invalid role choice.\n";
            continue;
        }

        LoginUI loginUI(authService);
        shared_ptr<User> user = loginUI.performLogin();

        if (!user) {
            cout << "Login failed. Please try again.\n";
            continue;
        }

        UserRole actualRole = user->getRole();
        bool validRole = (roleChoice == 1 && actualRole == UserRole::Administrator) ||
                         (roleChoice == 2 && actualRole == UserRole::BookingAgent) ||
                         (roleChoice == 3 && actualRole == UserRole::Passenger);

        if (!validRole) {
            cout << "Error: Your role does not match the selected role.\n";
            continue;
        }

        unique_ptr<Menu> menu = createMenuForRole(user);

        if (menu) {
            bool menuRunning = true;
            while (menuRunning) {
                menu->display();
                int choice;
                cin >> choice;
                cin.ignore();

                menuRunning = menu->handleInput(choice);
            }
        }

        authService.logout();
    }
}