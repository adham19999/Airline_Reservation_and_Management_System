#include "AircraftService.h"
#include <iostream>
#include <sstream>
#include <algorithm>

using namespace std;

AircraftService::AircraftService(AircraftRepository& repo)
    : aircraftRepo(repo) {}

shared_ptr<Aircraft> AircraftService::addAircraft(const string& registration,
                                                  const string& model,
                                                  const string& manufacturer,
                                                  int economySeats,
                                                  int businessSeats) {
    // Check registration doesn't exist
    if (aircraftRepo.findByRegistration(registration)) {
        cerr << "Error: Aircraft with registration '" << registration << "' already exists.\n";
        return nullptr;
    }

    // Generate aircraft ID
    static int aircraftCounter = 1;
    stringstream ss;
    ss << "AC" << aircraftCounter++;
    string aircraftId = ss.str();

    int totalSeats = economySeats + businessSeats;
    auto aircraft = make_shared<Aircraft>(aircraftId, registration, model, 
                                         manufacturer, economySeats, 
                                         businessSeats, totalSeats);

    aircraftRepo.addAircraft(aircraft);
    cout << "Aircraft added successfully with ID: " << aircraftId << "\n";
    
    return aircraft;
}

shared_ptr<Aircraft> AircraftService::getAircraftById(const string& aircraftId) const {
    return aircraftRepo.findByAircraftId(aircraftId);
}

const vector<shared_ptr<Aircraft>>& AircraftService::getAllAircraft() const {
    return aircraftRepo.getAllAircraft();
}

void AircraftService::updateAircraftDetails(const string& aircraftId,
                                           const string& model,
                                           const string& manufacturer,
                                           int economySeats,
                                           int businessSeats) {
    auto aircraft = aircraftRepo.findByAircraftId(aircraftId);
    if (!aircraft) {
        cerr << "Error: Aircraft '" << aircraftId << "' not found.\n";
        return;
    }

    // Create updated aircraft object
    int totalSeats = economySeats + businessSeats;
    auto updatedAircraft = make_shared<Aircraft>(aircraftId, aircraft->getRegistration(),
                                                model, manufacturer, economySeats,
                                                businessSeats, totalSeats);

    aircraftRepo.updateAircraft(updatedAircraft);
    cout << "Aircraft details updated successfully.\n";
}

void AircraftService::deleteAircraft(const string& aircraftId) {
    auto aircraft = aircraftRepo.findByAircraftId(aircraftId);
    if (!aircraft) {
        cerr << "Error: Aircraft '" << aircraftId << "' not found.\n";
        return;
    }

    aircraftRepo.deleteAircraft(aircraftId);
    cout << "Aircraft deleted successfully.\n";
}

void AircraftService::scheduleMaintenance(const string& aircraftId,
                                         const string& maintenanceDate,
                                         const string& description) {
    auto aircraft = aircraftRepo.findByAircraftId(aircraftId);
    if (!aircraft) {
        cerr << "Error: Aircraft '" << aircraftId << "' not found.\n";
        return;
    }

    static int maintenanceCounter = 1;
    stringstream ss;
    ss << "M" << maintenanceCounter++;
    string maintenanceId = ss.str();

    auto record = make_shared<MaintenanceRecord>(maintenanceId, aircraftId,
                                                maintenanceDate, description);
    maintenanceRecords.push_back(record);
    cout << "Maintenance scheduled successfully for " << maintenanceDate << "\n";
}

void AircraftService::completeMaintenance(const string& aircraftId) {
    for (auto& record : maintenanceRecords) {
        if (record->getAircraftId() == aircraftId && !record->isCompleted()) {
            record->markAsCompleted();
            cout << "Maintenance marked as completed.\n";
            return;
        }
    }
    cerr << "Error: No pending maintenance found for aircraft '" << aircraftId << "'.\n";
}

bool AircraftService::isAircraftAvailable(const string& aircraftId, const string& date) const {
    auto aircraft = aircraftRepo.findByAircraftId(aircraftId);
    if (!aircraft) return false;

    // Check if aircraft is under maintenance on this date
    for (const auto& record : maintenanceRecords) {
        if (record->getAircraftId() == aircraftId && 
            !record->isCompleted() && 
            record->getMaintenanceDate() == date) {
            return false;
        }
    }

    return true;
}

MaintenanceStatus AircraftService::getMaintenanceStatus(const string& aircraftId) const {
    for (const auto& record : maintenanceRecords) {
        if (record->getAircraftId() == aircraftId && !record->isCompleted()) {
            return MaintenanceStatus::UnderMaintenance;
        }
    }
    return MaintenanceStatus::Operational;
}

const vector<shared_ptr<MaintenanceRecord>>& AircraftService::getMaintenanceHistory() const {
    return maintenanceRecords;
}