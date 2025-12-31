#include "ManageCrewUI.h"
#include <iostream>
#include <iomanip>

using namespace std;

ManageCrewUI::ManageCrewUI(CrewService& cs, FlightService& fs)
    : crewService(cs), flightService(fs) {}

void ManageCrewUI::display() {
    int choice;
    do {
        cout << "\n========================================\n";
        cout << "--- Manage Crew ---\n";
        cout << "========================================\n";
        cout << "1. View All Crew Members\n";
        cout << "2. Add New Pilot\n";
        cout << "3. Add New Flight Attendant\n";
        cout << "4. Assign Crew to Flight\n";
        cout << "5. View Crew Assignments\n";
        cout << "6. View Flight Hours Report\n";
        cout << "7. Delete Crew Member\n";
        cout << "8. Back\n";
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1:
                displayAllCrew();
                break;
            case 2:
                addNewPilot();
                break;
            case 3:
                addNewFlightAttendant();
                break;
            case 4:
                assignCrewToFlight();
                break;
            case 5:
                viewCrewAssignments();
                break;
            case 6:
                viewFlightHoursReport();
                break;
            case 7:
                deleteCrewMember();
                break;
            case 8:
                return;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (true);
}

void ManageCrewUI::displayAllCrew() const {
    auto allCrew = crewService.getAllCrew();

    if (allCrew.empty()) {
        cout << "\nNo crew members found.\n";
        return;
    }

    cout << "\n========================================\n";
    cout << "--- Crew Roster ---\n";
    cout << "========================================\n";
    cout << left << setw(8) << "ID" << setw(20) << "Name"
         << setw(12) << "Type" << "Details\n";
    cout << "----------------------------------------\n";

    for (const auto& crew : allCrew) {
        cout << left << setw(8) << crew->getCrewId()
             << setw(20) << crew->getName()
             << setw(12) << crew->getCrewType();

        auto pilot = dynamic_pointer_cast<Pilot>(crew);
        if (pilot) {
            cout << "License: " << pilot->getLicenseNumber()
                 << " | Hours: " << fixed << setprecision(1) 
                 << pilot->getTotalFlightHours();
        }

        auto attendant = dynamic_pointer_cast<FlightAttendant>(crew);
        if (attendant) {
            cout << "Level: " << attendant->getCertificationLevel();
        }

        cout << "\n";
    }

    cout << "\nTotal Crew: " << allCrew.size() << "\n";
}

void ManageCrewUI::addNewPilot() {
    cout << "\n========================================\n";
    cout << "--- Add New Pilot ---\n";
    cout << "========================================\n";

    string name, licenseNumber;
    double initialHours;

    cout << "Full Name: ";
    getline(cin, name);

    cout << "License Number (e.g., ATP-12345): ";
    getline(cin, licenseNumber);

    cout << "Initial Flight Hours: ";
    cin >> initialHours;
    cin.ignore();

    if (initialHours < 0) {
        cout << "Error: Invalid flight hours.\n";
        return;
    }

    auto pilot = crewService.addPilot(name, licenseNumber, initialHours);

    if (pilot) {
        cout << "\n✓ Pilot added successfully!\n";
        cout << "  Pilot ID: " << pilot->getCrewId() << "\n";
    }
}

void ManageCrewUI::addNewFlightAttendant() {
    cout << "\n========================================\n";
    cout << "--- Add New Flight Attendant ---\n";
    cout << "========================================\n";

    string name, certificationLevel;

    cout << "Full Name: ";
    getline(cin, name);

    cout << "Certification Level (Junior/Senior/Lead): ";
    getline(cin, certificationLevel);

    auto attendant = crewService.addFlightAttendant(name, certificationLevel);

    if (attendant) {
        cout << "\n✓ Flight Attendant added successfully!\n";
        cout << "  Crew ID: " << attendant->getCrewId() << "\n";
    }
}

void ManageCrewUI::assignCrewToFlight() {
    cout << "\n========================================\n";
    cout << "--- Assign Crew to Flight ---\n";
    cout << "========================================\n";

    // Display available flights
    auto allFlights = flightService.getAllFlights();
    if (allFlights.empty()) {
        cout << "No flights available.\n";
        return;
    }

    cout << "\nAvailable Flights:\n";
    for (size_t i = 0; i < allFlights.size(); ++i) {
        cout << i + 1 << ". " << allFlights[i]->getFlightNumber()
             << " (" << allFlights[i]->getDepartureCity() << " → "
             << allFlights[i]->getArrivalCity() << ")\n";
    }

    int flightChoice;
    cout << "\nSelect Flight (1-" << allFlights.size() << "): ";
    cin >> flightChoice;
    cin.ignore();

    if (flightChoice < 1 || flightChoice > (int)allFlights.size()) {
        cout << "Invalid selection.\n";
        return;
    }

    auto selectedFlight = allFlights[flightChoice - 1];

    // Display available crew
    auto allCrew = crewService.getAllCrew();
    if (allCrew.empty()) {
        cout << "No crew members available.\n";
        return;
    }

    cout << "\nAvailable Crew:\n";
    for (size_t i = 0; i < allCrew.size(); ++i) {
        cout << i + 1 << ". " << allCrew[i]->getName()
             << " (" << allCrew[i]->getCrewType() << ")\n";
    }

    int crewChoice;
    cout << "\nSelect Crew Member (1-" << allCrew.size() << "): ";
    cin >> crewChoice;
    cin.ignore();

    if (crewChoice < 1 || crewChoice > (int)allCrew.size()) {
        cout << "Invalid selection.\n";
        return;
    }

    auto selectedCrew = allCrew[crewChoice - 1];

    // Get flight duration
    double duration;
    cout << "Flight Duration (hours): ";
    cin >> duration;
    cin.ignore();

    // Assign crew
    bool success = crewService.assignCrewToFlight(
        selectedCrew->getCrewId(),
        selectedFlight->getFlightId(),
        selectedFlight->getDepartureTime(),
        duration
    );

    if (success) {
        cout << "\n✓ Crew assigned successfully!\n";
    }
}

void ManageCrewUI::viewCrewAssignments() const {
    cout << "\n========================================\n";
    cout << "--- Crew Assignments ---\n";
    cout << "========================================\n";

    string crewId;
    cout << "Enter Crew ID (or 'all' for all assignments): ";
    getline(cin, crewId);

    if (crewId == "all") {
        auto allCrew = crewService.getAllCrew();
        for (const auto& crew : allCrew) {
            auto assignments = crewService.getCrewAssignments(crew->getCrewId());
            if (!assignments.empty()) {
                cout << "\n" << crew->getName() << " (" << crew->getCrewId() << "):\n";
                for (const auto& assignment : assignments) {
                    cout << "  - Flight: " << assignment.flightId
                         << " | Date: " << assignment.assignmentDate
                         << " | Duration: " << fixed << setprecision(1)
                         << assignment.flightDuration << "h\n";
                }
            }
        }
    } else {
        auto crew = crewService.getCrewById(crewId);
        if (!crew) {
            cout << "Crew member not found.\n";
            return;
        }

        auto assignments = crewService.getCrewAssignments(crewId);
        if (assignments.empty()) {
            cout << "No assignments found for this crew member.\n";
            return;
        }

        cout << "\nAssignments for " << crew->getName() << ":\n";
        for (const auto& assignment : assignments) {
            cout << "  - Flight: " << assignment.flightId
                 << " | Date: " << assignment.assignmentDate
                 << " | Duration: " << fixed << setprecision(1)
                 << assignment.flightDuration << "h\n";
        }
    }
}

void ManageCrewUI::viewFlightHoursReport() const {
    cout << "\n========================================\n";
    cout << "--- Flight Hours Report ---\n";
    cout << "========================================\n";

    auto allPilots = crewService.getAllPilots();

    if (allPilots.empty()) {
        cout << "No pilots in system.\n";
        return;
    }

    cout << left << setw(8) << "Pilot ID" << setw(20) << "Name"
         << setw(10) << "Daily" << setw(10) << "Weekly"
         << setw(10) << "Monthly" << setw(10) << "Yearly\n";
    cout << "----------------------------------------\n";

    for (const auto& pilot : allPilots) {
        string crewId = pilot->getCrewId();
        cout << left << setw(8) << crewId
             << setw(20) << pilot->getName()
             << setw(10) << fixed << setprecision(1) << crewService.getDailyFlightHours(crewId)
             << setw(10) << crewService.getWeeklyFlightHours(crewId)
             << setw(10) << crewService.getMonthlyFlightHours(crewId)
             << setw(10) << pilot->getTotalFlightHours() << "\n";
    }

    cout << "\nRegulatory Limits:\n";
    cout << "  Daily: 8.0h | Weekly: 30.0h | Monthly: 100.0h | Yearly: 1000.0h\n";
}

void ManageCrewUI::deleteCrewMember() {
    cout << "\n========================================\n";
    cout << "--- Delete Crew Member ---\n";
    cout << "========================================\n";

    string crewId;
    cout << "Enter Crew ID: ";
    getline(cin, crewId);

    auto crew = crewService.getCrewById(crewId);
    if (!crew) {
        cout << "Error: Crew member not found.\n";
        return;
    }

    cout << "\nCrew Member to delete:\n";
    cout << "  Name: " << crew->getName() << "\n";
    cout << "  Type: " << crew->getCrewType() << "\n";

    char confirm;
    cout << "\nAre you sure? (y/n): ";
    cin >> confirm;
    cin.ignore();

    if (confirm == 'y' || confirm == 'Y') {
        crewService.deleteCrewMember(crewId);
    } else {
        cout << "Delete cancelled.\n";
    }
}