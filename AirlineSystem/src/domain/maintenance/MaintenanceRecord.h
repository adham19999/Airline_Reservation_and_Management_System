#ifndef MAINTENANCE_RECORD_H
#define MAINTENANCE_RECORD_H

#include <string>
#include <ctime>

using namespace std;

class MaintenanceRecord {
private:
    string maintenanceId;
    string aircraftId;
    string maintenanceDate;
    string description;
    bool completed;
    string completionDate;

public:
    MaintenanceRecord(const string& maintenanceId,
                     const string& aircraftId,
                     const string& maintenanceDate,
                     const string& description);

    // Getters
    string getMaintenanceId() const;
    string getAircraftId() const;
    string getMaintenanceDate() const;
    string getDescription() const;
    bool isCompleted() const;
    string getCompletionDate() const;

    // Setters
    void markAsCompleted();
};

#endif