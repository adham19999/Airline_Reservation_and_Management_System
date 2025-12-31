#ifndef AIRCRAFT_SERVICE_H
#define AIRCRAFT_SERVICE_H

#include <memory>
#include <vector>
#include <string>
#include "../../repository/AircraftRepository.h"
#include "../../domain/flight/Aircraft.h"
#include "../../domain/maintenance/MaintenanceRecord.h"

using namespace std;

enum class MaintenanceStatus {
    Operational,
    ScheduledMaintenance,
    UnderMaintenance,
    OutOfService
};

class AircraftService {
private:
    AircraftRepository& aircraftRepo;
    vector<shared_ptr<MaintenanceRecord>> maintenanceRecords;

    bool isAircraftBusy(const string& aircraftId, const string& date) const;

public:
    AircraftService(AircraftRepository& repo);

    // Aircraft CRUD operations
    shared_ptr<Aircraft> addAircraft(const string& registration,
                                     const string& model,
                                     const string& manufacturer,
                                     int economySeats,
                                     int businessSeats);

    shared_ptr<Aircraft> getAircraftById(const string& aircraftId) const;
    const vector<shared_ptr<Aircraft>>& getAllAircraft() const;
    
    void updateAircraftDetails(const string& aircraftId,
                              const string& model,
                              const string& manufacturer,
                              int economySeats,
                              int businessSeats);
    
    void deleteAircraft(const string& aircraftId);

    // Maintenance management
    void scheduleMaintenance(const string& aircraftId,
                            const string& maintenanceDate,
                            const string& description);
    
    void completeMaintenance(const string& aircraftId);
    
    bool isAircraftAvailable(const string& aircraftId, const string& date) const;
    
    MaintenanceStatus getMaintenanceStatus(const string& aircraftId) const;
    
    const vector<shared_ptr<MaintenanceRecord>>& getMaintenanceHistory() const;
};

#endif