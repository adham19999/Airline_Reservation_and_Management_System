#include "PassengerMenu.h"
#include "SearchFlightsUI.h"
#include "BookFlightUI.h"
#include <iostream>
#include <memory>

using namespace std;

PassengerMenu::PassengerMenu(shared_ptr<User> user, 
                             FlightService& fs, 
                             AircraftService& as,
                             BookingService& bs,
                             SeatService& ss)
    : Menu(user), 
      flightService(fs), 
      aircraftService(as),
      bookingService(bs),
      seatService(ss) {}

void PassengerMenu::display() {
    cout << "\n========================================\n";
    cout << "--- Passenger Menu ---\n";
    cout << "========================================\n";
    cout << "1. Search Flights\n";
    cout << "2. Book a Flight\n";
    cout << "3. View My Reservations\n";
    cout << "4. Check-In\n";
    cout << "5. Logout\n";
    cout << "Enter choice: ";
}

bool PassengerMenu::handleInput(int choice) {
    switch (choice) {
        case 1:
            searchFlights();
            break;
        case 2:
            bookFlight();
            break;
        case 3:
            viewReservations();
            break;
        case 4:
            checkIn();
            break;
        case 5:
            running = false;
            return false;
        default:
            cout << "Invalid choice. Try again.\n";
    }
    return running;
}

void PassengerMenu::searchFlights() {
    SearchFlightsUI searchUI(flightService, aircraftService);
    searchUI.display();
}

void PassengerMenu::bookFlight() {
    auto passenger = dynamic_pointer_cast<Passenger>(currentUser);
    if (!passenger) {
        cout << "Error: Invalid user type.\n";
        return;
    }

    BookFlightUI bookUI(bookingService, flightService, seatService, passenger);
    bookUI.display();
}

void PassengerMenu::viewReservations() {
    cout << "\n[TODO] View My Reservations - Coming in Phase 3\n";
}

void PassengerMenu::checkIn() {
    cout << "\n[TODO] Check-In - Coming in Phase 4\n";
}