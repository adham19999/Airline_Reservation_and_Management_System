#ifndef MANAGE_FLIGHTS_UI_H
#define MANAGE_FLIGHTS_UI_H

#include <memory>
#include "../../services/flight/FlightService.h"
#include "../../services/flight/AircraftService.h"

using namespace std;

class ManageFlightsUI {
private:
    FlightService& flightService;
    AircraftService& aircraftService;
    
    void displayFlights() const;
    void addFlight();
    void updateFlightStatus();
    void deleteFlight();

public:
    ManageFlightsUI(FlightService& fs, AircraftService& as);
    
    void display();
};

#endif