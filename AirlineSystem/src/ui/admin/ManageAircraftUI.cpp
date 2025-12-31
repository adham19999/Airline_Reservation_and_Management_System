#include "ManageAircraftUI.h"
#include <iostream>
#include <iomanip>

using namespace std;

ManageAircraftUI::ManageAircraftUI(AircraftService& as)
    : aircraftService(as) {}

void ManageAircraftUI::display() {
    int choice;
    do {
        cout << "\n========================================\n";
        cout << "--- Manage Aircraft ---\n";
        cout << "========================================\n";
        cout << "1. View All Aircraft\n";
        cout << "2. Add New Aircraft\n";
        cout << "3. Update Aircraft Details\n";
        cout << "4. Delete Aircraft\n";
        cout << "5. View Maintenance Schedule\n";
        cout << "6. Schedule Maintenance\n";
        cout << "7. Back\n";
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1:
                displayAllAircraft();
                break;
            case 2:
                addNewAircraft();
                break;
            case 3:
                updateAircraftDetails();
                break;
            case 4:
                deleteAircraftRecord();
                break;
            case 5:
                viewMaintenanceSchedule();
                break;
            case 6:
                scheduleMaintenanceCheck();
                break;
            case 7:
                return;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (true);
}

void ManageAircraftUI::displayAllAircraft() const {
    auto aircraft = aircraftService.getAllAircraft();

    if (aircraft.empty()) {
        cout << "\nNo aircraft found in the system.\n";
        return;
    }

    cout << "\n========================================\n";
    cout << "--- Aircraft Fleet ---\n";
    cout << "========================================\n";
    cout << left << setw(6) << "ID" << setw(12) << "Registration"
         << setw(15) << "Model" << setw(12) << "Economy" << "Business\n";
    cout << "----------------------------------------\n";

    for (const auto& ac : aircraft) {
        cout << left << setw(6) << ac->getAircraftId()
             << setw(12) << ac->getRegistration()
             << setw(15) << ac->getModel()
             << setw(12) << ac->getEconomySeats()
             << ac->getBusinessSeats() << "\n";
    }

    cout << "\nTotal Aircraft: " << aircraft.size() << "\n";
}

void ManageAircraftUI::addNewAircraft() {
    cout << "\n========================================\n";
    cout << "--- Add New Aircraft ---\n";
    cout << "========================================\n";

    string registration, model, manufacturer;
    int economySeats, businessSeats;

    cout << "Aircraft Registration (e.g., N12345): ";
    getline(cin, registration);

    cout << "Aircraft Model (e.g., Boeing 737): ";
    getline(cin, model);

    cout << "Manufacturer (e.g., Boeing): ";
    getline(cin, manufacturer);

    cout << "Economy Seats: ";
    cin >> economySeats;

    cout << "Business Seats: ";
    cin >> businessSeats;
    cin.ignore();

    if (economySeats <= 0 || businessSeats < 0) {
        cout << "Error: Invalid seat configuration.\n";
        return;
    }

    auto newAircraft = aircraftService.addAircraft(registration, model, 
                                                   manufacturer, economySeats, 
                                                   businessSeats);

    if (newAircraft) {
        cout << "\n✓ Aircraft added successfully!\n";
        cout << "  Aircraft ID: " << newAircraft->getAircraftId() << "\n";
        cout << "  Total Seats: " << newAircraft->getTotalSeats() << "\n";
    } else {
        cout << "Failed to add aircraft.\n";
    }
}

void ManageAircraftUI::updateAircraftDetails() {
    cout << "\n========================================\n";
    cout << "--- Update Aircraft Details ---\n";
    cout << "========================================\n";

    string aircraftId;
    cout << "Enter Aircraft ID: ";
    getline(cin, aircraftId);

    auto aircraft = aircraftService.getAircraftById(aircraftId);
    if (!aircraft) {
        cout << "Error: Aircraft not found.\n";
        return;
    }

    cout << "\nCurrent Details:\n";
    cout << "  Model: " << aircraft->getModel() << "\n";
    cout << "  Manufacturer: " << aircraft->getManufacturer() << "\n";
    cout << "  Economy Seats: " << aircraft->getEconomySeats() << "\n";
    cout << "  Business Seats: " << aircraft->getBusinessSeats() << "\n";

    string model, manufacturer;
    int economySeats, businessSeats;

    cout << "\nNew Aircraft Model (press Enter to keep current): ";
    getline(cin, model);
    if (model.empty()) model = aircraft->getModel();

    cout << "New Manufacturer (press Enter to keep current): ";
    getline(cin, manufacturer);
    if (manufacturer.empty()) manufacturer = aircraft->getManufacturer();

    cout << "New Economy Seats (enter -1 to keep current): ";
    cin >> economySeats;
    if (economySeats == -1) economySeats = aircraft->getEconomySeats();

    cout << "New Business Seats (enter -1 to keep current): ";
    cin >> businessSeats;
    if (businessSeats == -1) businessSeats = aircraft->getBusinessSeats();
    cin.ignore();

    aircraftService.updateAircraftDetails(aircraftId, model, manufacturer,
                                         economySeats, businessSeats);
}

void ManageAircraftUI::deleteAircraftRecord() {
    cout << "\n========================================\n";
    cout << "--- Delete Aircraft ---\n";
    cout << "========================================\n";

    string aircraftId;
    cout << "Enter Aircraft ID: ";
    getline(cin, aircraftId);

    auto aircraft = aircraftService.getAircraftById(aircraftId);
    if (!aircraft) {
        cout << "Error: Aircraft not found.\n";
        return;
    }

    cout << "\nAircraft to delete:\n";
    cout << "  Registration: " << aircraft->getRegistration() << "\n";
    cout << "  Model: " << aircraft->getModel() << "\n";

    char confirm;
    cout << "\nAre you sure? (y/n): ";
    cin >> confirm;
    cin.ignore();

    if (confirm == 'y' || confirm == 'Y') {
        aircraftService.deleteAircraft(aircraftId);
    } else {
        cout << "Delete cancelled.\n";
    }
}

void ManageAircraftUI::viewMaintenanceSchedule() const {
    cout << "\n========================================\n";
    cout << "--- Maintenance Schedule ---\n";
    cout << "========================================\n";

    auto maintenanceHistory = aircraftService.getMaintenanceHistory();

    if (maintenanceHistory.empty()) {
        cout << "No maintenance records found.\n";
        return;
    }

    cout << left << setw(6) << "ID" << setw(10) << "Aircraft"
         << setw(20) << "Date" << setw(12) << "Status" << "Description\n";
    cout << "----------------------------------------\n";

    for (const auto& record : maintenanceHistory) {
        string status = record->isCompleted() ? "Completed" : "Pending";
        cout << left << setw(6) << record->getMaintenanceId()
             << setw(10) << record->getAircraftId()
             << setw(20) << record->getMaintenanceDate()
             << setw(12) << status
             << record->getDescription() << "\n";
    }
}

void ManageAircraftUI::scheduleMaintenanceCheck() {
    cout << "\n========================================\n";
    cout << "--- Schedule Maintenance ---\n";
    cout << "========================================\n";

    auto allAircraft = aircraftService.getAllAircraft();
    if (allAircraft.empty()) {
        cout << "No aircraft available.\n";
        return;
    }

    cout << "Available Aircraft:\n";
    for (size_t i = 0; i < allAircraft.size(); ++i) {
        cout << i + 1 << ". " << allAircraft[i]->getRegistration()
             << " (" << allAircraft[i]->getModel() << ")\n";
    }

    int choice;
    cout << "Select Aircraft (1-" << allAircraft.size() << "): ";
    cin >> choice;
    cin.ignore();

    if (choice < 1 || choice > (int)allAircraft.size()) {
        cout << "Invalid selection.\n";
        return;
    }

    string aircraftId = allAircraft[choice - 1]->getAircraftId();

    string maintenanceDate, description;
    cout << "Maintenance Date (YYYY-MM-DD): ";
    getline(cin, maintenanceDate);

    cout << "Description: ";
    getline(cin, description);

    aircraftService.scheduleMaintenance(aircraftId, maintenanceDate, description);
    cout << "✓ Maintenance scheduled successfully.\n";
}