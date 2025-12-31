#include "ManageFlightsUI.h"
#include <iostream>
#include <iomanip>

using namespace std;

ManageFlightsUI::ManageFlightsUI(FlightService& fs, AircraftService& as)
    : flightService(fs), aircraftService(as) {}

void ManageFlightsUI::display() {
    int choice;
    do {
        cout << "\n========================================\n";
        cout << "--- Manage Flights ---\n";
        cout << "========================================\n";
        cout << "1. View All Flights\n";
        cout << "2. Add New Flight\n";
        cout << "3. Update Flight Status\n";
        cout << "4. Delete Flight\n";
        cout << "5. Back\n";
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore();
        
        switch (choice) {
            case 1:
                displayFlights();
                break;
            case 2:
                addFlight();
                break;
            case 3:
                updateFlightStatus();
                break;
            case 4:
                deleteFlight();
                break;
            case 5:
                return;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (true);
}

void ManageFlightsUI::displayFlights() const {
    auto flights = flightService.getAllFlights();
    
    if (flights.empty()) {
        cout << "\nNo flights found.\n";
        return;
    }
    
    cout << "\n========================================\n";
    cout << "--- All Flights ---\n";
    cout << "========================================\n";
    cout << left << setw(12) << "Flight #" << setw(15) << "Departure" 
         << setw(15) << "Arrival" << setw(12) << "Dep Time" << "Status\n";
    cout << "----------------------------------------\n";
    
    for (const auto& flight : flights) {
        string status;
        switch (flight->getStatus()) {
            case FlightStatus::Scheduled: status = "Scheduled"; break;
            case FlightStatus::InFlight: status = "In Flight"; break;
            case FlightStatus::Completed: status = "Completed"; break;
            case FlightStatus::Cancelled: status = "Cancelled"; break;
            case FlightStatus::Delayed: status = "Delayed"; break;
        }
        
        cout << left << setw(12) << flight->getFlightNumber()
             << setw(15) << flight->getDepartureCity()
             << setw(15) << flight->getArrivalCity()
             << setw(12) << flight->getDepartureTime()
             << status << "\n";
    }
}

void ManageFlightsUI::addFlight() {
    cout << "\n--- Add New Flight ---\n";
    
    // Display available aircraft
    auto allAircraft = aircraftService.getAllAircraft();
    if (allAircraft.empty()) {
        cout << "Error: No aircraft available. Add aircraft first.\n";
        return;
    }
    
    cout << "\nAvailable Aircraft:\n";
    for (size_t i = 0; i < allAircraft.size(); ++i) {
        cout << i + 1 << ". " << allAircraft[i]->getModel() 
             << " (" << allAircraft[i]->getAircraftId() << ")\n";
    }
    
    string flightNumber, departure, arrival, depTime, arrTime;
    int aircraftChoice;
    
    cout << "\nFlight Number: ";
    getline(cin, flightNumber);
    
    cout << "Departure City: ";
    getline(cin, departure);
    
    cout << "Arrival City: ";
    getline(cin, arrival);
    
    cout << "Departure Time (YYYY-MM-DD HH:MM): ";
    getline(cin, depTime);
    
    cout << "Arrival Time (YYYY-MM-DD HH:MM): ";
    getline(cin, arrTime);
    
    cout << "Select Aircraft (1-" << allAircraft.size() << "): ";
    cin >> aircraftChoice;
    cin.ignore();
    
    if (aircraftChoice < 1 || aircraftChoice > (int)allAircraft.size()) {
        cout << "Invalid aircraft selection.\n";
        return;
    }
    
    string aircraftId = allAircraft[aircraftChoice - 1]->getAircraftId();
    
    auto flight = flightService.createFlight(flightNumber, departure, arrival, 
                                            depTime, arrTime, aircraftId);
    
    if (flight) {
        cout << "Flight added successfully!\n";
    } else {
        cout << "Failed to add flight.\n";
    }
}

void ManageFlightsUI::updateFlightStatus() {
    cout << "\n--- Update Flight Status ---\n";
    
    string flightNumber;
    cout << "Enter Flight Number: ";
    getline(cin, flightNumber);
    
    auto flight = flightService.getFlightByNumber(flightNumber);
    if (!flight) {
        cout << "Flight not found.\n";
        return;
    }
    
    cout << "\nCurrent Status: ";
    switch (flight->getStatus()) {
        case FlightStatus::Scheduled: cout << "Scheduled\n"; break;
        case FlightStatus::InFlight: cout << "In Flight\n"; break;
        case FlightStatus::Completed: cout << "Completed\n"; break;
        case FlightStatus::Cancelled: cout << "Cancelled\n"; break;
        case FlightStatus::Delayed: cout << "Delayed\n"; break;
    }
    
    cout << "\nNew Status:\n";
    cout << "1. Scheduled\n2. In Flight\n3. Completed\n4. Cancelled\n5. Delayed\n";
    cout << "Enter choice: ";
    int choice;
    cin >> choice;
    cin.ignore();
    
    FlightStatus newStatus = FlightStatus::Scheduled;
    switch (choice) {
        case 1: newStatus = FlightStatus::Scheduled; break;
        case 2: newStatus = FlightStatus::InFlight; break;
        case 3: newStatus = FlightStatus::Completed; break;
        case 4: newStatus = FlightStatus::Cancelled; break;
        case 5: newStatus = FlightStatus::Delayed; break;
        default:
            cout << "Invalid choice.\n";
            return;
    }
    
    flightService.updateFlightStatus(flight->getFlightId(), newStatus);
    cout << "Flight status updated successfully.\n";
}

void ManageFlightsUI::deleteFlight() {
    cout << "\n--- Delete Flight ---\n";
    
    string flightNumber;
    cout << "Enter Flight Number: ";
    getline(cin, flightNumber);
    
    auto flight = flightService.getFlightByNumber(flightNumber);
    if (!flight) {
        cout << "Flight not found.\n";
        return;
    }
    
    char confirm;
    cout << "Are you sure you want to delete flight " << flightNumber << "? (y/n): ";
    cin >> confirm;
    cin.ignore();
    
    if (confirm == 'y' || confirm == 'Y') {
        flightService.deleteFlight(flight->getFlightId());
        cout << "Flight deleted successfully.\n";
    } else {
        cout << "Delete cancelled.\n";
    }
}