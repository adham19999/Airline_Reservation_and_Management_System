#ifndef MANAGE_CREW_UI_H
#define MANAGE_CREW_UI_H

#include <memory>
#include "../../services/flight/CrewService.h"
#include "../../services/flight/FlightService.h"

using namespace std;

class ManageCrewUI {
private:
    CrewService& crewService;
    FlightService& flightService;

    void displayAllCrew() const;
    void addNewPilot();
    void addNewFlightAttendant();
    void deleteCrewMember();
    void assignCrewToFlight();
    void viewCrewAssignments() const;
    void viewFlightHoursReport() const;

public:
    ManageCrewUI(CrewService& cs, FlightService& fs);

    void display();
};

#endif