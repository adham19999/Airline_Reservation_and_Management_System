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
        cout << "\nNo flights found matching your criteria.\n";
        return;
    }

    cout << "\n========================================\n";
    cout << "Search Results:\n";
    cout << "========================================\n";
    cout << left << setw(12) << "Flight#" << setw(15) << "Route" 
         << setw(20) << "Departure" << setw(20) << "Arrival" 
         << setw(15) << "Aircraft" << "\n";
    cout << "----------------------------------------\n";

    for (const auto& flight : flights) {
        // ✅ NOW WORKS: getAircraft() returns shared_ptr<Aircraft>
        auto aircraft = flight->getAircraft();
        string aircraftModel = aircraft ? aircraft->getModel() : "N/A";

        cout << left << setw(12) << flight->getFlightNumber()
             << setw(15) << (flight->getDepartureCity() + "-" + flight->getArrivalCity())
             << setw(20) << flight->getDepartureTime()
             << setw(20) << flight->getArrivalTime()
             << setw(15) << aircraftModel << "\n";
    }

    cout << "\n";
}