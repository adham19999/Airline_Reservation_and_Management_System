#ifndef MANAGE_AIRCRAFT_UI_H
#define MANAGE_AIRCRAFT_UI_H

#include <memory>
#include "../../services/flight/AircraftService.h"

using namespace std;

class ManageAircraftUI {
private:
    AircraftService& aircraftService;

    void displayAllAircraft() const;
    void addNewAircraft();
    void updateAircraftDetails();
    void deleteAircraftRecord();
    void viewMaintenanceSchedule() const;
    void scheduleMaintenanceCheck();

public:
    ManageAircraftUI(AircraftService& as);

    void display();
};

#endif