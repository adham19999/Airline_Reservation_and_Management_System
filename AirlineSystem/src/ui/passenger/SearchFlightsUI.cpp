#include "SearchFlightsUI.h"
#include <iostream>
#include <iomanip>

using namespace std;

SearchFlightsUI::SearchFlightsUI(FlightService& fs, AircraftService& as)
    : flightService(fs), aircraftService(as) {}

void SearchFlightsUI::display() {
    cout << "\n--- Search Flights ---\n";

    string origin, destination, departureDate;

    cout << "Enter Origin: ";
    getline(cin, origin);

    cout << "Enter Destination: ";
    getline(cin, destination);

    cout << "Enter Departure Date (YYYY-MM-DD): ";
    getline(cin, departureDate);

    // Search for flights
    auto results = flightService.searchFlights(origin, destination, departureDate);
    displayFlightResults(results);
}

void SearchFlightsUI::displayFlightResults(const vector<shared_ptr<Flight>>& flights) const {
    if (flights.empty()) {
        cout << "\nNo available flights found.\n";
        return;
    }

    cout << "\nAvailable Flights:\n";

    for (size_t i = 0; i < flights.size(); ++i) {
        const auto& flight = flights[i];
        
        // Get aircraft details
        auto aircraft = aircraftService.getAircraftById(flight->getAircraftId());
        string aircraftModel = aircraft ? aircraft->getModel() : "Unknown";
        
        // Calculate available seats (total seats - for now showing total, 
        // will be updated when reservation system is implemented)
        int availableSeats = aircraft ? aircraft->getTotalSeats() : 0;
        
        // Placeholder price (will be from pricing system later)
        double price = 250.00 + (i * 50.00);

        cout << (i + 1) << ". Flight Number: " << flight->getFlightNumber() << "\n";
        cout << "   Departure: " << flight->getDepartureTime() << "\n";
        cout << "   Arrival: " << flight->getArrivalTime() << "\n";
        cout << "   Aircraft: " << aircraftModel << "\n";
        cout << "   Available Seats: " << availableSeats << "\n";
        cout << "   Price: $" << fixed << setprecision(2) << price << "\n";
    }

    cout << "\n";
}