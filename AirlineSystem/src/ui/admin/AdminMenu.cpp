#include "AdminMenu.h"
#include <iostream>
#include "ManageFlightsUI.h"
#include "ManageAircraftUI.h"
#include "ManageCrewUI.h"
#include "ManageUserUI.h"

using namespace std;

AdminMenu::AdminMenu(shared_ptr<User> user,
                     FlightService& fs,
                     AircraftService& as,
                     CrewService& cs,
                     UserRepository& ur)
    : Menu(user), flightService(fs), aircraftService(as), crewService(cs), userRepository(ur) {}

void AdminMenu::display() {
    cout << "\n========================================\n";
    cout << "--- Administrator Menu ---\n";
    cout << "========================================\n";
    cout << "1. Manage Flights\n";
    cout << "2. Manage Aircraft\n";
    cout << "3. Manage Crew\n";
    cout << "4. Manage Users\n";
    cout << "5. Generate Reports\n";
    cout << "6. Logout\n";
    cout << "Enter choice: ";
}

bool AdminMenu::handleInput(int choice) {
    switch (choice) {
        case 1: manageFlights(); break;
        case 2: manageAircraft(); break;
        case 3: manageCrew(); break;
        case 4: manageUsers(); break;
        case 5: generateReports(); break;
        case 6: running = false; return false;
        default: cout << "Invalid choice. Try again.\n"; break;
    }
    return running;
}

void AdminMenu::manageFlights() {
    ManageFlightsUI ui(flightService, aircraftService);
    ui.display();
}

void AdminMenu::manageAircraft() {
    ManageAircraftUI ui(aircraftService);
    ui.display();
}

void AdminMenu::manageCrew() {
    ManageCrewUI ui(crewService, flightService);
    ui.display();
}

void AdminMenu::manageUsers() {
    ManageUsersUI ui(userRepository);
    ui.display();
}

void AdminMenu::generateReports() {
    cout << "\n[TODO] Generate Reports - Coming in Phase 4\n";
}