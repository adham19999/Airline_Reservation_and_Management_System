#include "PassengerMenu.h"
#include "SearchFlightsUI.h"
#include <iostream>

using namespace std;

PassengerMenu::PassengerMenu(shared_ptr<User> user, FlightService& fs, AircraftService& as)
    : Menu(user), flightService(fs), aircraftService(as) {}

void PassengerMenu::display() {
    cout << "\n--- Passenger Menu ---\n";
    cout << "1. Search Flights\n";
    cout << "2. View My Reservations\n";
    cout << "3. Check-In\n";
    cout << "4. Logout\n";
    cout << "Enter choice: ";
}

bool PassengerMenu::handleInput(int choice) {
    switch (choice) {
        case 1:
            searchFlights();
            break;
        case 2:
            viewReservations();
            break;
        case 3:
            checkIn();
            break;
        case 4:
            running = false;
            return false;
        default:
            cout << "Invalid choice. Try again.\n";
            break;
    }
    return running;
}

void PassengerMenu::searchFlights() {
    SearchFlightsUI searchUI(flightService, aircraftService);
    searchUI.display();
}

void PassengerMenu::viewReservations() {
    cout << "\n[TODO] View My Reservations - Coming in Phase 3\n";
}

void PassengerMenu::checkIn() {
    cout << "\n[TODO] Check-In - Coming in Phase 4\n";
}