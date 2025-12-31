#include "AgentMenu.h"
#include <iostream>

AgentMenu::AgentMenu(shared_ptr<User> user) : Menu(user) {}

void AgentMenu::display() {
    cout << "\n--- Booking Agent Menu ---\n";
    cout << "1. Search Flights\n";
    cout << "2. Book a Flight\n";
    cout << "3. Modify Reservation\n";
    cout << "4. Cancel Reservation\n";
    cout << "5. Logout\n";
    cout << "Enter choice: ";
}

bool AgentMenu::handleInput(int choice) {
    switch (choice) {
        case 1:
            searchFlights();
            break;
        case 2:
            bookFlight();
            break;
        case 3:
            modifyReservation();
            break;
        case 4:
            cancelReservation();
            break;
        case 5:
            running = false;
            return false;
        default:
            cout << "Invalid choice. Try again.\n";
    }
    return running;
}

void AgentMenu::searchFlights() {
    cout << "\n[TODO] Search Flights - Coming in Phase 3\n";
}

void AgentMenu::bookFlight() {
    cout << "\n[TODO] Book Flight - Coming in Phase 3\n";
}

void AgentMenu::modifyReservation() {
    cout << "\n[TODO] Modify Reservation - Coming in Phase 3\n";
}

void AgentMenu::cancelReservation() {
    cout << "\n[TODO] Cancel Reservation - Coming in Phase 3\n";
}