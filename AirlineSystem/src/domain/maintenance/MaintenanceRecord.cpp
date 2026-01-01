#include "MaintenanceRecord.h"
#include <ctime>
#include <sstream>
#include <iomanip>

using namespace std;

MaintenanceRecord::MaintenanceRecord(const string& maintenanceId,
                                     const string& aircraftId,
                                     const string& maintenanceDate,
                                     const string& description)
    : maintenanceId(maintenanceId),
      aircraftId(aircraftId),
      maintenanceDate(maintenanceDate),
      description(description),
      completed(false),
      completionDate("") {}

string MaintenanceRecord::getMaintenanceId() const {
    return maintenanceId;
}

string MaintenanceRecord::getAircraftId() const {
    return aircraftId;
}

string MaintenanceRecord::getMaintenanceDate() const {
    return maintenanceDate;
}

string MaintenanceRecord::getDescription() const {
    return description;
}

bool MaintenanceRecord::isCompleted() const {
    return completed;
}

string MaintenanceRecord::getCompletionDate() const {
    return completionDate;
}

void MaintenanceRecord::markAsCompleted() {
    completed = true;
    time_t now = time(nullptr);
    struct tm timeinfo;
    
    // Use localtime_s (Windows-safe version)
    localtime_s(&timeinfo, &now);
    
    stringstream ss;
    ss << (timeinfo.tm_year + 1900) << "-"
       << setw(2) << setfill('0') << (timeinfo.tm_mon + 1) << "-"
       << setw(2) << setfill('0') << timeinfo.tm_mday;
    completionDate = ss.str();
}

